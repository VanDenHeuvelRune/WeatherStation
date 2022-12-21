/** +----------------------------------------------+
 *  |       DM_Discord - Connect to Discord        |
 *  |----------------------------------------------|
 *  | Coded by DataMind (aka. Rune Van den Heuvel) |
 *  +----------------------------------------------+
 */

// IMPORT THE NECESSARY LIBRARIES
#include "Arduino.h"    // Include the Arduino library
#include "DM_Discord.h" // Include the header file where the declarations for this library are stored
#include <HTTPClient.h> // Used to create an object based on the class defined in this library
using namespace std;    // Used to be able to use the string type without needing to say "std::string" every time

/**
 * Send a message to the specified Discord webhook.
 *
 * @param webhookURL The URL of the Discord webhook.
 * @param message The message you want to send to the Discord webhook.
 */
void DM_WebhookConnector::sendMessage(string webhookURL, string message)
{
    // Construct a HTTP client
    HTTPClient HTTPClientForDiscord;

    // Specify the Discord webhook URL
    HTTPClientForDiscord.begin(String(webhookURL.c_str()));

    // Add the header to the POST request
    HTTPClientForDiscord.addHeader("Content-Type", "application/json");

    // Send the HTTP POST request
    int responseCode = HTTPClientForDiscord.POST(String(message.c_str()));

    // Inform the user based on the result
    if (responseCode == 200 || responseCode == 201 || responseCode == 204) // 200 = successful; 201 = the creation of something was succesful; 204 = successful but no content returned
    {
        Serial.println("\n[DM_Discord] Information successfully sent the Discord webhook!");
    }
    else
    {
        Serial.println("\n[DM_Discord] Something went wrong while sending the information to the Discord webhook.");
    }
}

/**
 * Build the JSON for the embed to send via a POST request.
 *
 * @param temperatureC The temperature in Celsius.
 * @param lightIntensityLux The light intensity in lux.
 * @param airPressureBar The air pressure in bar.
 *
 * @return The JSON string.
 */
string DM_WebhookConnector::embedBuilder(float temperatureC, float lightIntensityLux, float airPressureBar)
{
    return "{\"content\": null, \"embeds\": [{\"description\": \"**NEW MEASUREMENT**\", \"color\": 4176032, \"fields\": [{\"name\": \"Temperature\", \"value\": \"`" + std::to_string(temperatureC) + " °C`\", \"inline\": true}, {\"name\": \"Light intensity\", \"value\": \"`" + std::to_string(lightIntensityLux) + " lux`\", \"inline\": true}, {\"name\": \"Air pressure\", \"value\": \"`" + std::to_string(airPressureBar) + " Pa`\", \"inline\": true}]}], \"attachments\": []}";
}