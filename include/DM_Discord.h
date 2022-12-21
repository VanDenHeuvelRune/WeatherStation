/** +----------------------------------------------+
 *  |       DM_Discord - Connect to Discord        |
 *  |----------------------------------------------|
 *  | Coded by DataMind (aka. Rune Van den Heuvel) |
 *  +----------------------------------------------+
 */

// If this library is already included in a file, make sure to not include it again → we set the ifndef header guard
#ifndef DM_Discord_h
#define DM_Discord_h

// IMPORT THE NECESSARY LIBRARY
using namespace std; // Used to be able to use the string type without needing to say "std::string" every time

// DECLARE THE CLASS "DM_WebhookConnector"
class DM_WebhookConnector
{
public: // The public functions
    static void sendMessage(string webhookURL, string message);
    static string embedBuilder(float temperatureC, float lightIntensityLux, float airPressureBar);
};

#endif // End the header guard