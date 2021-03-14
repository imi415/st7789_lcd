#include "st7789_lcd.h"

uint8_t ST7789_PANEL_ZJY_240_240[] = {
    0x05, 0xB2, 0x0C, 0x0C, 0x00, 0x33, 0x33, // Porch Setting
    0x01, 0xB7, 0x35, // Gate Control
    0x01, 0xBB, 0x19, // VCOM Setting // Factory 0x19 -> 0.725V
    0x01, 0xC0, 0x2C, // LCM Control
    0x01, 0xC2, 0x01, // VDV and VRH Command Enable
    0x01, 0xC3, 0x12, // VRH Set // Factory 12H
    0x01, 0xC4, 0x20, // VDV Set
    0x01, 0xC6, 0x0F, //Frame Rate Control in Normal Mode
    0x02, 0xD0, 0xA4, 0xA1, // Power Control 1
    0x0E, 0xE0, 0xD0, 0x04, 0x0D, 0x11, 0x13, 0x2B, 0x3F, 0x54, 0x4C, 0x18, 0x0D, 0x0B, 0x1F,
    0x23, // Positive Voltage Gamma Control
    0x0E, 0xE1, 0xD0, 0x04, 0x0C, 0x11, 0x13, 0x2C, 0x3F, 0x44, 0x51, 0x2F, 0x1F, 0x1F, 0x20,
    0x23, // Negative Voltage Gamma Control
    0x00, 0x21, // Inversion On
};

st7789_ret_t _st7789_init_seq(st7789_lcd_t *lcd) {
    uint16_t i = 0;

    while(i < sizeof(ST7789_PANEL_SELECTION)) {
        if(lcd->cb.write_cmd_cb(lcd->user_data, &ST7789_PANEL_SELECTION[i + 1], ST7789_PANEL_SELECTION[i] + 1) != ST7789_OK) {
            return ST7789_ERROR;
        };
        i += ST7789_PANEL_SELECTION[i] + 2;
    }
}

st7789_ret_t _st7789_window(st7789_lcd_t *lcd, uint16_t x_start, uint16_t x_end, uint16_t y_start, uint16_t y_end) {
    // Cursor
}

st7789_ret_t st7789_lcd_init(st7789_lcd_t *lcd) {
    if(_st7789_init_seq(lcd) != ST7789_OK) return ST7789_ERROR;
}

st7789_ret_t st7789_lcd_load(st7789_lcd_t *lcd, uint8_t *data, uint16_t x_start, uint16_t x_end, uint16_t y_start, uint16_t y_end) {
    uint32_t data_len = (y_end - y_start + 1) * (x_end - x_start + 1);

    if(_st7789_window(lcd, x_start, x_end, y_start, y_end) != ST7789_OK) return ST7789_ERROR;
    if(lcd->cb.write_data_cb(lcd->user_data, data, data_len) != ST7789_OK) return ST7789_ERROR;

    return ST7789_OK;
}