
// #include "config.h"
// #include "gpio.h"
// #include "uart.h"

// void app_main(void)
// {
//     uint8_t *img_buf;
//     size_t img_size;

//     sprintf(Txdata, "Taking picture \r\n");
//     uart_write_bytes(uart_num, Txdata, strlen(Txdata));
//     while (ESP_OK != init_camera())
//     {
//         sprintf(Txdata, "error with cam initialize \r\n");
//         uart_write_bytes(uart_num, Txdata, strlen(Txdata));
//     }

//     sprintf(Txdata, "camera ok \r\n");
//     uart_write_bytes(uart_num, Txdata, strlen(Txdata));
//     init_led();
//     init_uart();

//     while (1)
//     {
//         sprintf(Txdata, "Taking picture \r\n");
//         uart_write_bytes(uart_num, Txdata, strlen(Txdata));

//         camera_fb_t *pic = esp_camera_fb_get();
//         vTaskDelay(2000 / portTICK_PERIOD_MS);
//         if (!pic)
//         {
//             ESP_LOGE(TAG, "Camera capture failed");
//             //return;
//         }
//         img_buf = pic->buf;
//         img_size = pic->len;
//         uart_write_bytes(uart_num, (const char *)img_buf, img_size);

//         sprintf(Txdata, "Picture taken! Its size was: : %zu bytes \r\n", pic->len);
//         uart_write_bytes(uart_num, Txdata, strlen(Txdata));
//         // use pic->buf to access the image
//         esp_camera_fb_return(pic);

//         sprintf(Txdata, "Hello world %d\r\n", pwm.duty);
//         uart_write_bytes(uart_num, Txdata, strlen(Txdata));
//         vTaskDelay(2000 / portTICK_PERIOD_MS);

//         if (pwm.direction)
//         {
//             pwm.duty += 100;
//             if (pwm.duty > LEDC_DUTY)
//             {
//                 pwm.direction = 0;
//             }
//         }
//         else
//         {
//             pwm.duty -= 100;
//             if (pwm.duty < 200)
//             {
//                 pwm.direction = 1;
//             }
//         }

//         update(pwm.duty);
//     }
// }

#include <stdio.h>
#include <stdlib.h>
#include <esp_event_loop.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/sdmmc_host.h"
#include "driver/sdmmc_defs.h"
#include "sdmmc_cmd.h"
#include "esp_vfs_fat.h"

#include "esp_camera.h"

#include "uart.h"
//#include "config.h"

static const char *TAG = "Camera";
#define MOUNT_POINT "/sdcard"

#define BOARD_ESP32CAM_AITHINKER 1

// ESP32Cam (AiThinker) PIN Map
#ifdef BOARD_ESP32CAM_AITHINKER

#define CAM_PIN_PWDN 32
#define CAM_PIN_RESET -1 // software reset will be performed
#define CAM_PIN_XCLK 0
#define CAM_PIN_SIOD 26
#define CAM_PIN_SIOC 27

#define CAM_PIN_D7 35
#define CAM_PIN_D6 34
#define CAM_PIN_D5 39
#define CAM_PIN_D4 36
#define CAM_PIN_D3 21
#define CAM_PIN_D2 19
#define CAM_PIN_D1 18
#define CAM_PIN_D0 5
#define CAM_PIN_VSYNC 25
#define CAM_PIN_HREF 23
#define CAM_PIN_PCLK 22

#endif

static camera_config_t camera_config = {
    .pin_pwdn = CAM_PIN_PWDN,
    .pin_reset = CAM_PIN_RESET,
    .pin_xclk = CAM_PIN_XCLK,
    .pin_sscb_sda = CAM_PIN_SIOD,
    .pin_sscb_scl = CAM_PIN_SIOC,

    .pin_d7 = CAM_PIN_D7,
    .pin_d6 = CAM_PIN_D6,
    .pin_d5 = CAM_PIN_D5,
    .pin_d4 = CAM_PIN_D4,
    .pin_d3 = CAM_PIN_D3,
    .pin_d2 = CAM_PIN_D2,
    .pin_d1 = CAM_PIN_D1,
    .pin_d0 = CAM_PIN_D0,
    .pin_vsync = CAM_PIN_VSYNC,
    .pin_href = CAM_PIN_HREF,
    .pin_pclk = CAM_PIN_PCLK,

    // XCLK 20MHz or 10MHz for OV2640 double FPS (Experimental)
    .xclk_freq_hz = 20000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,

    .pixel_format = PIXFORMAT_JPEG, // YUV422,GRAYSCALE,RGB565,JPEG
    .frame_size = FRAMESIZE_UXGA,   // QQVGA-UXGA Do not use sizes above QVGA when not JPEG

    .jpeg_quality = 12, // 0-63 lower number means higher quality
    .fb_count = 1       // if more than one, i2s runs in continuous mode. Use only with JPEG
};

static esp_err_t init_camera()
{
    // initialize the camera
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Camera Init Failed");
        return err;
    }

    return ESP_OK;
}

static void init_sdcard()
{
    esp_err_t ret = ESP_FAIL;

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024};
    sdmmc_card_t *card;

    const char mount_point[] = MOUNT_POINT;
    ESP_LOGI(TAG, "Initializing SD card");

    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();

    ESP_LOGI(TAG, "Mounting SD card...");
    gpio_set_pull_mode(15, GPIO_PULLUP_ONLY); // CMD, needed in 4- and 1- line modes
    gpio_set_pull_mode(2, GPIO_PULLUP_ONLY);  // D0, needed in 4- and 1-line modes
    gpio_set_pull_mode(4, GPIO_PULLUP_ONLY);  // D1, needed in 4-line mode only
    gpio_set_pull_mode(12, GPIO_PULLUP_ONLY); // D2, needed in 4-line mode only
    gpio_set_pull_mode(13, GPIO_PULLUP_ONLY); // D3, needed in 4- and 1-line modes

    ret = esp_vfs_fat_sdmmc_mount(mount_point, &host, &slot_config, &mount_config, &card);

    if (ret == ESP_OK)
    {
        ESP_LOGI(TAG, "SD card mount successfully!");
    }
    else
    {
        ESP_LOGE(TAG, "Failed to mount SD card VFAT filesystem. Error: %s", esp_err_to_name(ret));
    }

    // Card has been initialized, print its properties
    // sdmmc_card_print_info(stdout, card);
}

 
void app_main(void)
{
    int counter = 0;
    uint8_t *img_buf;
    size_t img_size;
    init_camera();
    init_sdcard();
    init_uart();
    while (1)
    {

        ESP_LOGI(TAG, "Taking picture %d...", counter);
        camera_fb_t *pic = esp_camera_fb_get();

        counter++;

        if (pic == NULL)
        {
            ESP_LOGI(TAG, "Pic == NULL");
        }
        char *pic_name = malloc(30 + sizeof(int64_t));
        sprintf(pic_name, MOUNT_POINT "/pic_%d.jpg", counter);
        FILE *file = fopen(pic_name, "w");

        if (file != NULL)
        {
            fwrite(pic->buf, 1, pic->len, file);
            ESP_LOGI(TAG, "File saved: %s", pic_name);

            img_buf = pic->buf;
            img_size = pic->len;
            uart_write_bytes(uart_num, (const char *)img_buf, img_size);
        }
        else
        {
            ESP_LOGE(TAG, "Could not open file =(");
        }
        fclose(file);
        free(pic_name);
        esp_camera_fb_return(pic);

        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}
