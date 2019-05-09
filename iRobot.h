#include <Wire.h>
#include <WiFi.h>
unsigned char IRthreshold = 180;
unsigned int baseSpeed = 70;
unsigned int Kp = 10;
unsigned char stopPerLine = 2;
unsigned char DISTANCE = 4;
//Pin Setup
//Right Motor Pins
const char motor1Pin1 = 18;
const char motor1Pin2 = 19;
const char enA = 5;
//Left Motor Pins
const char motor2Pin1 = 16;
const char motor2Pin2 = 17;
const char enB = 4;
// Setting PWM properties
const unsigned int freq = 30000;
const char pwmChannelB = 1;
const char pwmChannelA = 0;
const char resolution = 8;

//WiFi communication protocol
const char MESSAGE_MEET_STOP = 'M';
const char MESSAGE_RECEIVED = 'R';
const char MESSAGE_CONFIRMED = 'C';
const char* MASTER_IP = "192.168.1.199";
const uint16_t port = 80; 
//-------------------------------------------------------
//WiFi 
const char* ssid = "iRobotServer";
const char* password =  "GN000001";
WiFiServer wifiServer(80);
WiFiClient client;
//Variable initialization
float ek;
bool GO;
unsigned char lineFollower[8];      //Processed signal from the sensor
unsigned char _index;               //index for storing linefollower data
unsigned char detected;             //Variable for calculating error
unsigned char counter;              //Variable for calculating error
unsigned char stopCounter;          //Where the robot is 
unsigned char OTLCheck;             //How long to stop if no line read
unsigned char stopCheck;            //How many times a stop has met
unsigned char distanceCounter;      //How long to stop 
unsigned char stopFilter;           //How many times to confirm a stop is met

unsigned int sensorSum;             //Variable for calculating dynamic sensor threshold
bool onStop = false;


enum State {Initialize, Line, Wait};
class iRobot {
private:
  State currentState;

public:

  iRobot() {
    currentState = Initialize;
    ek = 0;
  }

  //Input: leftspeed and rightspeed for driving motor
  void drive(float leftSpeed, float rightSpeed) {
    //Setup Direction for Positive
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);  
    if ( rightSpeed < 0) {
      rightSpeed *= -1;
      digitalWrite(motor2Pin1, HIGH);
      digitalWrite(motor2Pin2, LOW);
    }
    if ( leftSpeed < 0) {
      leftSpeed *= -1;
      digitalWrite(motor1Pin1, LOW);
      digitalWrite(motor1Pin2, HIGH);
    }
    //Limitation of speed
    if ( leftSpeed  > 100) {
      leftSpeed  = 100;
    }
    if (rightSpeed > 100) {
      rightSpeed = 100;
    }
    
