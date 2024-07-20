#define trigPin1 27
#define echoPin1 35
#define trigPin2 32
#define echoPin2 33
#define waterPin 25

void setup() {
  Serial.begin(9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(waterPin, INPUT);
}

void loop() {
  detectWater();
  detectWaterGround();
}

void detectWater() {
  long duration;
  int distance;
  const int maxDistance = 100;

  // Mengirim pulsa ultrasonik
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);

  // Mengukur waktu pantulan pulsa
  duration = pulseIn(echoPin1, HIGH);
  distance = round(duration * 0.034 / 2);

  //Menghitung level air
  int waterLevel = maxDistance - distance;
  Serial.print("Water Level 1: ");
  Serial.println(waterLevel);
  
  delay(1000); // Menunggu 1 detik sebelum pembacaan berikutnya
}

void detectWaterGround() {
  long duration;
  int distance;
  const int maxDistance = 100;

  // Mengirim pulsa ultrasonik
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);

  // Mengukur waktu pantulan pulsa
  duration = pulseIn(echoPin2, HIGH);
  distance = round(duration * 0.034 / 2);
  int waterLevel = maxDistance - distance;
  Serial.print("Water Level 2: ");
  Serial.println(waterLevel);

  // Membaca kondisi sensor hujan
  int waterDetect = digitalRead(waterPin); // Membaca kondisi sensor hujan

  if (waterDetect == 0 ) {
    Serial.print("Kondisi: Basah, Tinggi Air = ");
    Serial.println(waterLevel);
  } else if (waterDetect == 1) {
    Serial.print("Kondisi: Kering, Tinggi tanah = ");
    Serial.println(waterLevel);
  }

  delay(1000); // Menunggu 1 detik sebelum pembacaan berikutnya
}
