#include "st7789_lcd.h"

uint8_t st7789_init_seq_zjy_240_240[] = {
    0x05, 0xB2, 0x0C, 0x0C, 0x00, 0x33, 0x33, // Porch Setting
    0x01, 0xB7, 0x35,                         // Gate Control
    0x01, 0xBB, 0x19,       // VCOM Setting // Factory 0x19 -> 0.725V
    0x01, 0xC0, 0x2C,       // LCM Control
    0x01, 0xC2, 0x01,       // VDV and VRH Command Enable
    0x01, 0xC3, 0x12,       // VRH Set // Factory 12H
    0x01, 0xC4, 0x20,       // VDV Set
    0x01, 0xC6, 0x0F,       // Frame Rate Control in Normal Mode
    0x02, 0xD0, 0xA4, 0xA1, // Power Control 1
    0x0E, 0xE0, 0xD0, 0x04, 0x0D, 0x11, 0x13, 0x2B,
    0x3F, 0x54, 0x4C, 0x18, 0x0D, 0x0B, 0x1F,
    0x23, // Positive Voltage Gamma Control
    0x0E, 0xE1, 0xD0, 0x04, 0x0C, 0x11, 0x13, 0x2C,
    0x3F, 0x44, 0x51, 0x2F, 0x1F, 0x1F, 0x20,
    0x23,       // Negative Voltage Gamma Control
    0x00, 0x21, // Inversion On
};

uint8_t st7789_init_seq_zjy_240_135[] = {
    0x05, 0xB2, 0x05, 0x05, 0x00, 0x33, 0x33, // Porch Setting
    0x01, 0xB7, 0x23,                         // Gate Control
    0x01, 0xBB, 0x22,       // VCOM Setting // Factory 0x19 -> 0.725V
    0x01, 0xC0, 0x2C,       // LCM Control
    0x01, 0xC2, 0x01,       // VDV and VRH Command Enable
    0x01, 0xC3, 0x13,       // VRH Set // Factory 12H
    0x01, 0xC4, 0x20,       // VDV Set
    0x01, 0xC6, 0x0F,       // Frame Rate Control in Normal Mode
    0x02, 0xD0, 0xA4, 0xA1, // Power Control 1
    0x0E, 0xE0, 0x70, 0x06, 0x0C, 0x08, 0x09, 0x27,
    0x2E, 0x34, 0x46, 0x37, 0x13, 0x13, 0x25,
    0x2A, // Positive Voltage Gamma Control
    0x0E, 0xE1, 0x70, 0x04, 0x08, 0x09, 0x07, 0x03,
    0x2C, 0x42, 0x42, 0x38, 0x14, 0x14, 0x27,
    0x2C,       // Negative Voltage Gamma Control
    0x00, 0x21, // Inversion On
};

st7789_ret_t _st7789_init_seq(st7789_lcd_t *lcd) {
    uint16_t i = 0;

    while(i < sizeof(ST7789_PANEL_SELECTION)) {
        if(lcd->cb.write_cmd_cb(lcd->user_data, &ST7789_PANEL_SELECTION[i + 1],
                                ST7789_PANEL_SELECTION[i] + 1) != ST7789_OK) {
            return ST7789_ERROR;
        };
        i += ST7789_PANEL_SELECTION[i] + 2;
    }

    return ST7789_OK;
}

st7789_ret_t _st7789_window(st7789_lcd_t *lcd, uint16_t x_start, uint16_t x_end,
                            uint16_t y_start, uint16_t y_end) {
    uint16_t real_x_start, real_x_end, real_y_start, real_y_end;

    uint16_t x_offset, y_offset;
    switch(lcd->config.direction) {
    case ST7789_DIR_0:
        x_offset = PANEL_X_OFFSET;
        y_offset = PANEL_Y_OFFSET;
        break;
    case ST7789_DIR_90:
        x_offset = PANEL_Y_OFFSET;
        y_offset = PANEL_X_OFFSET;
        break;
    case ST7789_DIR_180:
        x_offset = 0;
        y_offset = 80;
        break;
    case ST7789_DIR_270:
        x_offset = 80;
        y_offset = 0;
        break;
    default:
        x_offset = 0;
        y_offset = 0;
    }

    real_x_start = x_start + x_offset;
    real_x_end = x_end + x_offset;
    real_y_start = y_start + y_offset;
    real_y_end = y_end + y_offset;

    uint8_t tx_buf[5] = {0x2A, ((uint8_t)(real_x_start >> 0x08U) & 0xFFU),
                         (real_x_start & 0xFFU),
                         ((uint8_t)(real_x_end >> 0x08U) & 0xFFU),
                         (real_x_end & 0xFFU)};

    if(lcd->cb.write_cmd_cb(lcd->user_data, tx_buf, 0x05) != ST7789_OK) {
        return ST7789_ERROR;
    }

    tx_buf[0] = 0x2B;
    tx_buf[1] = ((uint8_t)(real_y_start >> 0x08U) & 0xFFU);
    tx_buf[2] = (real_y_start & 0xFFU);
    tx_buf[3] = ((uint8_t)(real_y_end >> 0x08U) & 0xFFU);
    tx_buf[4] = (real_y_end & 0xFFU);

    if(lcd->cb.write_cmd_cb(lcd->user_data, tx_buf, 0x05) != ST7789_OK) {
        return ST7789_ERROR;
    }

    return ST7789_OK;
}

st7789_ret_t _st7789_reset(st7789_lcd_t *lcd) {
    return lcd->cb.reset_cb(lcd->user_data);
}

st7789_ret_t st7789_lcd_init(st7789_lcd_t *lcd) {
    if(_st7789_init_seq(lcd) != ST7789_OK) return ST7789_ERROR;
    if(st7789_lcd_sleep(lcd, 0) != ST7789_OK) return ST7789_ERROR;
    if(st7789_lcd_display(lcd, 1) != ST7789_OK) return ST7789_ERROR;
}

st7789_ret_t st7789_lcd_load(st7789_lcd_t *lcd, uint8_t *data, uint16_t x_start,
                             uint16_t x_end, uint16_t y_start, uint16_t y_end) {

    uint32_t pixel_count = (y_end - y_start + 1) * (x_end - x_start + 1);

    uint32_t data_len = 0;

    switch(lcd->config.pix_fmt) {
    case ST7789_RGB444:
        data_len = pixel_count * 3 / 2;
        break;
    case ST7789_RGB565:
        data_len = pixel_count * 2;
        break;
    case ST7789_RGB666:
    case ST7789_RGB888:
        data_len = pixel_count * 3;
        break;
    default:
        data_len = pixel_count;
        break;
    }

    if(_st7789_window(lcd, x_start, x_end, y_start, y_end) != ST7789_OK) {
        return ST7789_ERROR;
    }

    if(lcd->cb.write_data_cb(lcd->user_data, data, data_len) != ST7789_OK) {
        return ST7789_ERROR;
    }

    return ST7789_OK;
}

st7789_ret_t st7789_lcd_sleep(st7789_lcd_t *lcd, uint8_t sleep_mode) {
    uint8_t command = 0x11;
    if(sleep_mode) command = 0x10;

    return lcd->cb.write_cmd_cb(lcd->user_data, &command, 0x01);
}

st7789_ret_t st7789_lcd_display(st7789_lcd_t *lcd, uint8_t display_on) {
    uint8_t command = 0x28;
    if(display_on) command = 0x29;

    return lcd->cb.write_cmd_cb(lcd->user_data, &command, 0x01);
}