/** +----------------------------------------------+
 *  |             DM_Utils - Utilities             |
 *  |----------------------------------------------|
 *  | Coded by DataMind (aka. Rune Van den Heuvel) |
 *  +----------------------------------------------+
 */

// If this library is already included in a file, make sure to not include it again → we set the ifndef header guard
#ifndef DM_Utils_h
#define DM_Utils_h

// DECLARE THE CLASS "DM_Utils"
class DM_Utils
{
public: // The public function
    static float roundTwoDecimals(float decimal);
};

#endif // End the header guard