/** +----------------------------------------------+
 *  |          DM_WiFi - Wi-Fi Connector           |
 *  |----------------------------------------------|
 *  | Coded by DataMind (aka. Rune Van den Heuvel) |
 *  +----------------------------------------------+
 */

// IMPORT THE NECESSARY LIBRARIES
#include "Arduino.h" // Include the Arduino library
#include "DM_WiFi.h" // Include the header file where the declarations for this library are stored
#include <WiFi.h>    // Include the "WiFi" library to communicate with the Wi-Fi chip on the ESP32
using namespace std; // Used to be able to use the string type without needing to say "std::string" every time

/**
 * Connect to a Wi-Fi network with the given SSID and password (return false when no connection could be established for one minute of trying).
 *
 * @param SSID The name of the Wi-Fi network you want to connect to.
 * @param password The password of the Wi-Fi network you want to connect to.
 *
 * @return The success rate of the connection.
 */
bool DM_WiFi::connectToWiFi(string SSID, string password)
{
    // Set the Wi-Fi mode to 'station' in order to connect to another network
    WiFi.mode(WIFI_STA);

    // Make the connection with the given SSID using the given Wi-Fi password (and convert the parameters to the correct formats)
    WiFi.begin(SSID.c_str(), password.c_str());

    // Print a status message
    Serial.println("\n[DM_WiFi] Connecting to Wi-Fi...\n");

    // Initialize the fail counter
    int amountOfFails = 0;

    // Keep looping until we are connected to the network or if the fail counter is higher than 11
    while (DM_WiFi::_isConnected() != WL_CONNECTED && amountOfFails < 12)
    {
        // Store the amount of available Wi-Fi networks
        int amountOfAvailableWiFiNetworks = _getAmountOfAvailableWiFiNetworks();

        // Check for the amount of available Wi-Fi networks
        if (amountOfAvailableWiFiNetworks == -2)
        {
            // Inform the user that the Wi-Fi network scan failed
            Serial.println("[DM_WiFi] Scanning for Wi-Fi networks failed or not done yet. Retrying in 5 seconds...");
        }
        else if (amountOfAvailableWiFiNetworks < 1)
        {
            // Inform the user no Wi-Fi networks have been found
            Serial.println("[DM_WiFi] No available Wi-Fi networks have been found. Retrying in 5 seconds...");
        }
        else if (DM_WiFi::_isConnected() == WL_CONNECT_FAILED)
        {
            // Inform user that the SSID has been found, but a connection could not be established
            Serial.println("[DM_WiFi] The SSID has been found, but no connection could be established. Please check the credentials (retrying in 5 seconds).");
        }
        else
        {
            // Inform user that the connection could not be established
            Serial.print("[DM_WiFi] The connection could not be established. Make sure the SSID \"");
            Serial.print(SSID.c_str());
            Serial.println("\" is an available Wi-Fi network. Retrying in 5 seconds...");

            // Print every available network
            _printAvailableWiFiNetworks();
        }

        // Increase the fail count
        amountOfFails += 1;

        // Wait five seconds
        delay(5000);
    }

    // Check if the Wi-Fi connection failed
    if (amountOfFails > 11)
    {
        // Inform the user that the connection failed
        Serial.println("\n[DM_WiFi] WARNING: Connecting to Wi-Fi failed (tried for one minute). Everything that depends on a Wi-Fi connection will be disabled.");

        // Give the user a little time to read the warning
        delay(5000);

        // Return the success rate
        return false;
    }

    // Print a status message that indicates success
    Serial.print("\n[DM_WiFi] Succesfully connected on Wi-Fi network \"");
    Serial.print(SSID.c_str());
    Serial.print("\" and received IP address \"");
    Serial.print(WiFi.localIP());
    Serial.println("\".");

    // Return the success rate (if we get to this point, we are successfully connected)
    return true;
}

/**
 * This function returns the amount of available Wi-Fi networks (if this returns -2, it means that no scanning could be done).
 *
 * @return The amount of available Wi-Fi networks.
 */
int DM_WiFi::_getAmountOfAvailableWiFiNetworks()
{
    // Create a loopcount varaible to prevent being stuck waiting for scan results
    int loopCount = 0;

    // Scan the Wi-Fi networks
    int result = WiFi.scanNetworks();

    // Keep looping until the result is higher than -1 or if we looped to much
    while (result < 0 && loopCount < 20)
    {
        // Increment loop count variable and wait 200ms
        loopCount++;
        delay(200);
    }

    // Return the result
    return result;
}

/**
 * Print the SSID of every available Wi-Fi network.
 */
void DM_WiFi::_printAvailableWiFiNetworks()
{
    // Get the number of available Wi-Fi networks
    int amountOfNetworks = _getAmountOfAvailableWiFiNetworks();

    // Print the number of available Wi-Fi networks
    Serial.print("[DM_WiFi] ");
    Serial.print(amountOfNetworks);
    Serial.println(amountOfNetworks > 1 ? " Wi-Fi networks have been found:" : " Wi-Fi network has been found:"); // Adjust the message slightly if the amount is exactly 1

    // Loop through every Wi-Fi network and print its SSID and security type (or show the only one if there is only one SSID)
    if (amountOfNetworks == 1)
    {
        Serial.print("   - ");
        Serial.print(WiFi.SSID(1));
        Serial.print(WiFi.encryptionType(1) == WIFI_AUTH_OPEN ? " (public network)" : "  (private network)");
    }
    else
    {
        for (int i = 0; i < amountOfNetworks; i++)
        {
            Serial.print("   - ");
            Serial.print(WiFi.SSID(i));
            Serial.println(WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? " (public network)" : " (private network)");
        }
    }

    // Print two empty lines
    Serial.println();
    Serial.println();
}

/**
 * Check if the device is connected to a Wi-Fi network.
 *
 * @return The status of the Wi-Fi connection.
 */
wl_status_t DM_WiFi::_isConnected()
{
    return WiFi.status();
}

/**
 * If we are not connected to the Wi-Fi, connect to it and return the result of the reconnection attempt.
 *
 * @param SSID The name of the Wi-Fi network you want to connect to.
 * @param password The password for the Wi-Fi network.
 *
 * @return If we are (now) connected to a Wi-Fi network.
 */
bool DM_WiFi::checkAndReconnect(string SSID, string password)
{
    // Return true if we are connected, and of not connect to the Wi-Fi and return that result
    if (DM_WiFi::_isConnected() != WL_CONNECTED)
    {
        // Inform the user that the connection has been lost
        Serial.println("\n[DM_WiFi] The Wi-Fi has been disconnected. Trying to reconnect...");

        // (Re)connect
        return DM_WiFi::connectToWiFi(SSID, password);
    }

    // If we get here, we are connected so return true
    return true;
}