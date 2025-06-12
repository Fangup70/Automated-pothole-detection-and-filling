// Optimized version of your Arduino code with improved SerialEvent handling

const int pingPin =10; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 11; // Echo Pin of Ultrasonic Sensor

int IN2=2;    //FORWARD BACKWARD
int IN1=3;
int IN4=4;    
int IN3=5;

int IN5=6;     //FILLING
int IN6=7;

int IN7=8;    // LEVELING
int IN8=9;

String command = "";

void FORWARD();
void REVERSE();
void LEFT();
void RIGHT();
void STOP();
void SerialEvent();
void OPEN_MUD();
void OPEN_MUD_FIX();
void CLOSE_MUD_FIX();
void LEVEL_DOWN();
void LEVEL_UP();
long microsecondsToCentimeters(long microseconds);
void executeCommand(String cmd);
void sendResponse(String msg);
char Serial_read(void);
void WIFI(void);
void SENSOR_UV();

void setup() {
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(IN5,OUTPUT);
  pinMode(IN6,OUTPUT);
  pinMode(IN7,OUTPUT);
  pinMode(IN8,OUTPUT);

//  delay(5000);
  Serial.begin(115200);
  Serial.println("POTHOLE DETECTION ROBOT..");
  delay(5000);
  FORWARD();
  

  WIFI();
  sendResponse("MVJCE");
}

void loop() {
  SerialEvent();
}

char Serial_read(void) {
  char ch;
  while(Serial.available() == 0);
  ch = Serial.read(); 
  return ch;
}

void WIFI(void) {
  Serial.print("ATE0\r\n"); delay(50);
  Serial.print("AT\r\n"); delay(50);
  Serial.print("AT+CWMODE=2\r\n"); delay(50);
  Serial.print("AT+CIPAP=\"192.168.10.1\"\r\n"); delay(50);
  Serial.print("AT+CIPMUX=1\r\n"); delay(50);
  Serial.print("AT+CIPSERVER=1,80\r\n"); delay(50);
  Serial.print("AT+CIFSR\r\n"); delay(50);
  sendResponse("TRACKING.....");
}

void SerialEvent() {
  while (Serial.available() > 0) {
    char ch = Serial.read();
    if (ch == '$') {
      command = ""; // start of new command
    } else if (ch == '\n' || ch == '\r') {
      if (command.length() > 0) {
        executeCommand(command);
        command = "";
      }
    } else {
      command += ch;
    }
  }
  SENSOR_UV();
}

void executeCommand(String cmd) {
  if (cmd == "F") {
    FORWARD(); sendResponse("FORWARD MOVEMENT");
  } else if (cmd == "R") {
    REVERSE(); sendResponse("REVERSE MOVEMENT");
  } else if (cmd == "S") {
    STOP(); sendResponse("STOP");
  } else if (cmd == "O") {
    OPEN_MUD(); sendResponse("OPEN_MUD");
  } else if (cmd == "D") {
    LEVEL_DOWN(); sendResponse("LEVEL_FILL");
  } else if (cmd == "U") {
    LEVEL_UP(); sendResponse("LEVEL_FILL");
  } else if (cmd == "B") {
    RIGHT(); sendResponse("TURN_RIGHT");
  } else if (cmd == "L") {
    LEFT(); sendResponse("TURN_LEFT");
  } else if (cmd == "V") {
    OPEN_MUD_FIX(); sendResponse("OPEN_MUD_FIX");
  } else if (cmd == "C") {
    CLOSE_MUD_FIX(); sendResponse("CLOSE_MUD_FIX");
  }
}

void sendResponse(String msg) {
  Serial.write("AT+CIPSEND=0,15\r\n");
  delay(50);
  Serial.print(msg);
  delay(50);
  Serial.write("\n\r\r");
  delay(100);
}

void SENSOR_UV() {
  long duration, cm;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(pingPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  cm = microsecondsToCentimeters(duration);

  if(cm > 5) {
    Serial.println("POTHOLE DETECTED1:" + String(cm));
    sendResponse("POTHOLE DETECTED1");
    STOP(); delay(2000);
    RIGHT(); delay(500);
    STOP(); delay(1000);
    FORWARD(); delay(1800);
    STOP(); delay(1000);
    LEFT(); delay(500);
    STOP(); delay(1000);
    OPEN_MUD(); delay(1000);
    LEVEL_DOWN(); delay(1000);
    FORWARD(); delay(3000);
    STOP(); delay(1000);
    LEVEL_UP(); delay(1000);
    FORWARD(); delay(1000);
    STOP(); delay(1000);
    LEVEL_DOWN(); delay(1000);
    REVERSE(); delay(4000);
    STOP(); delay(1000);
    LEVEL_UP(); delay(1000);
    FORWARD();
  }
}

void OPEN_MUD() {
  Serial.println("POTHOLE FILLING..");
  digitalWrite(IN5,HIGH);
  digitalWrite(IN6,LOW);
  delay(500);
  digitalWrite(IN5,LOW);
  digitalWrite(IN6,LOW); 
  delay(12000);
  digitalWrite(IN5,LOW);
  digitalWrite(IN6,HIGH);
  delay(500);
  digitalWrite(IN5,LOW);
  digitalWrite(IN6,LOW);
}

void OPEN_MUD_FIX() {
  Serial.println("POTHOLE FILLING..");
  digitalWrite(IN5,HIGH);
  digitalWrite(IN6,LOW);
  delay(50);
  digitalWrite(IN5,LOW);
  digitalWrite(IN6,LOW);
}

void CLOSE_MUD_FIX() {
  Serial.println("POTHOLE FILLING..");
  digitalWrite(IN5,LOW);
  digitalWrite(IN6,HIGH);
  delay(50);
  digitalWrite(IN5,LOW);
  digitalWrite(IN6,LOW);
}

void LEVEL_DOWN() {
  Serial.println("POTHOLE LEVELLING");
  digitalWrite(IN7,LOW);
  digitalWrite(IN8,HIGH);
  delay(600);
  digitalWrite(IN7,LOW);
  digitalWrite(IN8,LOW);
  delay(1000);
}

void LEVEL_UP() {
  Serial.println("POTHOLE LEVELLED");
  digitalWrite(IN7,HIGH);
  digitalWrite(IN8,LOW);
  delay(600);
  digitalWrite(IN7,LOW);
  digitalWrite(IN8,LOW);
  delay(1000);
}

void FORWARD() {
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
}

void REVERSE() {
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
}

void LEFT() {
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
}

void RIGHT() {
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
}

void STOP() {
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
