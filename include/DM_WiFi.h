/** +----------------------------------------------+
 *  |          DM_WiFi - Wi-Fi Connector           |
 *  |----------------------------------------------|
 *  | Coded by DataMind (aka. Rune Van den Heuvel) |
 *  +----------------------------------------------+
 */

// If this library is already included in a file, make sure to not include it again → we set the ifndef header guard
#ifndef DM_WiFi_h
#define DM_WiFi_h

// IMPORT THE NECESSARY LIBRARIES
#include <WiFi.h>    // Used to be able to use "wl_status_t" as a return type of a function
using namespace std; // Used to be able to use the string type without needing to say "std::string" every time

// DECLARE THE CLASS "DM_WiFi"
class DM_WiFi
{
public: // The public functions
    static bool connectToWiFi(string SSID, string password);
    static bool checkAndReconnect(string SSID, string password);

private: // The private functions
    static wl_status_t _isConnected();
    static int _getAmountOfAvailableWiFiNetworks();
    static void _printAvailableWiFiNetworks();
};

#endif // End the header guard