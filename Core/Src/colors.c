#include "colors.h"
#include <limits.h>

Color colors[] = {
    {0xF0F8FF, "Aliceblue"},
    {0xFAEBD7, "Antiquewhite"},
    {0x00FFFF, "Aqua"},
    {0x7FFFD4, "Aquamarine"},
    {0xF0FFFF, "Azure"},
    {0xF5F5DC, "Beige"},
    {0xFFE4C4, "Bisque"},
    {0x000000, "Black"},
    {0xFFEBCD, "Blanchedalmond"},
    {0x0000FF, "Blue"},
    {0x8A2BE2, "Blueviolet"},
    {0xA52A2A, "Brown"},
    {0xDEB887, "Burlywood"},
    {0x5F9EA0, "Cadetblue"},
    {0x7FFF00, "Chartreuse"},
    {0xD2691E, "Chocolate"},
    {0xFF7F50, "Coral"},
    {0x6495ED, "Cornflowerblue"},
    {0xFFF8DC, "Cornsilk"},
    {0xDC143C, "Crimson"},
    {0x00FFFF, "Cyan"},
    {0x00008B, "Darkblue"},
    {0x008B8B, "Darkcyan"},
    {0xB8860B, "Darkgoldenrod"},
    {0xA9A9A9, "Darkgray"},
    {0x006400, "Darkgreen"},
    {0xA9A9A9, "Darkgrey"},
    {0xBDB76B, "Darkkhaki"},
    {0x8B008B, "Darkmagenta"},
    {0x556B2F, "Darkolivegreen"},
    {0xFF8C00, "Darkorange"},
    {0x9932CC, "Darkorchid"},
    {0x8B0000, "Darkred"},
    {0xE9967A, "Darksalmon"},
    {0x8FBC8F, "Darkseagreen"},
    {0x483D8B, "Darkslateblue"},
    {0x2F4F4F, "Darkslategray"},
    {0x2F4F4F, "Darkslategrey"},
    {0x00CED1, "Darkturquoise"},
    {0x9400D3, "Darkviolet"},
    {0xFF1493, "Deeppink"},
    {0x00BFFF, "Deepskyblue"},
    {0x696969, "Dimgray"},
    {0x696969, "Dimgrey"},
    {0x1E90FF, "Dodgerblue"},
    {0xB22222, "Firebrick"},
    {0xFFFAF0, "Floralwhite"},
    {0x228B22, "Forestgreen"},
    {0xFF00FF, "Fuchsia"},
    {0xDCDCDC, "Gainsboro"},
    {0xF8F8FF, "Ghostwhite"},
    {0xDAA520, "Goldenrod"},
    {0xFFD700, "Gold"},
    {0x808080, "Gray"},
    {0x008000, "Green"},
    {0xADFF2F, "Greenyellow"},
    {0x808080, "Grey"},
    {0xF0FFF0, "Honeydew"},
    {0xFF69B4, "Hotpink"},
    {0xCD5C5C, "Indianred"},
    {0x4B0082, "Indigo"},
    {0xFFFFF0, "Ivory"},
    {0xF0E68C, "Khaki"},
    {0xFFF0F5, "Lavenderblush"},
    {0xE6E6FA, "Lavender"},
    {0x7CFC00, "Lawngreen"},
    {0xFFFACD, "Lemonchiffon"},
    {0xADD8E6, "Lightblue"},
    {0xF08080, "Lightcoral"},
    {0xE0FFFF, "Lightcyan"},
    {0xFAFAD2, "Lightgoldenrodyellow"},
    {0xD3D3D3, "Lightgray"},
    {0x90EE90, "Lightgreen"},
    {0xD3D3D3, "Lightgrey"},
    {0xFFB6C1, "Lightpink"},
    {0xFFA07A, "Lightsalmon"},
    {0x20B2AA, "Lightseagreen"},
    {0x87CEFA, "Lightskyblue"},
    {0x778899, "Lightslategray"},
    {0x778899, "Lightslategrey"},
    {0xB0C4DE, "Lightsteelblue"},
    {0xFFFFE0, "Lightyellow"},
    {0x00FF00, "Lime"},
    {0x32CD32, "Limegreen"},
    {0xFAF0E6, "Linen"},
    {0xFF00FF, "Magenta"},
    {0x800000, "Maroon"},
    {0x66CDAA, "Mediumaquamarine"},
    {0x0000CD, "Mediumblue"},
    {0xBA55D3, "Mediumorchid"},
    {0x9370DB, "Mediumpurple"},
    {0x3CB371, "Mediumseagreen"},
    {0x7B68EE, "Mediumslateblue"},
    {0x00FA9A, "Mediumspringgreen"},
    {0x48D1CC, "Mediumturquoise"},
    {0xC71585, "Mediumvioletred"},
    {0x191970, "Midnightblue"},
    {0xF5FFFA, "Mintcream"},
    {0xFFE4E1, "Mistyrose"},
    {0xFFE4B5, "Moccasin"},
    {0xFFDEAD, "Navajowhite"},
    {0x000080, "Navy"},
    {0xFDF5E6, "Oldlace"},
    {0x808000, "Olive"},
    {0x6B8E23, "Olivedrab"},
    {0xFFA500, "Orange"},
    {0xFF4500, "Orangered"},
    {0xDA70D6, "Orchid"},
    {0xEEE8AA, "Palegoldenrod"},
    {0x98FB98, "Palegreen"},
    {0xAFEEEE, "Paleturquoise"},
    {0xDB7093, "Palevioletred"},
    {0xFFEFD5, "Papayawhip"},
    {0xFFDAB9, "Peachpuff"},
    {0xCD853F, "Peru"},
    {0xFFC0CB, "Pink"},
    {0xDDA0DD, "Plum"},
    {0xB0E0E6, "Powderblue"},
    {0x800080, "Purple"},
    {0x663399, "Rebeccapurple"},
    {0xFF0000, "Red"},
    {0xBC8F8F, "Rosybrown"},
    {0x4169E1, "Royalblue"},
    {0x8B4513, "Saddlebrown"},
    {0xFA8072, "Salmon"},
    {0xF4A460, "Sandybrown"},
    {0x2E8B57, "Seagreen"},
    {0xFFF5EE, "Seashell"},
    {0xA0522D, "Sienna"},
    {0xC0C0C0, "Silver"},
    {0x87CEEB, "Skyblue"},
    {0x6A5ACD, "Slateblue"},
    {0x708090, "Slategray"},
    {0x708090, "Slategrey"},
    {0xFFFAFA, "Snow"},
    {0x00FF7F, "Springgreen"},
    {0x4682B4, "Steelblue"},
    {0xD2B48C, "Tan"},
    {0x008080, "Teal"},
    {0xD8BFD8, "Thistle"},
    {0xFF6347, "Tomato"},
    {0x40E0D0, "Turquoise"},
    {0xEE82EE, "Violet"},
    {0xF5DEB3, "Wheat"},
    {0xFFFFFF, "White"},
    {0xF5F5F5, "Whitesmoke"},
    {0xFFFF00, "Yellow"},
    {0x9ACD32, "Yellowgreen"}
};

