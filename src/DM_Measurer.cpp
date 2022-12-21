/** +----------------------------------------------+
 *  |        DM_Measurer - Read measurements       |
 *  |----------------------------------------------|
 *  | Coded by DataMind (aka. Rune Van den Heuvel) |
 *  +----------------------------------------------+
 */

// IMPORT THE NECESSARY LIBRARIES
#include "Arduino.h"         // Include the Arduino library
#include "DM_Measurer.h"     // Include the header file where the declarations for this library are stored
#include <BH1750.h>          // Used to create an object based on the class defined in this library
#include <Adafruit_BMP280.h> // Used to create an object based on the class defined in this library
#include <DM_Utils.h>        // Include the self-made library that contains the rounding function

// INITIALIZE THE CLASS MEMBERS (if we don't do this, the code will give errors saying these should be initialised first)
Adafruit_BMP280 DM_Measurer::_BMP280Chip; // The BMP280 chip

/**
 * Initialize the BH1750 light sensor.
 *
 * @param measurementChip The BH1750 chip that is needs to be initialized (a reference to the already created object).
 *
 * @return The success rate of the initialization.
 */
bool DM_Measurer::initializeBH1750(BH1750 &measurementChip)
{
    // Initialize the light sensor in high resulution mode (this is default, so no parameters should be given)
    bool success = measurementChip.begin();

    // Return an error message if an error happened
    if (!success)
    {
        Serial.println("\n[DM_Measurer] ERROR: An error occured while initializing the BH1750 sensor.");
    }

    // Return the success rate
    return success;
}

/**
 * Initialize the BMP280 sensor and assign it to the member variable.
 *
 * @param measurementChip The BMP280 sensor object (a reference to the already created object).
 *
 * @return The success rate of the initialization of the BMP280 sensor.
 */
bool DM_Measurer::initializeBMP280(Adafruit_BMP280 &measurementChip)
{
    // Print an empty line
    Serial.println();

    // Initialize the BMP280 sensor on the I2C address bus "76"
    bool success = measurementChip.begin(0x76);

    // Return an error message if an error happened
    if (!success)
    {
        // Print an error message
        Serial.println("[DM_Measurer] ERROR: A valid BMP280 sensor could not be found. Please check the wiring...");
    }
    else
    {
        // Print a success message
        Serial.println("[DM_Measurer] A valid BMP280 sensor was found!");

        // Set what data the BMP280 chip should read
        measurementChip.setSampling(Adafruit_BMP280::MODE_NORMAL,     // We use "normal" for the operation mode because we want the sensor to automatically switch between a measurement and being standby (this is good for filtering distrubances.)
                                    Adafruit_BMP280::SAMPLING_X2,     // We set the sampling rate for temperature measurements to 2, what means that we read each value twice and the average of those two values will be the measurement.
                                    Adafruit_BMP280::SAMPLING_X16,    // We set the sampling rate for pressure measurements to 16, what means that we read each value 16 times and the average of those two values will be the measurement.
                                    Adafruit_BMP280::FILTER_X16,      //
                                    Adafruit_BMP280::STANDBY_MS_500); // Set the standby time to a duration of 500 milliseconds

        // Assign the initialized sensor to the class member
        DM_Measurer::_BMP280Chip = measurementChip;
    }

    // Return the success rate
    return success;
}

/**
 * Convert a decimal value of pressure in Pascals to a decimal value of pressure in bars.
 *
 * @param decimalPa The pressure in Pascal.
 *
 * @return The pressure in bar.
 */
float DM_Measurer::_convertPaToBar(float decimalPa)
{
    return DM_Utils::roundTwoDecimals(decimalPa / 100000.0);
};

/**
 * Read the temperature from the BMP280 sensor and return it as a float.
 *
 * @return The temperature in Celsius.
 */
float DM_Measurer::BMP280readTemperatureC()
{
    return DM_Utils::roundTwoDecimals(DM_Measurer::_BMP280Chip.readTemperature());
}

/**
 * Read the pressure from the BMP280 sensor and return it in Pascal.
 *
 * @return The pressure in Pa.
 */
float DM_Measurer::BMP280readPressurePa()
{
    return DM_Utils::roundTwoDecimals(DM_Measurer::_BMP280Chip.readPressure());
}

/**
 * Read the pressure from the BMP280 sensor and converts it to bar.
 *
 * @return The pressure in bar.
 */
float DM_Measurer::BMP280readPressureBar()
{
    return DM_Measurer::_convertPaToBar(DM_Measurer::_BMP280Chip.readPressure());
}

/**
 * Read the light level from the BH1750 light sensor and return the value in lux.
 *
 * @param measurementChip The BH1750 object that is used to measure the light level.
 *
 * @return The light level in lux.
 */
float DM_Measurer::BH1750readLightLevelLux(BH1750 measurementChip)
{
    return DM_Utils::roundTwoDecimals(measurementChip.readLightLevel());
}