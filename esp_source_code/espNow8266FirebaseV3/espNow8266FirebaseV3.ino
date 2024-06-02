//--------------------------------------------------------------- IMPORTS
#include <WiFiManager.h> // WIFI
#include <FirebaseClient.h> // FIREBASE
#include <WiFiClientSecure.h> // SSL_CLIENT
#include <Servo.h> // SERVO
#include <espnow.h> // COMMUNICATION BETWEEN ESP8266 and ESP32
//--------------------------------------------------------------- FIREBASE VARIABLES
#define API_KEY "API_KEY"
#define USER_EMAIL "USER_EMAIL"
#define USER_PASSWORD "USER_PASSWORD"
#define DATABASE_URL "DATABASE_URL"
//--------------------------------------------------------------- LIBRARY DECLARATIONS (?)
DefaultNetwork network;
WiFiClientSecure ssl_client;

FirebaseApp app;
RealtimeDatabase Database;
UserAuth user_auth(API_KEY, USER_EMAIL, USER_PASSWORD);
AsyncClientClass aClient(ssl_client, getNetwork(network));

Servo mainLockServo;
//--------------------------------------------------------------- FUNCTION DECLARATIONS
void setupWiFi();
void setupFirebase();
void asyncCB(AsyncResult &aResult); // Function declaration for asynchronous callback when we receive data from the DB

void handleMainLock(void);
void handleCB1Lock(void);
void handleDoorbellButton(void);
void handleMainLockLED(void);
void handleBuzzer(void);
void handleCB1LED(void);

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus);
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len);
void handleCamera(void);
//--------------------------------------------------------------- SOFTWARE VARIABLES
int mainLockIsOpenCurr = 0;
int mainLockIsOpenPrev = 0;
int mainLockTimeOpen = 0;
int mainLockTimeGet = 0;
const int turningAngle = 180; // MAIN LOCK VARIABLES

int cb1LockIsOpenCurr = 0;
int cb1LockIsOpenPrev = 0;
int cb1LockTimeOpen = 0;
int cb1LockTimeGet = 0; // CASHBOX LOCK VARIABLES

bool doorbellButtonState = LOW;
bool doorbellLastButtonState = LOW;
unsigned long doorbellLastDebounceTime = 0;
unsigned long doorbellDebounceDelay = 50; // DOORBELL VARIABLES

int buzzerIsOpenPrev = 0;
int buzzerTimeOpen = 0; // BUZZER VARIABLES

int ledIsOpenPrev = 0; // MAIN LOCK LED VARIABLE

int ledCB1IsOpenPrev = 0; // CASHBOX 1 LED VARIABLE
int cb1LockLEDTimeOpen = 0;

