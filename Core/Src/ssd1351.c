#include <stdlib.h>
#include "main.h" // include the main header file from the MCU project
#include "ssd1351.h"
#include "fonts.h"

SSD1351_FONT current_font;

void SSD1351_Select() {
    HAL_GPIO_WritePin(SSD1351_CS_PORT, SSD1351_CS_PIN, GPIO_PIN_RESET);
}

void SSD1351_Deselect() {
    HAL_GPIO_WritePin(SSD1351_CS_PORT, SSD1351_CS_PIN, GPIO_PIN_SET);
}

static void SSD1351_Reset() {
    HAL_GPIO_WritePin(SSD1351_RST_PORT, SSD1351_RST_PIN, GPIO_PIN_SET);
    HAL_Delay(50);
    HAL_GPIO_WritePin(SSD1351_RST_PORT, SSD1351_RST_PIN, GPIO_PIN_RESET);
    HAL_Delay(50);
    HAL_GPIO_WritePin(SSD1351_RST_PORT, SSD1351_RST_PIN, GPIO_PIN_SET);
    HAL_Delay(10);
}

static void SSD1351_Command(uint8_t cmd) {
    HAL_GPIO_WritePin(SSD1351_DC_PORT, SSD1351_DC_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&SSD1351_SPI, &cmd, sizeof(cmd), HAL_MAX_DELAY);
}

static void SSD1351_Data(uint8_t *data, size_t size) {
    HAL_GPIO_WritePin(SSD1351_DC_PORT, SSD1351_DC_PIN, GPIO_PIN_SET);

    while (size > 0) {
        size_t chunk = size > 32 ? 32 : size;
        HAL_SPI_Transmit(&SSD1351_SPI, data, chunk, HAL_MAX_DELAY);
        data += chunk;
        size -= chunk;
    }
}

void SSD1351_Init() {
    SSD1351_Select();
    SSD1351_Reset();

    SSD1351_Command(SSD1351_DISPLAY_OFF);

    SSD1351_Command(SSD1351_NORMAL_DISPLAY);
    SSD1351_Command(SSD1351_DISPLAY_ON);
}

static void SSD1351_SetWindow(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    int column_offset = (128 - SSD1351_WIDTH) / 2;

    SSD1351_Command(SSD1351_SET_COLUMN);
    SSD1351_Data((uint8_t[]){ (x + column_offset) & 0xFF, (w + column_offset) & 0xFF }, 2);

    SSD1351_Command(SSD1351_SET_ROW);
    SSD1351_Data((uint8_t[]){ y & 0xFF, h & 0xFF }, 2);

    SSD1351_Command(SSD1351_WRITE_RAM);
}

void SSD1351_DrawPixel(uint8_t x, uint8_t y, uint16_t color) {
    if (x >= SSD1351_WIDTH || y >= SSD1351_HEIGHT) {
        return;
    }

    SSD1351_Select();

    SSD1351_SetWindow(x, y, x, y);
    SSD1351_Data((uint8_t[]){ color >> 8, color & 0xFF }, 2);

    SSD1351_Deselect();
}

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

void SSD1351_FillScreen(uint16_t color) {
    SSD1351_FillRect(0, 0, SSD1351_WIDTH, SSD1351_HEIGHT, color);
}

void SSD1351_Clear() {
    SSD1351_FillScreen(0);
}

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

void SSD1351_SetFont(const uint8_t* font) {
    current_font.data = font;
    current_font.height = font[6];
    current_font.start_char = font[2] + (font[3] << 8);
    current_font.end_char = font[4] + (font[5] << 8);
}

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

void SSD1351_DrawString(uint8_t x, uint8_t y, const char* str, uint16_t color, uint16_t bg) {
    if (!str) {
        return;
    }

    if (x >= SSD1351_WIDTH || y >= SSD1351_HEIGHT) {
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
