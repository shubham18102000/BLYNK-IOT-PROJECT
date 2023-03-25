#define BLYNK_TEMPLATE_ID "TMPL9tS641ms"
#define BLYNK_TEMPLATE_NAME "SOIL"
#define BLYNK_AUTH_TOKEN "EwZYRc8L6Ri36So1qRZhskhv4rrYKoWw"
#define BLYNK_PRINT Serial
#include <OneWire.h>
#include <SPI.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS D2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
char auth[] ="EwZYRc8L6Ri36So1qRZhskhv4rrYKoWw";
char ssid[] = "iPhone";      // wifi name
char pass[] = "123456789";// wifi password

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

SimpleTimer timer;

int sensor = 0;
int output = 0;
int relayPin = 4; // Relay connected to digital pin D4

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Blynk.virtualWrite(V5, h); //V5 is for Humidity
  Blynk.virtualWrite(V6, t); //V6 is for Temperature
}

void setup()
{
  Serial.begin(9600);
  dht.begin();
  pinMode(relayPin, OUTPUT); // set relay pin as an output pin
  digitalWrite(relayPin, LOW); // Turn off the relay at the beginning

  timer.setInterval(1000L, sendSensor);
  Blynk.begin(auth, ssid, pass);
  sensors.begin();
}

void sendTemps()
{
  sensor = analogRead(A0);
  output = (145 - map(sensor, 0, 1023, 0, 100)); //in place 145 there is 100(it change with the change in sensor)
  delay(1000);
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);
  Serial.println(temp);
  Serial.print("moisture = ");
  Serial.print(output);
  Serial.println("%");
  Blynk.virtualWrite(V1, temp);
  Blynk.virtualWrite(V2, output);

  if (temp >= 25) { // if the temperature is greater than or equal to 30 degrees Celsius, turn on the relay
    digitalWrite(relayPin, HIGH); // Turn on the relay
    Blynk.virtualWrite(V3, 1); // Set V3 widget value to 1 indicating that the relay is on
  } else { // if the temperature is less than 30 degrees Celsius, turn off the relay
    digitalWrite(relayPin, LOW); // Turn off the relay
    Blynk.virtualWrite(V3, 0); // Set V3 widget value to 0 indicating that the relay is off
  }

  delay(1000);
}

void loop()
{
  Blynk.run();
  timer.run();
  sendTemps();
}
// connect soil moisture pin to A0
// connect humidity pin to D5
// connect realy pi to D4