#ifndef __ST7789_LCD_H
#define __ST7789_LCD_H

#include <stdint.h>

typedef enum {
    ST7789_OK,
    ST7789_ERROR,
} st7789_ret_t;

typedef enum {
    ST7789_DIR_0 = 0x08U,
    ST7789_DIR_90 = 0x68U,
    ST7789_DIR_180 = 0xC8U,
    ST7789_DIR_270 = 0xA8U,
} st7789_direction_t;

typedef enum {
    ST7789_RGB444 = 3,
    ST7789_RGB565 = 5,
    ST7789_RGB666 = 6,
    ST7789_RGB888 = 7
} st7789_pixfmt_t;

typedef struct {
    st7789_ret_t (*reset_cb)(void *handle);
    st7789_ret_t (*write_cmd_cb)(void *handle, uint8_t *cmd, uint8_t len);
    st7789_ret_t (*write_data_cb)(void *handle, uint8_t *data, uint32_t len);
} st7789_cb_t;

typedef struct {
    st7789_direction_t direction;
    st7789_pixfmt_t pix_fmt;
    uint8_t inversion;
    uint8_t bgr_mode;
} st7789_config_t;

typedef struct {
    void *user_data;
    st7789_cb_t cb;
    st7789_config_t config;
} st7789_lcd_t;

#ifdef ST7789_PANEL_ZJY_240

#define ST7789_PANEL_SELECTION st7789_init_seq_zjy_240_240
#define PANEL_X_OFFSET 0
#define PANEL_Y_OFFSET 0

#elif ST7789_PANEL_ZJY_135

#define ST7789_PANEL_SELECTION st7789_init_seq_zjy_240_135
#define PANEL_X_OFFSET 53
#define PANEL_Y_OFFSET 40

#else

#warning "Panel not defined, using default."
#define ST7789_PANEL_SELECTION st7789_init_seq_zjy_240_240
#define PANEL_X_OFFSET 0
#define PANEL_Y_OFFSET 0

#endif

st7789_ret_t st7789_lcd_init(st7789_lcd_t *lcd);
st7789_ret_t st7789_lcd_load(st7789_lcd_t *lcd, uint8_t *data, uint16_t x_start,
                             uint16_t x_end, uint16_t y_start, uint16_t y_end);
st7789_ret_t st7789_lcd_sleep(st7789_lcd_t *lcd, uint8_t sleep_mode);
st7789_ret_t st7789_lcd_display(st7789_lcd_t *lcd, uint8_t display_on);
st7789_ret_t st7789_lcd_config(st7789_lcd_t *lcd, st7789_config_t *config);
#endif
