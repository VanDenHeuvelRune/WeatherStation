/** +--------------------------------------------------+
 *  |     DM_ThingSpeak - Connector for ThingSpeak     |
 *  |--------------------------------------------------|
 *  |   Coded by DataMind (aka. Rune Van den Heuvel)   |
 *  +--------------------------------------------------+
 */

// If this library is already included in a file, make sure to not include it again → we set the ifndef header guard
#ifndef DM_ThingSpeak_h
#define DM_ThingSpeak_h

// IMPORT THE NECESSARY LIBRARY
using namespace std; // Used to be able to use the string type without needing to say "std::string" every time

// DECLARE THE CLASS "DM_ThingSpeak"
class DM_ThingSpeak
{
private: // The public functions
    static unsigned long _channelNumber;
    static string _MQTTClientID;
    static string _MQTTUsername;
    static string _MQTTPassword;

public: // The private functions
    static void setConnectionParameters(unsigned long channelNumber, string MQTTClientID, string MQTTUsername, string MQTTPassword);
    static void publishInformation(float temperatureC, float lightIntensityLux, float airPressureBar);
    static bool connectToMQTT();
    static bool checkAndReconnectToMQTT();
};

#endif // End the header guard