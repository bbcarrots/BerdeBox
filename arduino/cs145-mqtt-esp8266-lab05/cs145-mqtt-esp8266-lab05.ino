/* 
IMPORTANT!!! Please don't forget to do these:
1. Turn on MQTT Broker in Services
2. Change `mqttServer` to the WiFi IP Address of your computer.
3. Change the `ssid` and `password` to "dcs-students2" and "W1F14students". (For tracefile generation)
4. 
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// const char* ssid = "dcs-students2";
// const char* password = "W1F14students";
const char* ssid = "Graceland"; // CHANGE
const char* password = "Tisaybubayelvisringo!"; // CHANGE
const char* mqttServer = "192.168.68.57"; // CHANGE

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

//---------------------------------------------------------------
// HARDWARE VARIABLES
const int led = D8;
const int button = D7;
const int LDR = A0;

bool buttonState = HIGH;
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
//---------------------------------------------------------------
// SOFTWARE VARIABLES
char morseCode[100]; // stores the morse code when we press the button
int morseIndex = 0;

const char endMorseCode[9] = ". -. -..";

unsigned long lastInputTime = 0; // used for inserting a space in between encoding letters

// checks for the "END" flag for connection termination
bool isEnd = false;
bool isDisconnected = false;

// MAIN GROUP HERE // CHANGE
const char* mainGroupRawTopic = "cs145/B0x0Labs/RAW_MORSE/+"; // for receiving "DOT" or "DASH" from opposite group
const char* mainGroupOutTopic = "cs145/B0x0Labs/out"; // for receiving morse code from opposite group

// OPPOSITE GROUP HERE // CHANGE
// const char* oppositeGroupRawTopic = "cs145/IoT247/RAW_MORSE/+"; // for sending "DOT" or "DASH" to opposite group
const char* oppositeGroupOutTopic = "cs145/IoT247/out"; // for sending main group morse code to opposite group

const char* OGDashTopic = "cs145/IoT247/RAW_MORSE/dash"; // for sending the "DASH" and "DOT" string payload
const char* OGDotTopic = "cs145/IoT247/RAW_MORSE/dot";
//---------------------------------------------------------------

void setup_wifi() {
  // Connects to WiFi provided the name and password
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  // strcmp() will return 0 if strings are equal
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");

  // copies payload because when we publish, the payload gets overwritten
  char receivedData[100];
  memcpy(receivedData, payload, length);

  if (strcmp(topic, mainGroupOutTopic) == 0) { // only executes if we receive data in the mainGroupOutTopic topic
    bool isEndDetected = true;

    // lights up the LED based on the Morse code sent by opposite group
    for (int m = 0; m < length; m++) {
      Serial.print((char)receivedData[m]);
      if ((char)receivedData[m] == '.') {
        digitalWrite(led, 1);
        delay(250);
        digitalWrite(led, 0);
        client.publish(OGDotTopic, "DOT");
      } else if ((char)receivedData[m] == '-') {
        digitalWrite(led, 1);
        delay(1000);
        digitalWrite(led, 0);
        client.publish(OGDashTopic, "DASH");
      } else if ((char)receivedData[m] == ' ') {
        client.publish(OGDotTopic, "...");
        delay(750);
      }
      delay(250);

      // checks for "END" flag
      if (!(length == sizeof(endMorseCode) - 1) || (char)receivedData[m] != endMorseCode[m]) { // if payload length is the same as an "END" flag length
        isEndDetected = false;
      }
    }
    
    client.publish(OGDotTopic, "...");
    Serial.println("");

    // handles when we receive the "END" flag
    if (isEndDetected) {
      client.publish(oppositeGroupOutTopic, endMorseCode);
      isEnd = isEndDetected;
    }
  }

  // if (strcmp(topic, mainGroupRawTopic) {
  //   for (int h = 0; h < length; h++) {
  //     Serial.print(receivedData[h]);
  //   }
  //   Serial.println("");
  // }
}

void reconnect() {
  // Connects to the MQTT Broker and handles subscriptions to topics
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      //---------------------------------------------------------------      
      // SUBSCRIBE TO REQUIRED TOPICS
      client.subscribe(mainGroupOutTopic);
      // client.subscribe(mainGroupRawTopic);
      //---------------------------------------------------------------
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void readButton() {
  // Reads button presses and updates morseCode array and morseIndex accordingly
  int reading = digitalRead(button);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  // Debouncing logic. This is copied from the webserver.ino file
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {
        int ldrOutput = analogRead(LDR);
        if (ldrOutput < 768) { // LDR is covered - darkPresses
          morseCode[morseIndex] = '-';
          // Serial.print(morseCode[morseIndex]);
          morseIndex++;
          lastInputTime = millis();
          for (int i = 0; i < morseIndex; i++) {
            Serial.print(morseCode[i]);
          }
          Serial.println("");
        } else { // LDR is shone upon - lightPresses
          morseCode[morseIndex] = '.';
          // Serial.print(morseCode[morseIndex]);
          morseIndex++;
          lastInputTime = millis();
          for (int i = 0; i < morseIndex; i++) { // prints the content of morseCode array after every button press
            Serial.print(morseCode[i]);
          }
          Serial.println("");
        }
        digitalWrite(led, 0);
      } else {
        digitalWrite(led, 1);
      }
    }
  }

  lastButtonState = reading;
}

void checkSpace() {
  // Adds a space in the morseCode array to differentiate letters
  if (morseIndex > 0) {
    if (millis() - lastInputTime == 3000 && morseCode[morseIndex - 1] != ' ') {
      morseCode[morseIndex] = ' ';
      morseIndex++;
      for (int i = 0; i < morseIndex; i++) {
        Serial.print(morseCode[i]);
      }
      Serial.println("");
    }
  }
}

bool checkSendFlag() {
  // Checks if the end flag (SPACE) has been encoded

  if (morseIndex > 6) { // if our morseCode contains the minimum length for 7-dots, we check for end flag
    for (int k = morseIndex - 7; k < morseIndex; k++) {
      if (morseCode[k] != '.') {
        return false;
      }
    }

    return true;
  }

  return false;
}

void setup() {\
  // PIN DECLARATION
  pinMode(led, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  digitalWrite(led, 0);
  
  // SET UP WIFI
  Serial.begin(115200);
  setup_wifi();

  // SET UP MQTT CLIENTS HERE
  client.setServer(mqttServer, 1883);
  client.setCallback(callback);

  Serial.println(".");
  Serial.println(".");
  Serial.println(".");
}

void loop() {
  // Main controller. Reading of button presses and disconnection is here
  if (!isEnd) { // if we have not detected an "END" flag, we continue encoding and decoding
    if (!client.connected()) {
      reconnect();
    }
    client.loop();

    readButton();
    checkSpace();
    
    if (checkSendFlag()) {
      Serial.println("Clear to send!");
      morseCode[morseIndex - 8] = '\0';
      client.publish(oppositeGroupOutTopic, morseCode);
      morseIndex = 0;
    }
  } else if (isEnd && !isDisconnected) { // if we have received an "END" flag. we disconnect and stop all operations
    client.unsubscribe(mainGroupOutTopic);
    client.disconnect();
    isDisconnected = true;
    Serial.println("Device disconnected!");
  }
}
