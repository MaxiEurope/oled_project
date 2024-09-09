#include <stdlib.h>
#include "main.h" // include the main header file from the MCU project
#include "ssd1351.h"
#include "fonts.h"

SSD1351_FONT current_font;

/**
 * @brief Selects the SSD1351 display.
 *
 * This function sets the CS (Chip Select) pin of the SSD1351 display to low,
 * enabling communication with the display.
 */
void SSD1351_Select() {
    HAL_GPIO_WritePin(SSD1351_CS_PORT, SSD1351_CS_PIN, GPIO_PIN_RESET);
}

/**
 * @brief Deselects the SSD1351 display.
 */
void SSD1351_Deselect() {
    HAL_GPIO_WritePin(SSD1351_CS_PORT, SSD1351_CS_PIN, GPIO_PIN_SET);
}

/**
 * @brief Reset the SSD1351 display.
 *
 * This function resets the SSD1351 display by toggling the reset pin.
 * The reset pin is set to high, then low, then high again with delays in between.
 */
static void SSD1351_Reset() {
    HAL_GPIO_WritePin(SSD1351_RST_PORT, SSD1351_RST_PIN, GPIO_PIN_SET);
    HAL_Delay(50);
    HAL_GPIO_WritePin(SSD1351_RST_PORT, SSD1351_RST_PIN, GPIO_PIN_RESET);
    HAL_Delay(50);
    HAL_GPIO_WritePin(SSD1351_RST_PORT, SSD1351_RST_PIN, GPIO_PIN_SET);
    HAL_Delay(10);
}

/**
 * @brief Sends a command to the SSD1351 display.
 *
 * This function is used to send a command to the SSD1351 display. It sets the DC (Data/Command) pin to low to indicate that a command is being sent, and then transmits the command using SPI communication.
 *
 * @param cmd The command to be sent to the display.
 */
static void SSD1351_Command(uint8_t cmd) {
    HAL_GPIO_WritePin(SSD1351_DC_PORT, SSD1351_DC_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&SSD1351_SPI, &cmd, sizeof(cmd), HAL_MAX_DELAY);
}

/**
 * @brief Transmits data to the SSD1351 display.
 *
 * This function is responsible for transmitting data to the SSD1351 display using SPI communication.
 * It sets the data/command pin to high and transmits the data in chunks of 32 bytes or less.
 *
 * @param data Pointer to the data buffer to be transmitted.
 * @param size Size of the data buffer in bytes.
 */
static void SSD1351_Data(uint8_t *data, size_t size) {
    HAL_GPIO_WritePin(SSD1351_DC_PORT, SSD1351_DC_PIN, GPIO_PIN_SET);

    while (size > 0) {
        size_t chunk = size > 32 ? 32 : size;
        HAL_SPI_Transmit(&SSD1351_SPI, data, chunk, HAL_MAX_DELAY);
        data += chunk;
        size -= chunk;
    }
}

/**
 * @brief Initializes the SSD1351 display.
 *
 * This function selects the SSD1351 display, performs a reset, and sends the necessary commands to initialize the display.
 * It turns off the display, sets the display mode to normal, and turns on the display.
 */
void SSD1351_Init() {
    SSD1351_Select();
    SSD1351_Reset();

    SSD1351_Command(SSD1351_DISPLAY_OFF);

    SSD1351_Command(SSD1351_NORMAL_DISPLAY);
    SSD1351_Command(SSD1351_DISPLAY_ON);
}

/**
 * Sets the window for drawing on the SSD1351 display.
 *
 * @param x The starting x-coordinate of the window.
 * @param y The starting y-coordinate of the window.
 * @param w The width of the window.
 * @param h The height of the window.
 */
static void SSD1351_SetWindow(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    int column_offset = (128 - SSD1351_WIDTH) / 2;

    SSD1351_Command(SSD1351_SET_COLUMN);
    SSD1351_Data((uint8_t[]){ (x + column_offset) & 0xFF, (w + column_offset) & 0xFF }, 2);

    SSD1351_Command(SSD1351_SET_ROW);
    SSD1351_Data((uint8_t[]){ y & 0xFF, h & 0xFF }, 2);

    SSD1351_Command(SSD1351_WRITE_RAM);
}

/**
 * @brief Draws a pixel on the SSD1351 OLED display.
 *
 * This function draws a pixel at the specified coordinates with the specified color on the SSD1351 OLED display.
 * If the coordinates are outside the display boundaries, the function returns without drawing anything.
 *
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param color The color of the pixel in RGB565 format.
 */
void SSD1351_DrawPixel(uint8_t x, uint8_t y, uint16_t color) {
    if (x >= SSD1351_WIDTH || y >= SSD1351_HEIGHT) {
        return;
    }

    SSD1351_Select();

    SSD1351_SetWindow(x, y, x, y);
    SSD1351_Data((uint8_t[]){ color >> 8, color & 0xFF }, 2);

    SSD1351_Deselect();
}

