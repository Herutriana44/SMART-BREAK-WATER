#include <WiFi.h>
#include <ESP32Firebase.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define SSID "DESKTOP-ADE-FIRMAN"
#define PASSWORD "00111100"
#define FIREBASE_URL "https://adefirebase-b54f6-default-rtdb.firebaseio.com/"
#define DATETIME_URL "http://worldtimeapi.org/api/timezone/Asia/Jakarta"
#define trigPin1 19
#define echoPin1 18
#define trigPin2 23
#define echoPin2 22
#define raindropPin 25

int water1 = 0;
int ground1 = 0;
int water2 = 0;
float tingkatBahaya = 0;
String statusBahaya = "";
String datetime = "";

Firebase firebase(FIREBASE_URL);

unsigned long preTime = millis();
unsigned long n = 0;

float AMembershipFunction(float x, float a, float b, float c)
{
  if((x >= a) and (x <= b)) return 1;
  else if((x > b) and (x < c)) return (c - x) / (c - b);
  else return 0;
}

float BMembershipFunction(float x, float a, float b, float c)
{
  if((x > a) and (x <= b)) return (x - a) / (b - a);
  else if((x > b) and (x < c)) return (c - x) / (c - b);
  else return 0;
}

float CMembershipFunction(float x, float a, float b, float c)
{
  if((x > a) and (x < b)) return (x - a) / (b - a);
  else if((x >= b) and (x <= c)) return 1;
  else return 0;
}

void fuzzify(float x, float &low, float &mid, float &high)
{
  low = AMembershipFunction(x, 0, 20, 60);
  mid = BMembershipFunction(x, 20, 60, 140);
  high = CMembershipFunction(x, 60, 140, 300);
}

float inference(float waterLevel1, float waterLevel2)
{
  float low1, mid1, high1;
  float low2, mid2, high2;

  fuzzify(waterLevel1, low1, mid1, high1);
  fuzzify(waterLevel2, low2, mid2, high2);

  float rule1 = min(low1, low2); // weak
  float rule2 = min(low1, mid2); // weak
  float rule3 = min(low1, high2); // normal
  float rule4 = min(mid1, low2); // weak
  float rule5 = min(mid1, mid2); // normal
  float rule6 = min(mid1, high2); // strong
  float rule7 = min(high1, low2); // normal
  float rule8 = min(high1, mid2); // normal
  float rule9 = min(high1, high2); // strong

  return defuzzify(rule1, rule2, rule3, rule4, rule5, rule6, rule7, rule8, rule9);
}

float defuzzify(float rule1, float rule2, float rule3, float rule4, float rule5, float rule6, float rule7, float rule8, float rule9)
{
  int weak = 10;
  int normal = 40;
  int strong = 100;

  float numerator = (rule1 * weak) + (rule2 * weak) + (rule3 * normal) + (rule4 * weak) + (rule5 * normal) + (rule6 * strong) + (rule7 * normal) + (rule8 * normal) + (rule9 * strong);
  float denominator = rule1 + rule2 + rule3 + rule4 + rule5 + rule6 + rule7 + rule8 + rule9;
  float defuzzify = (denominator != 0) ? numerator / denominator : 0;

  return defuzzify;
}

String defuzzifyToString(float defuzzify)
{
  if(defuzzify < 20) return "Rendah";
  else if(defuzzify < 60) return "Sedang";
  else return "Tinggi";
}

String dateTime()
{
  String payload = "";
  
  if(WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    JsonDocument doc;

    http.begin(DATETIME_URL);

    int httpCode = http.GET();
    if(httpCode == HTTP_CODE_OK)
    {
      payload = http.getString();
      // Serial.println(payload);
      deserializeJson(doc, payload);
      payload = String(doc["datetime"]);
    }
    else
    {
      Serial.println("HTTP request failed");
    }

    http.end();
  }

  return payload;
}

int raindropSensor(const byte raindroppin)
{
  int raindropLevel = digitalRead(raindroppin);
  // int raindropLevel = analogRead(raindroppin);

  return raindropLevel;
}

int ultrasonicSensor(const byte trigPin, const byte echoPin)
{
  const int height = 0; // height reference

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPin, LOW);

  int duration = pulseIn(echoPin, HIGH);
  int distance = (duration / 2) * 0.0344;

  int ultrasonicLevel = height + distance; // -

  return ultrasonicLevel;
}

void connectivity()
{
  if(WiFi.status() != WL_CONNECTED)
  {
    Serial.println();
    Serial.println("Connecting to WiFi: " + String(SSID));
    WiFi.begin(SSID, PASSWORD);
    while(WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print("-");
    }
    Serial.println();
    Serial.println("Connected to WiFi: " + String(SSID));
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
  connectivity();

  if((millis() - preTime) >= 1000)
  {
    water2 = ultrasonicSensor(trigPin2, echoPin2);
    water1, ground1;
    (raindropSensor(raindropPin) == 0) ? water1 = ultrasonicSensor(trigPin1, echoPin1) : ground1 = ultrasonicSensor(trigPin1, echoPin1);

    tingkatBahaya = inference(water1, water2);
    statusBahaya = defuzzifyToString(tingkatBahaya);
    datetime = dateTime();

    firebase.setInt("Smart-Break-Water/1-Water1/" + String(n), water1); // Serial.println(millis() / 1000);
    firebase.setInt("Smart-Break-Water/2-Water2/" + String(n), water2); // Serial.println(millis() / 1000);
    firebase.setInt("Smart-Break-Water/3-Ground1/" + String(n), ground1); // Serial.println(millis() / 1000);
    firebase.setFloat("Smart-Break-Water/4-Tingkat-Bahaya/" + String(n), tingkatBahaya); // Serial.println(millis() / 1000);
    firebase.setString("Smart-Break-Water/5-Status-Bahaya/" + String(n), statusBahaya); // Serial.println(millis() / 1000);
    firebase.setString("Smart-Break-Water/6-Date-Time/" + String(n), datetime); // Serial.println(millis() / 1000);

    Serial.print(" | n: ");
    Serial.print(n);
    Serial.print(" | Water1: ");
    Serial.print(water1);
    Serial.print(" | Water2: ");
    Serial.print(water2);
    Serial.print(" | Ground1: ");
    Serial.print(ground1);
    Serial.print(" | Tingkat Bahaya: ");
    Serial.print(tingkatBahaya);
    Serial.print(" | Status Bahaya: ");
    Serial.print(statusBahaya);
    Serial.print(" | Date Time: ");
    Serial.print(datetime);
    Serial.println();

    n = n + 1;
    preTime = millis();
  }
}