uint8_t esp32Address[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // ESP NOW VARIABLES

typedef struct takePhotoDetail {
    char command;
    int id;
} takePhotoDetail;

takePhotoDetail sendData; // holds data to send to esp32
takePhotoDetail receiveData; // holds data received from esp32

char incomingCommand;
int successSendCommand = 0; // 0 if ESP32 has not responded with command 'r' (receive), else 1
int successUploadImg = 0; // 0 if ESP32 has not responded with command 'u' (upload), else 1

int takePhotoCurr = 0;
int takePhotoPrev = 0;
int takePhotoID = 0;
int takePhotoCommand = 0;
int takePhotoTimeGet = 0; // TAKEPHOTO VARIABLES
//--------------------------------------------------------------- HARDWARE VARIABLES
const int mainLock = D12;
const int cb1Lock = D10;
const int doorbellButton = D8;

const int buzzerSound = D2;
const int mainLockLED = D13;
const int cb1LockLED = D11;
//------------------------------------------------------------------------------------------------------------------------------ ARDUINO FUNCTIONS
void setup() {
  pinMode(mainLock, OUTPUT);
  pinMode(cb1Lock, OUTPUT);
  pinMode(doorbellButton, INPUT_PULLUP);
  pinMode(buzzerSound, OUTPUT);
  pinMode(mainLockLED, OUTPUT);
  pinMode(cb1LockLED, OUTPUT);
  Serial.begin(115200); // Start Serial monitor
  //--------------------------------------------------------------- SOFTWARE SETUP
  setupWiFi();
  setupFirebase();
  // Serial.print("ESP Board MAC Address:  ");
  // Serial.println(WiFi.macAddress());
  //--------------------------------------------------------------- HARDWARE SETUP
  // digitalWrite(mainLock, 0);
  mainLockServo.attach(mainLock); 
  mainLockServo.write(turningAngle);
  digitalWrite(cb1Lock, 0);
  digitalWrite(buzzerSound, 0);
  digitalWrite(mainLockLED, 0);
  digitalWrite(cb1LockLED, 0);
  //--------------------------------------------------------------- ESP NOW SETUP
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    ESP.reset;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_send_cb(OnDataSent);
  esp_now_add_peer(esp32Address, ESP_NOW_ROLE_COMBO, 0, NULL, 0);
  esp_now_register_recv_cb(OnDataRecv);
  //--------------------------------------------------------------- SEPARATE
  Serial.println(".");
  Serial.println(".");
  Serial.println(".");
}

void loop() {
  //--------------------------------------------------------------- FIREBASE LOOP
  app.loop();
  Database.loop();

  if (app.ready()) {
    handleMainLock();
    handleCB1Lock();
    handleDoorbellButton();
    handleMainLockLED();
    handleBuzzer();
    handleCamera();
    handleCB1LED();
  }
}

void asyncCB(AsyncResult &aResult) {
  // Callback for getting values from Firebase Realtime Database
  if (aResult.available()) {
    // Firebase.printf("task: %s, payload: %s\n", aResult.uid().c_str(), aResult.c_str());
    // memcpy(task, aResult.uid().c_str(), sizeof(aResult.uid().c_str()));
    int result = aResult.to<RealtimeDatabaseResult>().to<int>(); // returns a 1 or 0

    // Firebase.printf("Task: %s Payload: %d %d\n", aResult.uid().c_str(), result, strcmp(aResult.uid().c_str(), "getMainLockStatus"));
    if (strcmp(aResult.uid().c_str(), "getMainLockStatus") == 0) { // Get Main Lock status
      Firebase.printf("MainLock %d\n", result);
      mainLockIsOpenCurr = result;
    } else if (strcmp(aResult.uid().c_str(), "getCB1LockStatus") == 0) { // Get Cash Box Lock status
      Firebase.printf("CashBox1 %d\n", result);
      cb1LockIsOpenCurr = result;
    } else if (strcmp(aResult.uid().c_str(), "getTakePhotoStatus") == 0) { // Get Take Photo status
      Firebase.printf("TakePhoto %d\n", result);
      takePhotoCurr = result;
    } else if (strcmp(aResult.uid().c_str(), "getTakePhotoID") == 0) { // Get Take Photo ID status
      Firebase.printf("TakePhotoID %d\n", result);
      takePhotoID = result;
    } else if (strcmp(aResult.uid().c_str(), "getTakePhotoCommand") == 0) { // Get Take Photo Command status
      Firebase.printf("TakePhotoCommand %d\n", result);
      takePhotoCommand = result;
    }
  }

  if (aResult.isError()) {
    Firebase.printf("Error task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.error().message().c_str(), aResult.error().code());
  }
}

void handleMainLock(void) {
  // Handles Main Lock logic
  if (millis() - mainLockTimeGet >= 1000) { // Gets the main_lock_is_open value every 1 second
    Database.get(aClient, "berdebox1/output/main_lock_is_open", asyncCB, false, "getMainLockStatus");
    mainLockTimeGet = millis();
  }

  if (mainLockIsOpenCurr != mainLockIsOpenPrev) { // Only triggers when Main Lock value changes to avoid redundant assignment/writing
    if (mainLockIsOpenCurr == 1) { // Opens the Main Lock
      // digitalWrite(mainLock, 1);
      mainLockServo.write(0);
      mainLockTimeOpen = millis();
      Serial.println("MainLock is OPEN.");
    } else if (mainLockIsOpenCurr == 0) { // Closes the Main Lock
      // digitalWrite(mainLock, 0);
      mainLockServo.write(turningAngle);
      Serial.println("MainLock is CLOSED.");
    }
    mainLockIsOpenPrev = mainLockIsOpenCurr;
  }
  
  if (mainLockIsOpenCurr == 1) {
    if (millis() - mainLockTimeOpen >= 45000 && mainLockTimeOpen != 0) { // Once 45 seconds has elapsed, automatically close the Main Lock and update Realtime DB
      object_t json;
      JsonWriter writer;
      writer.create(json, "main_lock_is_open", false); //-> {"data":{"value":x}}
      // Or set the seialized JSON string to the object_t as object_t("{\"data\":{\"value\":x}}")
      Serial.println("Close Main Lock!");
      Database.update(aClient, "berdebox1/output", json, asyncCB, "autoCloseMainLock");
      mainLockTimeOpen = millis();
    }
  }
}

void handleCB1Lock(void) {
  // Handles Cash Box 1 Lock logic
  if (millis() - cb1LockTimeGet >= 1100) { // Gets the cb1_lock_is_open value every 1.1 second
    Database.get(aClient, "berdebox1/output/modules/cb1_is_open", asyncCB, false, "getCB1LockStatus");
    cb1LockTimeGet = millis();
  }

  if (cb1LockIsOpenCurr != cb1LockIsOpenPrev) { // Only triggers when Cash Box 1 Lock value changes to avoid redundant assignment/writing
    if (cb1LockIsOpenCurr == 1) {
      digitalWrite(cb1Lock, 1);
      cb1LockTimeOpen = millis();
      Serial.println("cb1Lock is OPEN.");
    } else if (cb1LockIsOpenCurr == 0) {
      digitalWrite(cb1Lock, 0);
      Serial.println("cb1Lock is CLOSED.");
    }
    cb1LockIsOpenPrev = cb1LockIsOpenCurr;
  }
  
  if (cb1LockIsOpenCurr == 1) {
    if (millis() - cb1LockTimeOpen >= 1000 && cb1LockTimeOpen != 0) { // Once 1 second has elapsed, automatically close the Cash Box 1 Lock and update Realtime DB
      // This is done to avoid sending more than 12V for too long to the solenoid as we have not regulated it at the time
      object_t json;
      JsonWriter writer;
      writer.create(json, "cb1_is_open", false); //-> {"data":{"value":x}}
      // Or set the seialized JSON string to the object_t as object_t("{\"data\":{\"value\":x}}")
      Serial.println("Close cb1 Lock!");
      Database.update(aClient, "berdebox1/output/modules", json, asyncCB, "autoClosecb1Lock");
      cb1LockTimeOpen = millis();
    }
  }
}

void handleCB1LED(void) {
  // Handles the LED for the Cash Box 1. Opens when CB1 is open, and closes after 30 seconds
  if (cb1LockIsOpenCurr != ledCB1IsOpenPrev) {
    if (cb1LockIsOpenCurr == 1) {
      digitalWrite(cb1LockLED, 1);
      cb1LockLEDTimeOpen = millis();
    }
    ledCB1IsOpenPrev = cb1LockIsOpenCurr;
  }
  if (millis() - cb1LockLEDTimeOpen >= 30000) {
    digitalWrite(cb1LockLED, 0);
  }
}

void handleDoorbellButton(void) {
  // Handles the Doorbell button
  int reading = digitalRead(doorbellButton);

  if (reading != doorbellLastButtonState) {
    doorbellLastDebounceTime = millis();
  }

  // Debouncing logic.
  if ((millis() - doorbellLastDebounceTime) > doorbellDebounceDelay) { // Read doorbell input
    if (reading != doorbellButtonState) {
      doorbellButtonState = reading;

      if (doorbellButtonState == LOW) { // If doorbell is pressed, update the Realtime DB and open buzzer to indicate doorbell press
        object_t json;
        JsonWriter writer;
        writer.create(json, "doorbell_button_is_pressed", true); //-> {"data":{"value":x}}
        // Or set the seialized JSON string to the object_t as object_t("{\"data\":{\"value\":x}}")
        Serial.println("Doorbell rung!");
        Database.update(aClient, "berdebox1/input", json, asyncCB, "doorbellRing");

        digitalWrite(buzzerSound, 1);
        buzzerTimeOpen = millis();
      }
    }
  }

  doorbellLastButtonState = reading;

  if (millis() - buzzerTimeOpen >= 1000) {
    digitalWrite(buzzerSound, 0);
  }
}

void handleMainLockLED(void) {
  // Handles Main Lock LED. Opens the LED when ML is open, and closes when ML is closed.
  if (mainLockIsOpenCurr != ledIsOpenPrev) {
    if (mainLockIsOpenCurr == 1) {
      digitalWrite(mainLockLED, 1);
    } else if (mainLockIsOpenCurr == 0) {
      digitalWrite(mainLockLED, 0);
    }
    ledIsOpenPrev = mainLockIsOpenCurr;
  }
}

void handleBuzzer(void) {
  // Handles the Buzzer when the Main Lock is open. Opens Buzzer for 1 second when Main Lock is opened.
  if (mainLockIsOpenCurr != buzzerIsOpenPrev) {
    if (mainLockIsOpenCurr == 1) {
      digitalWrite(buzzerSound, 1);
      buzzerTimeOpen = millis();
    } 
    buzzerIsOpenPrev = mainLockIsOpenCurr;
  }
  if (millis() - buzzerTimeOpen >= 1000) {
    digitalWrite(buzzerSound, 0);
  }
}

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  // Callback function when a response is sent to the ESP32 to take a photo
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("ESP32 command sent!");
  }
  else{
    Serial.println("ESP32 command fail.");
  }
}

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  // Callback function when a reply is received from the ESP32
  memcpy(&receiveData, incomingData, sizeof(receiveData));
  Serial.print("ESP32 Payload: ");
  Serial.println(receiveData.command);
  incomingCommand = receiveData.command;
  
  if (incomingCommand == 'r') { // ESP32 has successfully received a command to take a photo
    successSendCommand = 1;
  } else if (incomingCommand == 'u') { // ESP32 has successfully uploaded the photo
    successUploadImg = 1;
  }
}

