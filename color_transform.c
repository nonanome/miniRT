#include "miniRT.h"

//RGB vertauscht x = b

int min_of(int a, int b)
{
	if(a < b)
		return a;
	return b;
}


uint32_t	get_color_from_tuple(t_xyzvektor color)
{
	uint8_t a = 255;
    uint8_t r = min_of(color.x * 255, 255);
    uint8_t g = min_of(color.y * 255, 255);
    uint8_t b = min_of(color.z * 255, 255);


    // Combine into a single 32-bit integer (0xRRGGBB)
    return (r << 24) | (g << 16) | (b << 8) | a;
}



t_xyzvektor	get_color_from_uint(uint32_t color)
{
	t_xyzvektor result;
	uint8_t a = color & 0xFF;
	uint8_t r = (color >> 24) & 0xFF;
    uint8_t g = (color >> 16) & 0xFF;
    uint8_t b = (color >> 8) & 0xFF;

    result.x = (double)r / 255;
    result.y = (double)g / 255;
    result.z = (double)b / 255;
	result.w = (double)a / 255;

    return result;
}