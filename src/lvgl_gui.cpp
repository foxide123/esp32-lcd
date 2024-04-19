/*
*  lvgl_gui.cpp
*
*  Created on: Mar 25, 2024
*/
/******************************************************************************/

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/

#include <Arduino.h>
#include <lvgl.h>
#include <Wire.h>
#include <SPI.h>
#include <Arduino_GFX_Library.h>
#include "app_config.h"
#include "app_config.h"
#include "ui/ui.h"
#include "lvgl_events.h"
#include "lvgl_gui.h"

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/



/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

static const uint8_t st7701_init_operations[] = {
    BEGIN_WRITE,
    WRITE_COMMAND_8, 0x11,
    DELAY, 20,

    WRITE_COMMAND_8, 0xFF,
    WRITE_BYTES, 5, 0x77, 0x01, 0x00, 0x00, 0x13,

    WRITE_C8_D8, 0xEF, 0x08,

    WRITE_COMMAND_8, 0xFF,
    WRITE_BYTES, 5, 0x77, 0x01, 0x00, 0x00, 0x10,

    WRITE_COMMAND_8, 0xC0,
    WRITE_BYTES, 2, 0xE5, 0x02,

    WRITE_COMMAND_8, 0xC1,
    WRITE_BYTES, 2, 0x0C, 0x0A,

    WRITE_COMMAND_8, 0xC2,
    WRITE_BYTES, 2, 0x07, 0x0F,

    WRITE_C8_D8, 0xC3, 0x02,
    WRITE_C8_D8, 0xCC, 0x10,
    WRITE_C8_D8, 0xCD, 0x08,

    WRITE_COMMAND_8, 0xB0,
    WRITE_BYTES, 16, 0x00, 0x08, 0x51, 0x0D, 0xCE, 0x06, 0x00, 0x08, 0x08, 0x1D, 0x02, 0xD0, 0x0F, 0x6F, 0x36, 0x3F,

    WRITE_COMMAND_8, 0xB1,
    WRITE_BYTES, 16, 0x00, 0x10, 0x4F, 0x0C, 0x11, 0x05, 0x00, 0x07, 0x07, 0x1F, 0x05, 0xD3, 0x11, 0x6E, 0x34, 0x3F,

    WRITE_COMMAND_8, 0xFF,
    WRITE_BYTES, 5, 0x77, 0x01, 0x00, 0x00, 0x11,

    WRITE_C8_D8, 0xB0, 0x4D,
    WRITE_C8_D8, 0xB1, 0x1C,
    WRITE_C8_D8, 0xB2, 0x87,
    WRITE_C8_D8, 0xB3, 0x80,

    WRITE_C8_D8, 0xB5, 0x47,
    WRITE_C8_D8, 0xB7, 0x85,
    WRITE_C8_D8, 0xB8, 0x21,
    WRITE_C8_D8, 0xB9, 0x10,

    WRITE_C8_D8, 0xC1, 0x78,
    WRITE_C8_D8, 0xC2, 0x78,
    WRITE_C8_D8, 0xD0, 0x88,
    DELAY, 100,

    WRITE_COMMAND_8, 0xE0,
    WRITE_BYTES, 3, 0x80, 0x00, 0x02,

    WRITE_COMMAND_8, 0xE1,
    WRITE_BYTES, 11, 0x04, 0xA0, 0x00, 0x00, 0x05, 0xA0, 0x00, 0x00, 0x00, 0x60, 0x60,

    WRITE_COMMAND_8, 0xE2,
    WRITE_BYTES, 13, 0x30, 0x30, 0x60, 0x60, 0x3C, 0xA0, 0x00, 0x00, 0x3D, 0xA0, 0x00, 0x00, 0x00,

    WRITE_COMMAND_8, 0xE3,
    WRITE_BYTES, 4, 0x00, 0x00, 0x33, 0x33,

    WRITE_COMMAND_8, 0xE4,
    WRITE_BYTES, 2, 0x44, 0x44,

    WRITE_COMMAND_8, 0xE5,
    WRITE_BYTES, 16, 0x06, 0x3E, 0xA0, 0xA0, 0x08, 0x40, 0xA0, 0xA0, 0x0A, 0x42, 0xA0, 0xA0, 0x0C, 0x44, 0xA0, 0xA0,

    WRITE_COMMAND_8, 0xE6,
    WRITE_BYTES, 4, 0x00, 0x00, 0x33, 0x33,

    WRITE_COMMAND_8, 0xE7,
    WRITE_BYTES, 2, 0x44, 0x44,

    WRITE_COMMAND_8, 0xE8,
    WRITE_BYTES, 16, 0x07, 0x3F, 0xA0, 0xA0, 0x09, 0x41, 0xA0, 0xA0, 0x0B, 0x43, 0xA0, 0xA0, 0x0D, 0x45, 0xA0, 0xA0,

    WRITE_COMMAND_8, 0xEB,
    WRITE_BYTES, 7, 0x00, 0x01, 0x4E, 0x4E, 0xEE, 0x44, 0x00,

    WRITE_COMMAND_8, 0xED,
    WRITE_BYTES, 16, 0xFF, 0xFF, 0x04, 0x56, 0x72, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x27, 0x65, 0x40, 0xFF, 0xFF,

    WRITE_COMMAND_8, 0xEF,
    WRITE_BYTES, 6, 0x10, 0x0D, 0x04, 0x08, 0x3F, 0x1F,

    WRITE_COMMAND_8, 0xFF,
    WRITE_BYTES, 5, 0x77, 0x01, 0x00, 0x00, 0x13,

    WRITE_COMMAND_8, 0xE8,
    WRITE_BYTES, 2, 0x00, 0x0E,

    WRITE_COMMAND_8, 0xFF,
    WRITE_BYTES, 5, 0x77, 0x01, 0x00, 0x00, 0x00,

    WRITE_COMMAND_8, 0x11,
    END_WRITE,
    DELAY, 120,

    BEGIN_WRITE,
    WRITE_COMMAND_8, 0xFF,
    WRITE_BYTES, 5, 0x77, 0x01, 0x00, 0x00, 0x13,

    WRITE_COMMAND_8, 0xE8,
    WRITE_BYTES, 2, 0x00, 0x0C,
    DELAY, 10,

    WRITE_COMMAND_8, 0xE8,
    WRITE_BYTES, 2, 0x00, 0x00,

    WRITE_COMMAND_8, 0xFF,
    WRITE_BYTES, 5, 0x77, 0x01, 0x00, 0x00, 0x00,

    WRITE_C8_D8, 0x36, 0x00,
    WRITE_C8_D8, 0x3A, 0x55,
    WRITE_COMMAND_8, 0x11,
    END_WRITE,
    DELAY, 120,

    BEGIN_WRITE,
    WRITE_COMMAND_8, 0x29,
    DELAY, 20,
    END_WRITE
};

