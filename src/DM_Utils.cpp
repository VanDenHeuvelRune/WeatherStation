/** +----------------------------------------------+
 *  |             DM_Utils - Utilities             |
 *  |----------------------------------------------|
 *  | Coded by DataMind (aka. Rune Van den Heuvel) |
 *  +----------------------------------------------+
 */

// IMPORT THE NECESSARY LIBRARIES
#include "Arduino.h"  // Include the Arduino library
#include "DM_Utils.h" // Include the header file where the declarations for this library are stored

/**
 * Round the given parameter by multiplying the number by 100, taking the whole part and then dividing by 100.
 *
 * @param decimal The decimal number to round
 *
 * @return The rounded value of the given parameter.
 */
float DM_Utils::roundTwoDecimals(float decimal)
{
    return floor(decimal * 100.0) / 100.0;
}