/**
 * @brief Converts a 16-bit RGB565 color to 24-bit RGB888 color.
 *
 * This function takes a 16-bit RGB565 color value and converts it to its equivalent 24-bit RGB888 color representation.
 * The resulting color components are stored in separate variables pointed to by the `r`, `g`, and `b` parameters.
 *
 * @param color565 The 16-bit RGB565 color value to convert.
 * @param r Pointer to a variable where the resulting red component (8-bit) will be stored.
 * @param g Pointer to a variable where the resulting green component (8-bit) will be stored.
 * @param b Pointer to a variable where the resulting blue component (8-bit) will be stored.
 */
void RGB565ToRGB888(uint16_t color565, uint8_t *r, uint8_t *g, uint8_t *b) {
    *r = ((color565 >> 11) & 0x1F) * 255 / 31;
    *g = ((color565 >> 5) & 0x3F) * 255 / 63;
    *b = (color565 & 0x1F) * 255 / 31;
}

/**
 * @brief Converts a hexadecimal color value to RGB888 format.
 *
 * This function takes a 32-bit hexadecimal color value and converts it to RGB888 format.
 * The resulting RGB values are stored in the provided pointers.
 *
 * @param hex The hexadecimal color value to convert.
 * @param r   Pointer to store the red component of the RGB value.
 * @param g   Pointer to store the green component of the RGB value.
 * @param b   Pointer to store the blue component of the RGB value.
 */
void HexToRGB888(uint32_t hex, uint8_t *r, uint8_t *g, uint8_t *b) {
    *r = (hex >> 16) & 0xFF;
    *g = (hex >> 8) & 0xFF;
    *b = hex & 0xFF;
}


/**
 * @brief Finds the closest color name to a given generated color.
 *
 * This function takes a generated color in RGB565 format and finds the closest color name from a predefined list of colors.
 * The generated color is converted to RGB888 format using the RGB565ToRGB888 function.
 * Then, the function calculates the distance between the generated color and each color in the list using the Euclidean distance formula.
 * The color with the minimum distance is considered the closest color.
 *
 * @param generatedColor The generated color in RGB565 format.
 * @return The closest color name as a string.
 */
const char* FindClosestColor(uint16_t generatedColor) {
    uint8_t r1, g1, b1;
    RGB565ToRGB888(generatedColor, &r1, &g1, &b1);

    int minDistance = INT_MAX;
    const char* closestColorName = "Unknown";

    for (int i = 0; i < sizeof(colors) / sizeof(colors[0]); i++) {
        uint8_t r2, g2, b2;
        HexToRGB888(colors[i].hex, &r2, &g2, &b2);

        int distance = (r1 - r2) * (r1 - r2) + (g1 - g2) * (g1 - g2) + (b1 - b2) * (b1 - b2);

        if (distance < minDistance) {
            minDistance = distance;
            closestColorName = colors[i].name;
        }
    }

    return closestColorName;
}