    leftSpeed = map(leftSpeed, 0, 100, 0, 255);
    rightSpeed = map(rightSpeed, 0, 100, 0, 255);
    ledcWrite(pwmChannelB, leftSpeed);
    ledcWrite(pwmChannelA, rightSpeed);
  }

  //Check the line and update controller output for each cycle
  State updateState() {
    //Read data from the line follower sensor
    detected = 0;
    counter = 0;
    ek = 0.0;
    readSensor();
    //printSensor();
    analyzeSensor();
    errorCheck();
    return currentState;
  }

  
  void readSensor() {
    sensorSum = 0;
    Wire.requestFrom(9, 16);    // request 16 bytes from slave device #9
    _index = 0;
    while (Wire.available())   // slave may send less than requested
    {
      if (_index%2==0){
        lineFollower[_index/2] = Wire.read();
        //Serial.print(lineFollower[_index/2]);
        sensorSum+=lineFollower[_index/2]; // receive a byte as character
      }
      else {
        Wire.read();
      }
      _index++;
    }
    delay(70);
    //Calculate dynamic threshold
    IRthreshold = sensorSum/8;
    if (IRthreshold < 50) {
      IRthreshold = 50;
    }
    else if (IRthreshold > 200) {
      IRthreshold = 200;
    }
    for (int i = 0; i < 8; i++) {
      lineFollower[i] = (lineFollower[i]<IRthreshold);
    }
  }

  //Setup for all
  void pinSetup() {
    //Pin setup
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    pinMode(enA, OUTPUT);
    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);
    pinMode(enB, OUTPUT);

    ledcSetup(pwmChannelA, freq, resolution);
    ledcSetup(pwmChannelB, freq, resolution);
    ledcAttachPin(enA, pwmChannelA);
    ledcAttachPin(enB, pwmChannelB);

    //Sensor setup
    Wire.begin();
    Serial.begin(115200);

    //WiFi setup
    WiFi.begin(ssid, password);
 
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi..");
    } 
    Serial.println("Connected to the WiFi network");
    Serial.println(WiFi.localIP());
  
    wifiServer.begin();

    //Variable initialization
    currentState = Line;
    stopCounter = 0;
    stopCheck = 0;
    OTLCheck = 0;
    distanceCounter = 0;
    GO = false;

    //Listen from server for parameters
    char values[6] = {0,0,0,0,0,0}; //stopCounter,DISTANCE,Kp,baseSpeed,stopFilter,stopPerLine
    for (int i = 0; i < sizeof(values); i++) {
      client = wifiServer.available();
      char c = NULL;
      if (client) {
        //Serial.println("Master Connected!");
        while(client.connected()){
          while(client.available()) {
            c = client.read();   
          }
          delay(10);
        }
        client.stop();
      }
      if (c == NULL) {
        i--;
      }
      else {
        values[i] = c;
      }    
    }
    stopCounter = values[0];
    DISTANCE = values[1];
    Kp = values[2];
    baseSpeed = values[3];
    stopFilter = values[4];
    stopPerLine = values[5];
    
    readSensor();
    delay(1000);
    readSensor();
    printSensor();
  }

 //
 //Read input command from WiFi if available and update system state.
 //
 void readCommand(){
  client = wifiServer.available();
  char c = NULL;
  if (client) {
    //Serial.println("Master Connected!");
    while(client.connected()){
      while(client.available()) {
        c = client.read();
        //Serial.print(c);      
      }
      delay(10);
    }
    client.stop();
  }
  if (c != NULL) {
    if (c == 's') {
      wait();
    }
    else if (GO == false && c == 'g') {
      GO = true; 
      //Wire.begin();
      currentState = Line;
      drive(baseSpeed+5,baseSpeed+5);
      delay(600);
    }
  }
 }


 //
 //Disable motor and sensor reading
 //
  void wait() {
    GO = false;
    drive(0,0);
    //Serial.end();
    //Wire.endTransmission();
    //delay(3000);
    //delay(60000*59);
  }

  //
  //Check sensor signal is currect or not
  //
  void errorCheck() {
    if (counter == 8) {
      OTLCheck++;
      if (OTLCheck == 10) {
        drive(0,0);
        while(1){
        }
      }
    }
    else if (counter > 4) {//&& lineFollower[0] == 0 && lineFollower[7] == 0
      OTLCheck = 0;
      stopCheck++;
      if (stopCheck == stopFilter && !onStop) {
        distanceCounter++;
        onStop = true;
        if (distanceCounter == DISTANCE) {
          meetStop();
          stopCheck = 0;
          distanceCounter = 0;
        }
      }
    }
    else if (counter > 0){
      ek = (3.5 - float(detected)/counter);
      onStop = false;
      stopCheck = 0;
      OTLCheck = 0;
    }
    else if (counter == 0) {
      OTLCheck++;
      if (OTLCheck == 10) {
        drive(0,0);
        while(1){
        }
      }
    }
  }

  //
  //Process readings from the sensor
  //
  void analyzeSensor() {
    //Turns
    bool onLine = false; 
      //Turns at left half circle
      if (stopCounter <= (stopPerLine * 2)) {
        for (int i = 0; i < 8; i++) {
          if (lineFollower[i]) {
            counter++;
            detected += i;
            onLine = true;
          }
          else if (onLine) {
            if (counter>2 && counter < 4) {
              counter -=1;
              detected-=i;
            }
            break;
          }
        }
      }
      //Turns at right half circle
      else {
        for (int i = 7; i >= 0; i--) {
          if (lineFollower[i]) {
            counter++;
            detected += i;
            onLine = true;
          }
          else if(onLine) {
            if (counter>2 && counter < 4) {
              counter -=1;
              detected-=i;
            }
            break;
          }
        }
      }
  }

  
  void meetStop() {
    currentState = Wait;
    drive(0,0);
    stopCounter += 1;
    if (stopCounter == (stopPerLine * 4 + 1)) {
      stopCounter = 1;
    }
    //sendtoMaster(stopCounter+'0');
    sendtoMaster(MESSAGE_MEET_STOP);
  }

  //Send massage to server
  void sendtoMaster(char message) {
    bool confirmed = false;
    while(!confirmed){                                    //While not receive confirmation from master
      client = wifiServer.available();

      if (client) {
        while (client.connected()) {
          while(client.available()) {
            char input = client.read();
            if (input == MESSAGE_CONFIRMED) {
              confirmed = true;
              //Serial.print("Commands sent for");
              //Serial.println(message);
            }
          }
          delay(10);
        }
        client.stop();
      }
      if (!confirmed){
      while(!client.connect(MASTER_IP,port)){
        delay(1000);
      }
      client.write(message);
      client.stop();
      }
    }
  }
  
  //
  //Print processed sensor signal through serial/WiFi 
  //
  void printSensor() {
    for (int i = 0; i < 8; i++) {
    //  client.print(lineFollower[i]);
      Serial.print(lineFollower[i]); 
     // client.print(" "); 
      Serial.print(" ");
    }
    //client.print("\r\n");
    Serial.print("\r\n");
  }
};
