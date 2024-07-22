#include <WiFi.h>
#include <ESP32Firebase.h>

#define SSID "DESKTOP-ADE-FIRMAN"
#define PASSWORD "00111100"
#define REFERENCE_URL "https://adefirebase-b54f6-default-rtdb.firebaseio.com/"

Firebase firebase(REFERENCE_URL);

const byte trigPin1 = 19;
const byte echoPin1 = 18;
const byte trigPin2 = 23;
const byte echoPin2 = 22;
const byte raindropPin = 27;

int water1, water2, ground;
int data[50];
int maxx, meann, minn;

unsigned long preTime = millis();

int water1Detect()
{
  const int height1 = 0; //height reference

  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPin1, LOW);

  int duration = pulseIn(echoPin1, HIGH);
  int distance = (duration / 2) * 0.0344;

  int waterLevel = height1 + distance; // -
  // Serial.print("Water Level 1: ");
  // Serial.println(waterLevel);

  return waterLevel;
}

int water2Detect()
{
  const int height2 = 0; //height reference

  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPin2, LOW);

  int duration = pulseIn(echoPin2, HIGH);
  int distance = (duration / 2) * 0.0344;

  int waterLevel = height2 + distance; // -
  // Serial.print("Water Level 2: ");
  // Serial.println(waterLevel);

  return waterLevel;
}

int groundDetect()
{
  int waterDetect = digitalRead(raindropPin);
  if(waterDetect == 0)
  {
    // Serial.print("Kondisi: Basah, Tinggi Air = ");
    // Serial.println(waterLevel);
  }
  else if(waterDetect == 1)
  {
    // Serial.print("Kondisi: Kering, Tinggi tanah = ");
    // Serial.println(waterLevel);
  }

  return waterDetect;
}

void prints()
{
  Serial.print(" water1 ");
  Serial.print(water1);
  Serial.print(" maxx ");
  Serial.print(maxx);
  Serial.print(" meann ");
  Serial.print(meann);
  Serial.print(" minn ");
  Serial.print(minn);

  Serial.println();
}

void shiftRegister(int input)
{
  data[0] = input;
  for(int i=49; i>0; i--)
  {
    data[i] = data[i-1];
  }
  
  for(int i=0; i<50; i=i+2) {meann = meann + data[i] + data[i+1];}
  meann = meann / 50;
  if(water1 > maxx) {maxx = water1;}
  if(water1 < minn) {minn = water1;}
}

void connecting()
{
  if(WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Connecting to: " + String(SSID) + "\n");
    WiFi.begin(SSID, PASSWORD);
    while(WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print("-");
    }
    Serial.println("WiFi Connected");
  }
}

void setup()
{
  Serial.begin(9600);

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(raindropPin, INPUT);
}

void loop()
{
  // connecting();

  if((millis() - preTime) > 100)
  {
    water1 = water1Detect();
    water2 = water2Detect();
    ground = groundDetect();

    // shiftRegister(water1);

    firebase.setInt("Smart-Break-Water/Water1", water1);
    firebase.setInt("Smart-Break-Water/Water2", water2);
    firebase.setInt("Smart-Break-Water/Ground", ground);

    // prints();

    preTime = millis();
  }
}
