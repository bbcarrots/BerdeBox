//--------------------------------------------------------------- IMPORTS
#include "Arduino.h"
#include <WiFiManager.h> // WIFI
#include "esp_camera.h" // CAMERA
#include "soc/soc.h"           // Disable brownout problems
#include "soc/rtc_cntl_reg.h"  // Disable brownout problems
#include "driver/rtc_io.h"
#include <LittleFS.h> // FILESYSTEM
#include <FS.h>
#include <Firebase_ESP_Client.h> // FIREBASE
#include <addons/TokenHelper.h>
#include <esp_now.h> // ESP NOW
//--------------------------------------------------------------- VARIABLE DEFINITIONS
#define API_KEY "API_KEY"
#define USER_EMAIL "USER_EMAIL"
#define USER_PASSWORD "USER_PASSWORD"
#define STORAGE_BUCKET_ID "STORAGE_BUCKET_ID" // FIREBASE DETAILS

#define FILE_PHOTO_PATH "/photo.jpg" // Photo File Name to save in LittleFS

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22 // OV2640 camera module pins (CAMERA_MODEL_AI_THINKER)
//--------------------------------------------------------------- FIREBASE
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig configF;
//--------------------------------------------------------------- FUNCTION DECLARATIONS
void fcsUploadCallback(FCS_UploadStatusInfo info);
void initWiFi();
void initLittleFS();
void initCamera();

void OnDataSent(uint8_t *mac_addr, esp_now_send_status_t status);
void OnDataRecv(const esp_now_recv_info * mac, const uint8_t *incomingData, int len);
//--------------------------------------------------------------- SOFTWARE VARIABLES
uint8_t esp8266Address[] = {0x4C, 0x11, 0xAE, 0x05, 0x37, 0x82};

typedef struct takePhotoDetail {
    char command;
    int id;
} takePhotoDetail;

takePhotoDetail sendData; // holds data to send to esp8266
takePhotoDetail receiveData; // holds data received from esp8266

char incomingCommand; // holds the incoming command from ESP8266
int incomingID; // holds the incoming photo id from ESP8266
int previousID;

int takeNewPhoto = 0;
int hasTakenPhoto = 0;
int taskCompleted = 0;
int sendResponse = 0;

esp_now_peer_info_t peerInfo;
//--------------------------------------------------------------- HARDWARE VARIABLES
const int flash = 4; // pin 4 is the flash on the board
//------------------------------------------------------------------------------------------------------------------------------ ARDUINO FUNCTIONS
//--------------------------------------------------------------- ESP NOW FUNCTIONS
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // Callback function called when a message from ESP32 has been sent to the ESP8266
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail Retrying");
  if (status == 0){ 
    // If successful reply/response sending, reset the variables
    takeNewPhoto = 0;
    sendResponse = 1;
  }
}

void OnDataRecv(const esp_now_recv_info * mac, const uint8_t *incomingData, int len) {
  // Callback function called when ESP32 receives a response from ESP8266
  // Gets the command and photo id
  // Sets the variables to enable taking a photo
  memcpy(&receiveData, incomingData, sizeof(receiveData));
  Serial.print("ESP8266 Payload: ");
  Serial.print(receiveData.command);
  Serial.print(" ");
  Serial.print(receiveData.id);
  incomingCommand = receiveData.command;
  incomingID = receiveData.id;
  takeNewPhoto = 1;
  hasTakenPhoto = 0;
  taskCompleted = 0;
  sendResponse = 0;
}
//--------------------------------------------------------------- CAMERA FUNCTIONS
void initLittleFS() {
  // Initializes the File System
  if (!LittleFS.begin(true)) {
    Serial.println("An Error has occurred while mounting LittleFS");
    ESP.restart();
  }
  else {
    delay(500);
    Serial.println("LittleFS mounted successfully");
  }
}

void initCamera() {
 // Initializes the OV2640 camera module
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.grab_mode = CAMERA_GRAB_LATEST;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 1;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    ESP.restart();
  } 
}

void fcsUploadCallback(FCS_UploadStatusInfo info){
  // Callback function for uploading the image to the Firebase Storage
  if (info.status == firebase_fcs_upload_status_init){ // Prints update status percentage
    Serial.printf("Uploading file %s (%d) to %s\n", info.localFileName.c_str(), info.fileSize, info.remoteFileName.c_str());
  } else if (info.status == firebase_fcs_upload_status_upload) {
    Serial.printf("Uploaded %d%s, Elapsed time %d ms\n", (int)info.progress, "%", info.elapsedTime);
  } else if (info.status == firebase_fcs_upload_status_complete) { // Upload has finished
    Serial.println("Upload completed\n");
    FileMetaInfo meta = fbdo.metaData();
    Serial.printf("Name: %s\n", meta.name.c_str());
    Serial.printf("Bucket: %s\n", meta.bucket.c_str());
    Serial.printf("contentType: %s\n", meta.contentType.c_str());
  } else if (info.status == firebase_fcs_upload_status_error){ // If upload failed, try again
    Serial.printf("Upload failed, %s\n", info.errorMsg.c_str());
    // ESP.restart();
    taskCompleted = 0;
  }
}

