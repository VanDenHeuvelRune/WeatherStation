/** +----------------------------------------------+
 *  |               WEATHER STATION                |
 *  |----------------------------------------------|
 *  | Coded by DataMind (aka. Rune Van den Heuvel) |
 *  +----------------------------------------------+
 */

// IMPORT THE NECESSARY LIBRARIES
#include <Wire.h>            // Used for initializing the I2C bus
#include <BH1750.h>          // Used to create an object based on the class defined in this library
#include <Adafruit_BMP280.h> // Used to create an object based on the class defined in this library
#include <DM_Utils.h>        // Used to access the round function inside this library
#include <DM_Measurer.h>     // Used for the measurements
#include <DM_WiFi.h>         // Used for all Wi-Fi related functionalities
#include <DM_ThingSpeak.h>   // Used for all ThingSpeak and MQTT related functionalities
#include <DM_Discord.h>      // Used for the Discord integration
using namespace std;         // Used to be able to use the string type without needing to say "std::string" every time

// VARIABLES
bool successfullSetup;                             // This will decide if we start our loop() code or not
bool initialWiFiSuccessfullyConnected;             // We will update this according to the success of establishing the network connection for the first time
bool wifiSuccessfullyConnected;                    // We will update this according to the success of establishing the network connection
bool mqttSuccessfullyConnected;                    // We will update this according to the success of establishing a connection to the MQTT server
bool initialMQTTSuccessfullyConnected;             // We will update this according to the success of establishing a connection to the MQTT server for the first time
string WiFiSSID = "xxxxxxxxxxxxxxxxxxxx";          // The Wi-Fi SSID
string WiFiPassword = "xxxxxxxxxxxxxxxxxxxx";      // The Wi-Fi password
string MQTTClientID = "xxxxxxxxxxxxxxxxxxxx";      // The client ID for MQTT
string MQTTUsername = "xxxxxxxxxxxxxxxxxxxx";      // The username for MQTT
string MQTTPassword = "xxxxxxxxxxxxxxxxxxxx";      // The password for MQTT
unsigned long ThingSpeakChannel = 1973314;         // The ThingSpeak channel number
string DiscordWebhookURL = "xxxxxxxxxxxxxxxxxxxx"; // The Discord webhook ID

BH1750 lightSensor;                          // This will be our BH1750 sensor "object"
Adafruit_BMP280 temperaturePressureChip;     // This will be our BPM280 chip "object"
DM_Measurer measurer;                        // This will be our measure "object"
DM_ThingSpeak ThingSpeakClient;              // This will be our ThingSpeak "client"
DM_WebhookConnector DiscordWebhookConnector; // This will be our Discord webhook connector

// SETUP (EXECUTES ONE TIME, WHEN THE DEVICE BOOTS)
void setup()
{
  // Set the speed of data transfer to 9.600 bits per second
  Serial.begin(9600);

  // Print a boot message
  Serial.println("\n+----------------------------------------------+\n|               WEATHER STATION                |\n|----------------------------------------------|\n| Coded by DataMind (aka. Rune Van den Heuvel) |\n+----------------------------------------------+");

  // Initialize the I2C bus as a master (we say "as a master" because we don't give an address as parameter)
  Wire.begin();

  // Start the Wi-Fi connector and store the success rate of the very first wifi connection attempt
  initialWiFiSuccessfullyConnected = DM_WiFi::connectToWiFi(WiFiSSID, WiFiPassword);

  // Set the MQTT connection parameters;
  ThingSpeakClient.setConnectionParameters(ThingSpeakChannel, MQTTClientID, MQTTUsername, MQTTPassword);

  // Connect to MQTT for the first time, but only if the first Wi-Fi connection succeeded (based on the 'initialWiFiSuccessfullyConnected' variable)
  initialMQTTSuccessfullyConnected = initialWiFiSuccessfullyConnected && ThingSpeakClient.connectToMQTT();

  // Initialize the BH1750 sensor and the BMP280 as a measure device and save the success rate in a variable
  successfullSetup = measurer.initializeBH1750(lightSensor) && measurer.initializeBMP280(temperaturePressureChip);
}

// LOOP (EXECUTES UNTILL DEVICE LOSES POWER)
void loop()
{
  // Don't start this code if the setup of the BMP280 or BH1750 wasn't a success
  if (!successfullSetup)
    return;

  // Reconnect to the Wi-Fi network if the connection has been lost and store the result of the connection attempt in a variable because this will determine if we execute Wi-Fi related functions or not (only do this when the very first Wi-Fi connection was a success)
  wifiSuccessfullyConnected = initialWiFiSuccessfullyConnected && DM_WiFi::checkAndReconnect(WiFiSSID, WiFiPassword);

  // Reconnect to the MQTT server if the connection has been lost and store the result of the connection attempt in a variable because this will determine if we execute MQTT related functions (only do this when the very first Wi-Fi and MQTT connection were a success)
  mqttSuccessfullyConnected = wifiSuccessfullyConnected && initialMQTTSuccessfullyConnected && DM_ThingSpeak::checkAndReconnectToMQTT();

  // Read the measurements and store them in variables
  float lightLevel = DM_Measurer::BH1750readLightLevelLux(lightSensor);
  float temperature = measurer.BMP280readTemperatureC();
  float pressurePa = measurer.BMP280readPressurePa();
  float pressureBar = measurer.BMP280readPressureBar();

  // Make room for (new) measurements to display
  Serial.println("\n--- New measurement --------------------------");

  // Show the light intensity
  Serial.print("Current light intensity: "); // Print the light intensity value (first part)
  Serial.print(lightLevel);                  // Print the light intensity value (second part)
  Serial.println(" lux");                    // Print the light intensity value (third part)

  // Show the temperature, pressure and altitude
  Serial.print("Current temperature: "); // Print the temperature value (first part)
  Serial.print(temperature);             // Print the temperature value (second part)
  Serial.println(" °C");                 // Print the temperature value (third part)
  Serial.print("Current pressure: ");    // Print the pressure value (first part)
  Serial.print(pressurePa);              // Print the pressure value in Pa (second part)
  Serial.print(" Pa (");                 // Print the pressure value (third part)
  Serial.print(pressureBar);             // Print the pressure value in bar (fourth part)
  Serial.println(" bar)\n");             // Print the pressure value (fifth part)

  // Publish the results to ThingSpeak, only if we are successfully connected with the MQTT server
  if (mqttSuccessfullyConnected)
  {
    ThingSpeakClient.publishInformation(temperature, lightLevel, pressurePa);
  }

  // Send the results to a Discord webhook, only if we are succesfully connected to the Wi-Fi network
  if (wifiSuccessfullyConnected)
    DiscordWebhookConnector.sendMessage(DiscordWebhookURL, DiscordWebhookConnector.embedBuilder(temperature, lightLevel, pressurePa));

  // Wait fifteen seconds before reading new data
  delay(15000);
}