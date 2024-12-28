#include "miniRT.h"


uint32_t	get_color_from_tuple(xyzvektor color)
{
    uint8_t r = min_of(color.x * 255, 255);
    uint8_t g = min_of(color.y * 255, 255);
    uint8_t b = min_of(color.z * 255, 255);
	uint8_t a = 255;

    // Combine into a single 32-bit integer (0xRRGGBB)
    return (a << 24) | (r << 16) | (g << 8) | b;
}



xyzvektor	get_color_from_uint(uint32_t color)
{
	xyzvektor result;
	uint8_t a = (color >> 24) & 0xFF;
	uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;

    result.x = (double)r / 255;
    result.y = (double)g / 255;
    result.z = (double)b / 255;
	result.w = (double)a / 255;

    return result;
}