void capturePhotoSaveLittleFS( void ) {
  // Handles capturing photos and saving it to LittleFS
  // Dispose first pictures because of bad quality
  camera_fb_t* fb = NULL;
  // Skip first 3 frames (increase/decrease number as needed).
  for (int i = 0; i < 4; i++) {
    fb = esp_camera_fb_get();
    esp_camera_fb_return(fb);
    fb = NULL;
  }
    
  // Take a new photo
  digitalWrite(flash, 1);
  fb = NULL;  
  fb = esp_camera_fb_get();  
  if(!fb) {
    Serial.println("Camera capture failed");
    delay(1000);
    ESP.restart();
  }  
  digitalWrite(flash, 0);
  // Photo file name
  Serial.printf("Picture file name: %s\n", FILE_PHOTO_PATH); // Only saves one image in the filesystem using the same filename
  File file = LittleFS.open(FILE_PHOTO_PATH, FILE_WRITE);

  // Insert the data in the photo file
  if (!file) {
    Serial.println("Failed to open file in writing mode");
  }
  else {
    file.write(fb->buf, fb->len); // payload (image), payload length
    Serial.print("The picture has been saved in ");
    Serial.print(FILE_PHOTO_PATH);
    Serial.print(" - Size: ");
    Serial.print(fb->len);
    Serial.println(" bytes");
  }
  // Close the file
  file.close();
  esp_camera_fb_return(fb);
}
//--------------------------------------------------------------- DO NOT TOUCH BELOW THIS LINE
void initWiFi() {
  // Sets up WiFi connection using WiFi Manager
  // If no valid connection, ESP will become an access point
  // Connect to "0x0 WiFi ESP32 Config" from any mobile phone to configure the WiFi connection of the ESP
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  // wm.resetSettings(); // For resetting saved parameters
  bool connected =  wm.autoConnect("ESP32 0x0 WiFi Config");
  if (!connected) {
    Serial.println("Failed to connect :<");
    ESP.restart();
  } else { Serial.println("Connected to WiFi successfully :>"); }
}
//--------------------------------------------------------------- SETUP AND LOOP
void setup() {
  pinMode(flash, OUTPUT);
  Serial.begin(115200);
  //--------------------------------------------------------------- HARDWARE SETUP
  digitalWrite(flash, 0);
  //--------------------------------------------------------------- SOFTWARE SETUP
  initWiFi();
  // Serial.println(WiFi.macAddress());
  initLittleFS();
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // Turn-off the 'brownout detector'
  initCamera();
  //--------------------------------------------------------------- FIREBASE SETUP
  configF.api_key = API_KEY; //
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  configF.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
  Firebase.begin(&configF, &auth);
  Firebase.reconnectWiFi(true);
  //--------------------------------------------------------------- ESP NOW SETUP
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  memcpy(peerInfo.peer_addr, esp8266Address, 6); // Register peer
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK){ // Add peer 
    Serial.println("Failed to add peer");
    return;
  }
  
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv); // Register for a callback function that will be called when data is received
  //--------------------------------------------------------------- SEPARATE
  Serial.print(".");
  Serial.println();
  Serial.print(".");
  Serial.println();
  Serial.print(".");
  Serial.println();
}

void loop() {
  if (takeNewPhoto == 1) { // If ESP32 receives a command to take a photo, we reply with received and start a capture sequence
    if (sendResponse == 0) { 
      sendData.command = 'r';
      sendData.id = 0;
      esp_now_send(esp8266Address, (uint8_t *) &sendData, sizeof(sendData));
    }
    capturePhotoSaveLittleFS();
    takeNewPhoto = 0;
    hasTakenPhoto = 1;
  }
  delay(1);
  // After taking a photo, upload to Firebase Storage
  if (Firebase.ready() && taskCompleted == 0 && takeNewPhoto == 0 && hasTakenPhoto == 1 && previousID != incomingID) { // Blocking
    Serial.print("Uploading picture... ");
    if (incomingCommand == 'm') {
      Firebase.Storage.upload(&fbdo, STORAGE_BUCKET_ID, FILE_PHOTO_PATH, mem_storage_type_flash, "/berdebox1/" + String(incomingID) + "-success-" + "mbopen", "image/jpeg", fcsUploadCallback);
    } else if (incomingCommand == 'c') {
      Firebase.Storage.upload(&fbdo, STORAGE_BUCKET_ID, FILE_PHOTO_PATH, mem_storage_type_flash, "/berdebox1/" + String(incomingID) + "-success-" + "cb1open", "image/jpeg", fcsUploadCallback);
    } else {
      Firebase.Storage.upload(&fbdo, STORAGE_BUCKET_ID, FILE_PHOTO_PATH, mem_storage_type_flash, "/berdebox1/" + String(incomingID) + "-alert-doorbell", "image/jpeg", fcsUploadCallback);
    }
    taskCompleted = 1;
  }
  // If ESP32 has successfully uploaded the image, send a response to the ESP8266 for confirmation
  if (taskCompleted == 1) {
    sendData.command = 'u';
    sendData.id = 0;
    esp_now_send(esp8266Address, (uint8_t *) &sendData, sizeof(sendData));
    delay(200);
    esp_now_send(esp8266Address, (uint8_t *) &sendData, sizeof(sendData));
    delay(200);
    esp_now_send(esp8266Address, (uint8_t *) &sendData, sizeof(sendData));
    delay(200);
    esp_now_send(esp8266Address, (uint8_t *) &sendData, sizeof(sendData));
    delay(200);
    esp_now_send(esp8266Address, (uint8_t *) &sendData, sizeof(sendData));
    delay(200);
    taskCompleted = 0;
    hasTakenPhoto == 0;
    previousID = incomingID;
  }
}