void handleCamera() {
  // Handles taking a photo by sending a command to the ESP32 
  if (millis() - takePhotoTimeGet >= 1700) { // gets take_photo and take_photo_id from the realtime database
    Database.get(aClient, "berdebox1/output/take_photo", asyncCB, false, "getTakePhotoStatus");
    Database.get(aClient, "berdebox1/output/take_photo_id", asyncCB, false, "getTakePhotoID");
    Database.get(aClient, "berdebox1/output/take_photo_command", asyncCB, false, "getTakePhotoCommand");
    takePhotoTimeGet = millis();
  }

  if (takePhotoCurr != takePhotoPrev) { // if take_photo == true, then we start the ESP32 command sequence
    if (takePhotoCurr == 1) {
      successSendCommand = 0;
      successUploadImg = 0;
      Serial.println("Take photo true!");
    } else if (takePhotoCurr == 0) {
      Serial.println("Take photo false!");
    }
    takePhotoPrev = takePhotoCurr;
  }

  if (takePhotoCurr == 1 && successSendCommand == 0) { // if we have not yet received a confirmation reply from ESP32, send a command to take a photo
    if (takePhotoCommand != 0) {
      if (takePhotoCommand == 1) { // Doorbell photo
        sendData.command = 'd';
      } else if (takePhotoCommand == 2) { // Main Lock photo
        sendData.command = 'm';
      } else if (takePhotoCommand == 3) { // Cash Box 1 photo
        sendData.command = 'c';
      }
      sendData.id = takePhotoID;

      esp_now_send(esp32Address, (uint8_t *) &sendData, sizeof(sendData));
    }
  }

  if (successUploadImg == 1 && takePhotoCurr == 1 && millis() - takePhotoTimeGet >= 1600) { // if photo upload is successful, update Realtime DB
    JsonWriter writer;
    object_t json1;
    writer.create(json1, "take_photo", false);
    Serial.println("Turn off Take Photo!");
    Database.update(aClient, "berdebox1/output", json1, asyncCB, "turnOffTakePhoto");
    
    object_t json2;
    writer.create(json2, "take_photo_id", takePhotoID + 1);
    Serial.println("Update Take Photo ID!");
    Database.update(aClient, "berdebox1/output", json2, asyncCB, "updateTakePhotoID");

    object_t json3;
    writer.create(json3, "take_photo_command", 0);
    Serial.println("Update Take Photo Command!");
    Database.update(aClient, "berdebox1/output", json3, asyncCB, "updateTakePhotoCommand");
  }
}

//--------------------------------------------------------------- DO NOT TOUCH BELOW THIS LINE

void setupWiFi() {
  // Sets up WiFi connection using WiFi Manager
  // If no valid connection, ESP will become an access point
  // Connect to "0x0 WiFi ESP8266 Config" from any mobile phone to configure the WiFi connection of the ESP
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  // wm.resetSettings(); // For resetting saved parameters
  bool connected =  wm.autoConnect("ESP8266 0x0 WiFi Config");
  if (!connected) {
    Serial.println("Failed to connect :<");
    ESP.reset();
  } else { Serial.println("Connected to WiFi successfully :>"); }
}

void setupFirebase() {
  // Sets up Firebase connection with realtime database
  Firebase.printf("Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);
  ssl_client.setInsecure();
  ssl_client.setBufferSizes(4096, 1024);
  initializeApp(aClient, app, getAuth(user_auth), asyncCB, "authTask");
  // initializeApp(aClient, app, getAuth(user_auth), aResult_no_callback); // NO CALLBACK
  app.getApp<RealtimeDatabase>(Database);
  Database.url(DATABASE_URL);
}