/**
 * Fills a rectangular area on the SSD1351 display with a specified color.
 *
 * @param x The x-coordinate of the top-left corner of the rectangle.
 * @param y The y-coordinate of the top-left corner of the rectangle.
 * @param w The width of the rectangle.
 * @param h The height of the rectangle.
 * @param color The color to fill the rectangle with.
 */
void SSD1351_FillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
    if (x >= SSD1351_WIDTH || y >= SSD1351_HEIGHT) {
        return;
    }
    if (x + w > SSD1351_WIDTH) {
        w = SSD1351_WIDTH - x;
    }
    if (y + h > SSD1351_HEIGHT) {
        h = SSD1351_HEIGHT - y;
    }

    SSD1351_Select();

    SSD1351_SetWindow(x, y, x + w - 1, y + h - 1);

    uint8_t data[w * h * 2];
    for (int i = 0; i < w * h; i++) {
        data[i * 2] = color >> 8;
        data[i * 2 + 1] = color & 0xFF;
    }

    SSD1351_Data(data, w * h * 2);

    SSD1351_Deselect();
}

/**
 * @brief Fills the entire screen with the specified color.
 *
 * This function fills the entire screen of the SSD1351 display with the specified color.
 *
 * @param color The color to fill the screen with.
 */
void SSD1351_FillScreen(uint16_t color) {
    SSD1351_FillRect(0, 0, SSD1351_WIDTH, SSD1351_HEIGHT, color);
}

/**
 * @brief Clears the SSD1351 display by filling it with the color black.
 */
void SSD1351_Clear() {
    SSD1351_FillScreen(0);
}

/**
 * Draws a line on the SSD1351 OLED display.
 *
 * @param x0 The starting x-coordinate of the line.
 * @param y0 The starting y-coordinate of the line.
 * @param x1 The ending x-coordinate of the line.
 * @param y1 The ending y-coordinate of the line.
 * @param color The color of the line.
 */
void SSD1351_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t color) {
    if (x0 >= SSD1351_WIDTH || y0 >= SSD1351_HEIGHT || x1 >= SSD1351_WIDTH || y1 >= SSD1351_HEIGHT) {
        return;
    }

    if (x0 == x1 && y0 == y1) {
        SSD1351_DrawPixel(x0, y0, color);
        return;
    }

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;

    while (1) {
        SSD1351_DrawPixel(x0, y0, color);

        if (x0 == x1 && y0 == y1) {
            break;
        }

        int e2 = err * 2;

        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }

        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

/**
 * @brief Draws a rectangle on the SSD1351 OLED display.
 *
 * This function draws a rectangle with the specified position, width, height, and color on the SSD1351 OLED display.
 * If the specified position is outside the display boundaries, the function does nothing.
 * If the rectangle extends beyond the display boundaries, it is automatically clipped to fit within the boundaries.
 *
 * @param x The x-coordinate of the top-left corner of the rectangle.
 * @param y The y-coordinate of the top-left corner of the rectangle.
 * @param w The width of the rectangle.
 * @param h The height of the rectangle.
 * @param color The color of the rectangle.
 */
void SSD1351_DrawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
    if (x >= SSD1351_WIDTH || y >= SSD1351_HEIGHT) {
        return;
    }

    if ((x + w) >= SSD1351_WIDTH) {
        w = SSD1351_WIDTH - x - 1;
    }

    if ((y + h) >= SSD1351_HEIGHT) {
        h = SSD1351_HEIGHT - y - 1;
    }

    if (w == 0 || h == 0) {
       return;
    }

    SSD1351_DrawLine(x, y, x + w, y, color);
    SSD1351_DrawLine(x + w, y, x + w, y + h, color);
    SSD1351_DrawLine(x + w, y + h, x, y + h, color);
    SSD1351_DrawLine(x, y + h, x, y, color);
}

/**
 * @brief Sets the font for the SSD1351 display.
 *
 * This function sets the font to be used for rendering text on the SSD1351 display.
 *
 * @param font A pointer to the font data.
 */
void SSD1351_SetFont(const uint8_t* font) {
    current_font.data = font;
    current_font.height = font[6];
    current_font.start_char = font[2] + (font[3] << 8);
    current_font.end_char = font[4] + (font[5] << 8);
}

/**
 * @brief Draws a character on the SSD1351 display.
 *
 * This function draws a character on the SSD1351 display at the specified coordinates (x, y) using the specified color and background color.
 * The character must be within the range of the current font's start and end characters, otherwise nothing will be drawn.
 *
 * @param x The x-coordinate of the top-left corner of the character.
 * @param y The y-coordinate of the top-left corner of the character.
 * @param c The character to be drawn.
 * @param color The color of the character.
 * @param bg The background color behind the character.
 */
