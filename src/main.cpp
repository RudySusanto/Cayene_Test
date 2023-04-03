#include <Arduino.h>
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP32.h>
#include <DHTesp.h>

// WiFi network info.
char ssid[] = "R_X3P";
char wifiPassword[] = "@RudySusanto";

#define LED_GREEN 14
#define LED_YELLOW 12
#define LED_RED 13
#define PIN_DHT 19
DHTesp dht;


// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "a5b95d80-d1e8-11ed-b72d-d9f6595c5b9d";
char password[] = "2092732b7308ebad1abd2f9712cc072c727d0116";
char clientID[] = "4c379690-d1e9-11ed-b72d-d9f6595c5b9d";


void setup() {
	Serial.begin(115200);
	Cayenne.begin(username, password, clientID, ssid, wifiPassword);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  dht.setup(PIN_DHT, DHTesp::DHT22);
}

void loop() {
	Cayenne.loop();
}

// Default function for sending sensor data at intervals to Cayenne.
// You can also use functions for specific channels, e.g CAYENNE_OUT(1) for sending channel 1 data.
CAYENNE_OUT_DEFAULT()
{
	// Write data to Cayenne here. This example just sends the current uptime in milliseconds on virtual channel 0.
	Cayenne.virtualWrite(0, millis());
  Serial.println("Send data");
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  Serial.printf("Temperature: %.2f, humidity: %.2f \n",temperature, humidity);
	// Some examples of other functions you can use to send data.
	Cayenne.celsiusWrite(1, temperature);
  Cayenne.virtualWrite(2, humidity, TYPE_RELATIVE_HUMIDITY, UNIT_PERCENT);
	//Cayenne.luxWrite(2, 700);
	//Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);
}

// Default function for processing actuator commands from the Cayenne Dashboard.
// You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
	CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
  switch (request.channel)
  {
  case 3:
    digitalWrite (LED_RED, getValue.asInt());
    break;
    case 4:
    digitalWrite (LED_GREEN, getValue.asInt());
    break;
  
  default:
    break;
  }
	//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}