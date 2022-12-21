/** +----------------------------------------------+
 *  |        DM_Measurer - Read measurements       |
 *  |----------------------------------------------|
 *  | Coded by DataMind (aka. Rune Van den Heuvel) |
 *  +----------------------------------------------+
 */

// If this library is already included in a file, make sure to not include it again → we set the ifndef header guard
#ifndef DM_Measurer_h
#define DM_Measurer_h

// IMPORT THE NECESSARY LIBRARIES
#include <Adafruit_BMP280.h> // Used to be able to use "Adafruit_BMP280" as a type for an argument and as a type for a member of the class
#include <BH1750.h>          // Used to be able to use "BH1750" as a type for an argument

// DECLARE THE CLASS "DM_Measurer"
class DM_Measurer
{
public: // The public functions
    static bool initializeBMP280(Adafruit_BMP280 &measurementChip);
    static bool initializeBH1750(BH1750 &measurementChip);
    static float BMP280readTemperatureC();
    static float BMP280readPressurePa();
    static float BMP280readPressureBar();
    static float BH1750readLightLevelLux(BH1750 measurementChip);

private: // The private function and member
    static Adafruit_BMP280 _BMP280Chip;
    static float _convertPaToBar(float numberPa);
};

#endif // End the header guard