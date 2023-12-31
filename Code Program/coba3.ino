#include <Servo.h>

#define BLYNK_TEMPLATE_ID "TMPL6-bsqYK-w"
#define BLYNK_TEMPLATE_NAME "Pakan ikan"
#define BLYNK_AUTH_TOKEN "zsNJP17_gYLzrnjzN36IE3Vz2TApfKMB"
#include <BlynkSimpleEsp8266.h>

Servo myservo;
int StatusPakan;

const int trigPin = 5; // GPIO5
const int echoPin = 4; // GPIO4

const int ledPin1 = 14; // GPIO14
const int ledPin2 = 12; // GPIO12
const int ledPin3 = 13; // GPIO13

const int buzzerPin = 15; // GPIO15

void setup() {
  myservo.write(90);
  Serial.begin(9600);
  myservo.attach(16);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);

  pinMode(buzzerPin, OUTPUT);

  WiFi.begin("K8", "bacotmek");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Wifi Telah Terkoneksi");

  Blynk.begin(BLYNK_AUTH_TOKEN, "K8", "bacotmek");
  Serial.println("Blynk Terkoneksi");
}

void Kasihmakan() {
  for (int posisi = 180; posisi <= 0; posisi++) {
    myservo.write(posisi);
    delay(10);
  }

  for (int posisi = 0; posisi >= 180; posisi--) {
    myservo.write(posisi);
    delay(10);
  }
}

void loop() {
  Blynk.run();

  Serial.println("Status Pakan: " + String(StatusPakan));

  if (StatusPakan == 1) {
    Kasihmakan();
    Blynk.virtualWrite(V0, 0);
    StatusPakan = 0;
    delay(3000);
  }

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;

  Serial.print("Jarak Ultrasonik: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance > 50) {
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);
  } else if (distance > 30) {
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin3, LOW);
  } else if (distance <= 15) {
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, HIGH);
    tone(buzzerPin, 1000);
  } else {
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);
    noTone(buzzerPin);
  }
}

BLYNK_WRITE(V0) {
  StatusPakan = param.asInt();
}