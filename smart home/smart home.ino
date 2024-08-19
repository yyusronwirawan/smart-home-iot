#include <ESP8266WiFi.h>
#include <Servo.h>
#include <DHT.h>

#define DHTPIN D2          // Pin untuk sensor DHT11
#define DHTTYPE DHT11      // Tipe sensor DHT11

#define TRIG_PIN D1        // Pin Trig sensor ultrasonik
#define ECHO_PIN D3        // Pin Echo sensor ultrasonik
#define LED_PIN D6         // Pin untuk LED
#define RELAY_PIN D7       // Pin untuk relay
#define SERVO_PIN D8       // Pin untuk servo

DHT dht(DHTPIN, DHTTYPE);
Servo servo;
bool isDoorOpen = false;

void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  dht.begin();
  servo.attach(SERVO_PIN);
  servo.write(0); // Pastikan servo dalam posisi tertutup di awal

  digitalWrite(LED_PIN, LOW);
  digitalWrite(RELAY_PIN, LOW);
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Baca jarak dari sensor ultrasonik
  long duration, distance;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2; // Jarak dalam cm

  // Logika buka pintu dengan servo jika ada orang terdeteksi oleh sensor ultrasonik
  if (distance >= 100 && !isDoorOpen) {
    servo.write(180); // Open the door (adjust the angle as needed)
    isDoorOpen = true;
    delay(5000); // Delay for 5 seconds (adjust as needed)
  } else if (distance < 100 && isDoorOpen) {
    servo.write(0); // Close the door
    isDoorOpen = false;
  }

  // Logika untuk menyalakan relay jika suhu lebih dari 37 derajat Celsius
  if (temperature > 37) {
    digitalWrite(RELAY_PIN, HIGH);
  } else {
    digitalWrite(RELAY_PIN, LOW);
  }

  // Logika untuk menyalakan LED jika ada objek dalam jarak 5 cm dari sensor ultrasonik
  if (distance <= 5) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  // Debugging output
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" C, Humidity: ");
  Serial.print(humidity);
  Serial.print(" %, Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(2000); // Tunda selama 2 detik
}
