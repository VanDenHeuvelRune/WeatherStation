/** +--------------------------------------------------+
 *  |     DM_ThingSpeak - Connector for ThingSpeak     |
 *  |--------------------------------------------------|
 *  |   Coded by DataMind (aka. Rune Van den Heuvel)   |
 *  +--------------------------------------------------+
 */

// IMPORT THE NECESSARY LIBRARIES
#include "Arduino.h"       // Include the Arduino library
#include "DM_ThingSpeak.h" // Include the header file where the declarations for this library are stored
#include <WiFi.h>          // Include the "WiFi" library to communicate with the Wi-Fi chip on the ESP32
#include <PubSubClient.h>  // Used to create an object based on the class defined in this library
#include <DM_Utils.h>      // Include the self-made library that contains the rounding function
using namespace std;       // Used to be able to use the string type without needing to say "std::string" every time

// INITIALIZE THE CLASS MEMBERS (if we don't do this, the code will give errors saying these should be initialised first)
unsigned long DM_ThingSpeak::_channelNumber; // The ID of the channel on ThingSpeak to send messages to
string DM_ThingSpeak::_MQTTClientID;         // The client ID for the MQTT connection
string DM_ThingSpeak::_MQTTUsername;         // The username for the MQTT connection
string DM_ThingSpeak::_MQTTPassword;         // The password for the MQTT connection

// OTHER VARIABLES
WiFiClient WiFiClientForMQTT;               // Construct a Wi-Fi client
PubSubClient MQTTClient(WiFiClientForMQTT); // Construct a MQTT client

/**
 * Publish the values to ThingSpeak.
 *
 * @param temperatureC The temperature in degrees Celsius.
 * @param lightIntensityLux The light intensity in lux.
 * @param airPressureBar The air pressure in bar.
 */
void DM_ThingSpeak::publishInformation(float temperatureC, float lightIntensityLux, float airPressureBar)
{
    // Build the variables used as arguments in the publish() function below
    string publishLink = "channels/" + std::to_string(DM_ThingSpeak::_channelNumber) + "/publish";
    string value = "field1=" + std::to_string(DM_Utils::roundTwoDecimals(temperatureC)) + "&field2=" + std::to_string(DM_Utils::roundTwoDecimals(lightIntensityLux)) + "&field3=" + std::to_string(DM_Utils::roundTwoDecimals(airPressureBar));

    // Send the information to ThingSpeak and store the result code
    bool sent = MQTTClient.publish(publishLink.c_str(), value.c_str());

    // Inform the user based on the result
    if (sent)
    {
        Serial.println("[DM_ThingSpeak] Information successfully sent to ThingSpeak!");
    }
    else
    {
        Serial.println("[DM_ThingSpeak] Something went wrong while sending the information to ThingSpeak.");
    }
}

/**
 * Configure the ThingSpeak channel number, MQTT client ID, MQTT username, and MQTT password.
 *
 * @param channelNumber The channel number of ThingSpeak you want to publish to.
 * @param MQTTClientID The client ID for the MQTT connection to ThingSpeak.
 * @param MQTTUsername The username for the MQTT connection to ThingSpeak.
 * @param MQTTPassword The password for the MQTT connection to ThingSpeak.
 */
void DM_ThingSpeak::setConnectionParameters(unsigned long channelNumber, string MQTTClientID, string MQTTUsername, string MQTTPassword)
{
    // Update the class members
    DM_ThingSpeak::_channelNumber = channelNumber;
    DM_ThingSpeak::_MQTTClientID = MQTTClientID;
    DM_ThingSpeak::_MQTTUsername = MQTTUsername;
    DM_ThingSpeak::_MQTTPassword = MQTTPassword;
}

/**
 * Connect to the MQTT server of ThingSpeak and return the success rate.
 *
 * @return The success rate of the connection.
 */
bool DM_ThingSpeak::connectToMQTT()
{
    // Inform the user
    Serial.println("\n[DM_ThingSpeak] Configuring MQTT connection parameters...");

    // Set the server used for the MQTT connection
    MQTTClient.setServer("mqtt3.thingspeak.com", 1883);

    // Inform the user
    Serial.println("\n[DM_ThingSpeak] Connecting to the MQTT server...");

    // Initialize the fail counter
    int amountOfFails = 0;

    // Keep looping until we are connected to the MQTT server or if the fail counter is higher than 11
    while (!MQTTClient.connected() && amountOfFails < 12)
    {
        // Connect and check if the connection was established successfully
        if (MQTTClient.connect(DM_ThingSpeak::_MQTTClientID.c_str(), DM_ThingSpeak::_MQTTUsername.c_str(), DM_ThingSpeak::_MQTTPassword.c_str()))
        {
            Serial.println("\n[DM_ThingSpeak] Successfully connected to the MQTT broker!");
        }
        else
        {
            Serial.print("\n[DM_ThingSpeak] Something went wrong while connecting to the MQTT broker. Retrying in 5 seconds...");
        }

        // Increase the fail counter
        amountOfFails += 1;

        // Wait 5 seconds
        delay(5000);
    }

    // Check if the MQTT connection failed
    if (amountOfFails > 11)
    {
        // Inform the user that the connection failed
        Serial.println("\n[DM_ThingSpeak] WARNING: Connecting to the MQTT broker failed (tried for one minute). Everything that depends on a the MQTT connection will be disabled.");

        // Give the user a little time to read the warning
        delay(5000);

        // Return the success rate
        return false;
    }

    // Return the success rate (if we get to this point, we are successfully connected)
    return true;
}

/**
 * If we are not connected to the MQTT server, connect to it and return the result of the reconnection attempt.
 *
 * @return If we are (now) connected to a Wi-Fi network.
 */
bool DM_ThingSpeak::checkAndReconnectToMQTT()
{
    // Return true if we are connected, and of not connect to the WiFi and return that result
    if (!MQTTClient.connected())
    {
        // Inform the user that the connection has been lost
        Serial.println("\n[DM_ThingSpeak] The connection to the MQTT server has been disconnected. Trying to reconnect...");

        // (Re)connect
        return DM_ThingSpeak::connectToMQTT();
    }

    // If we get here, we are connected so return true
    return true;
}