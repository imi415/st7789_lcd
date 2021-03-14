#ifndef __ST7789_LCD_H
#define __ST7789_LCD_H

#include <stdint.h>

typedef enum {
    ST7789_OK,
    ST7789_ERROR
} st7789_ret_t;

typedef struct {
    st7789_ret_t (*reset_cb)(void *handle);
    st7789_ret_t (*write_cmd_cb)(void *handle, uint8_t *cmd, uint8_t len);
    st7789_ret_t (*write_data_cb)(void *handle, uint8_t *data, uint8_t len);
} st7789_cb_t;

typedef struct {
    void *user_data;
    st7789_cb_t cb;
} st7789_lcd_t;

#ifndef ST7789_PANEL_SELECTION
#define ST7789_PANEL_SELECTION ST7789_PANEL_ZJY_240_240
#endif

st7789_ret_t st7789_lcd_init(st7789_lcd_t *lcd);
st7789_ret_t st7789_lcd_load(st7789_lcd_t *lcd, uint8_t *data, uint16_t x_start, uint16_t x_end, uint16_t y_start, uint16_t y_end);

#endif