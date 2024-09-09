#ifndef COLORS_H
#define COLORS_H

#include <stdint.h>

typedef struct {
    uint32_t hex;
    const char *name;
} Color;

extern Color colors[];

void RGB565ToRGB888(uint16_t color565, uint8_t *r, uint8_t *g, uint8_t *b);
void HexToRGB888(uint32_t hex, uint8_t *r, uint8_t *g, uint8_t *b);
const char* FindClosestColor(uint16_t generatedColor);

#endif // COLORS_H