void SSD1351_DrawChar(uint8_t x, uint8_t y, char c, uint16_t color, uint16_t bg) {
    if (c < current_font.start_char || c > current_font.end_char) {
        return;
    }

    uint16_t tmp = (c - current_font.start_char) << 2;
    const uint8_t *char_table = current_font.data + 8 + tmp;
    uint8_t char_width = *char_table;
    uint32_t offset = (uint32_t)char_table[1] | ((uint32_t)char_table[2] << 8) | ((uint32_t)char_table[3] << 16);
    const uint8_t *char_bitmap = current_font.data + offset;

    for (uint8_t ycount = 0; ycount < current_font.height; ycount++) {
        uint8_t temp = 0;
        uint8_t mask = 0;
        for (uint8_t xcount = 0; xcount < char_width; xcount++) {
            if (!mask) {
                temp = *char_bitmap++;
                mask = 0x01;
            }
            SSD1351_DrawPixel(x + xcount, y + ycount, (temp & mask) ? color : bg);
            mask <<= 1;
        }
    }
}

/**
 * @brief Draws a string on the SSD1351 OLED display.
 *
 * This function draws a string on the SSD1351 OLED display at the specified coordinates (x, y).
 * The string is specified by the input parameter 'str'. The color of the string and the background
 * color can also be specified using the 'color' and 'bg' parameters, respectively.
 *
 * @param x The x-coordinate of the starting position of the string.
 * @param y The y-coordinate of the starting position of the string.
 * @param str The string to be drawn on the display.
 * @param color The color of the string.
 * @param bg The background color.
 */
void SSD1351_DrawString(uint8_t x, uint8_t y, const char* str, uint16_t color, uint16_t bg) {
    if (!str || x >= SSD1351_WIDTH || y >= SSD1351_HEIGHT) {
        return;
    }

    uint8_t original = x;

    while (*str) {
        char c = *str++;

        if (c == '\n') {
            y += current_font.height;
            x = original;
            continue;
        }

        uint16_t tmp = (c - current_font.start_char) << 2;
        const uint8_t *char_table = current_font.data + 8 + tmp;
        uint8_t char_width = *char_table;

        if (x + char_width > SSD1351_WIDTH) {
            x = original;
            y += current_font.height;
        }

        SSD1351_DrawChar(x, y, c, color, bg);

        x += char_width + 1;
    }
}

/**
 * @brief Calculates the width of a string in pixels.
 *
 * This function calculates the width of a string in pixels based on the current font.
 * It iterates through each character in the string and checks if it falls within the range of the current font.
 * If the character is within the range, it adds the width of the character to the total width.
 *
 * @param str The string for which to calculate the width.
 * @return The width of the string in pixels.
 */
uint8_t SSD1351_GetStringWidth(const char* str) {
    if (!str) {
        return 0;
    }

    uint8_t width = 0;

    while (*str) {
        char c = *str++;

        if (c < current_font.start_char || c > current_font.end_char) {
            continue;
        }

        uint16_t tmp = (c - current_font.start_char) << 2;
        const uint8_t *char_table = current_font.data + 8 + tmp;
        width += *char_table + 1;
    }

    return width;
}

/**
 * Draws a string centered on the display.
 *
 * @param y The y-coordinate of the string.
 * @param str The string to be drawn.
 * @param color The color of the string.
 * @param bg The background color.
 */
void SSD1351_DrawStringCentered(uint8_t y, const char* str, uint16_t color, uint16_t bg) {
    if (!str || y >= SSD1351_HEIGHT) {
        return;
    }

    uint8_t x = 0;
    uint8_t str_width = SSD1351_GetStringWidth(str);

    if (str_width <= SSD1351_WIDTH) {
        x = (SSD1351_WIDTH - str_width) / 2;
        SSD1351_DrawString(x, y, str, color, bg);
    } else {
        char buffer[64];
        int buf_index = 0;

        while (*str) {
            buffer[buf_index++] = *str++;
            buffer[buf_index] = '\0';

            if (SSD1351_GetStringWidth(buffer) > SSD1351_WIDTH) {
                buffer[--buf_index] = '\0';
                x = (SSD1351_WIDTH - SSD1351_GetStringWidth(buffer)) / 2;
                SSD1351_DrawString(x, y, buffer, color, bg);

                y += current_font.height;
                if (y >= SSD1351_HEIGHT) {
                    break;
                }

                buffer[0] = *str++;
                buf_index = 1;
                buffer[buf_index] = '\0';
            }
        }

        if (buf_index > 0) {
            x = (SSD1351_WIDTH - SSD1351_GetStringWidth(buffer)) / 2;
            SSD1351_DrawString(x, y, buffer, color, bg);
        }
    }
}