/* ST7701 bus */
Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
    1 /* CS */, 12 /* SCK */, 11 /* SDA */,
    45 /* DE */, 4/* VSYNC */, 5 /* HSYNC */, 21 /* PCLK */,
    39 /* R0 */, 40 /* R1 */, 41 /* R2 */, 42 /* R3 */, 2 /* R4 */,
    0 /* G0/P22 */, 9 /* G1/P23 */, 14 /* G2/P24 */, 47 /* G3/P25 */, 48 /* G4/P26 */, 3 /* G5 */,
    6 /* B0 */, 7 /* B1 */, 15 /* B2 */, 16 /* B3 */, 8 /* B4 */
);

/* ST7701 lcd */
Arduino_ST7701_RGBPanel *lcd = new Arduino_ST7701_RGBPanel(
    bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */,
    true /* IPS */, LCD_WIDTH /* width */, LCD_HEIGHT /* height */,
    st7701_init_operations, sizeof(st7701_init_operations),
    true /* BGR */
);

/* For lvgl library process */
static lv_disp_draw_buf_t draw_buf;
static lv_color_t disp_draw_buf[LCD_WIDTH * LCD_HEIGHT / 10];
static lv_disp_drv_t disp_drv;

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/



/******************************************************************************/
/*                                FUNCTIONS                                   */
/******************************************************************************/



/******************************************************************************/

/* Read touching points */
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {

}

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

#if (LV_COLOR_16_SWAP != 0)
    lcd->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#else
    lcd->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#endif

    lv_disp_flush_ready(disp);
}

/**
 * @brief  Increase tick for lvgl task
 */
static void lv_tick_inc_cb(void *data) {
    uint32_t tick_inc_period_ms = *((uint32_t *) data);
    lv_tick_inc(tick_inc_period_ms);
}

/**
 * @brief  Create timer to increase tick
 */
static void lvgl_tick_init(void) {
    static const uint32_t tick_inc_period_ms = LVGL_TICK_HANDLER;
    static esp_timer_create_args_t periodic_timer_args;

    periodic_timer_args.callback = lv_tick_inc_cb;
    periodic_timer_args.name = "tick";     /* name is optional, but may help identify the timer when debugging */
    periodic_timer_args.arg = (void *) &tick_inc_period_ms;
    periodic_timer_args.dispatch_method = ESP_TIMER_TASK;
    periodic_timer_args.skip_unhandled_events = true;

    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));

    /* The timer has been created but is not running yet. Start the timer now */
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, tick_inc_period_ms * 1000));
}

/**
 * @brief  Task for update lvgl
 */
static void lvgl_task(void *arg) {
    /* Create UI */
    // ui_init();
    // lvgl_events_init();

    while (1) {
        lv_task_handler();    /* Let the GUI do its work */
        delay(LVGL_TICK_HANDLER);
    }
}


/**
 * @brief  Initialize UI
 */
void lcd_gui_init(void) {
    /* Reset LCD */
#ifdef TFT_RST
    pinMode(TFT_RST, OUTPUT);
    digitalWrite(TFT_BL, LOW);
    delay(1000);
    digitalWrite(TFT_BL, HIGH);
    delay(1000);
#endif

    /* Turn back-light on */
#ifdef TFT_BL
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
    delay(100);
#endif

    /* Unused pin */
    pinMode(B0_PIN, OUTPUT);
    digitalWrite(B0_PIN, LOW);
    pinMode(R0_PIN, OUTPUT);
    digitalWrite(R0_PIN, LOW);

    lcd->begin();
    lcd->fillScreen(BLACK);
    delay(100);
    lcd->fillScreen(RED);
    delay(1000);
    lcd->fillScreen(BLUE);
    delay(1000);
    lcd->fillScreen(GREEN);
    delay(1000);

    lv_init();
    lvgl_tick_init();
    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, LCD_WIDTH * LCD_HEIGHT / 10);

    /* Initialize the display */
    lv_disp_drv_init(&disp_drv);

    /* Change the following line to your display resolution */
    disp_drv.hor_res = LCD_WIDTH;
    disp_drv.ver_res = LCD_HEIGHT;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    /* Initialize the (dummy) input device driver */
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);

    /* Create lvgl task */
    xTaskCreatePinnedToCore(lvgl_task, "LVGL", 12288, NULL, 4, NULL, 1);
}