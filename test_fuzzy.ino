// #include <Arduino.h>

// // Fungsi keanggotaan segitiga
// float triangularMF(float x, float a, float b, float c) {
//   if (x <= a || x >= c) return 0;
//   else if (x > a && x <= b) return (x - a) / (b - a);
//   else return (c - x) / (c - b);
// }

// // Fungsi untuk menghitung derajat keanggotaan curah hujan
// void curahHujanMembership(float x, float &basah, float &lembab, float &kering) {
//   basah = triangularMF(x, 0, 200, 400);
//   lembab = triangularMF(x, 200, 400, 600);
//   kering = triangularMF(x, 400, 600, 700);
// }

// // Fungsi untuk menghitung derajat keanggotaan ketinggian air
// void ketinggianAirMembership(float x, float &pendek, float &sedang, float &tinggi) {
//   pendek = triangularMF(x, 0, 10, 30);
//   sedang = triangularMF(x, 10, 30, 50);
//   tinggi = triangularMF(x, 30, 50, 100);
// }

// // Fungsi defuzzifikasi sederhana (menggunakan metode singleton)
// float defuzzify(float rule1, float rule2, float rule3) {
//   float tingkat_bahaya = (rule1 * 100 + rule2 * 50 + rule3 * 0) / (rule1 + rule2 + rule3);
//   return tingkat_bahaya;
// }

// // Fungsi utama untuk menghitung tingkat bahaya
// float hitungTingkatBahaya(float curahHujan, float ketinggianAir) {
//   float basah, lembab, kering;
//   float pendek, sedang, tinggi;
  
//   curahHujanMembership(curahHujan, basah, lembab, kering);
//   ketinggianAirMembership(ketinggianAir, pendek, sedang, tinggi);
  
//   // Aturan fuzzy
//   float rule1 = min(kering, pendek);   // Bahaya tinggi
//   float rule2 = min(lembab, sedang);   // Bahaya sedang
//   float rule3 = min(basah, tinggi);    // Bahaya rendah
  
//   return defuzzify(rule1, rule2, rule3);
// }

// // Pin sensor (sesuaikan dengan konfigurasi hardware Anda)
// const int RAIN_SENSOR_PIN = A0;  // Pin analog untuk sensor curah hujan
// const int TRIG_PIN = D1;         // Pin trigger untuk sensor ultrasonik
// const int ECHO_PIN = D2;         // Pin echo untuk sensor ultrasonik

// void setup() {
//   Serial.begin(115200);
//   pinMode(TRIG_PIN, OUTPUT);
//   pinMode(ECHO_PIN, INPUT);
// }

// void loop() {
//   // Baca sensor curah hujan (asumsi output analog 0-1023)
//   int rainReading = analogRead(RAIN_SENSOR_PIN);
//   float curahHujan = map(rainReading, 0, 1023, 0, 700);  // Map ke range 0-700
  
//   // Baca sensor ultrasonik
//   digitalWrite(TRIG_PIN, LOW);
//   delayMicroseconds(2);
//   digitalWrite(TRIG_PIN, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(TRIG_PIN, LOW);
  
//   long duration = pulseIn(ECHO_PIN, HIGH);
//   float ketinggianAir = duration * 0.034 / 2;  // Konversi ke cm
  
//   // Hitung tingkat bahaya
//   float tingkatBahaya = hitungTingkatBahaya(curahHujan, ketinggianAir);
  
//   // Tampilkan hasil
//   Serial.print("Curah Hujan: ");
//   Serial.print(curahHujan);
//   Serial.print(" | Ketinggian Air: ");
//   Serial.print(ketinggianAir);
//   Serial.print(" cm | Tingkat Bahaya: ");
//   Serial.println(tingkatBahaya);
  
//   delay(1000);  // Delay 1 detik sebelum pembacaan berikutnya
// }