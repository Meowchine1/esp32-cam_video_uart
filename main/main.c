
#include "config.h"
#include "gpio.h"
#include "uart.h"

void app_main(void)
{

    init_led();
    init_uart();

    if (ESP_OK != init_camera())
    {
        return;
    }
         
    while (1)
    {

        sprintf (Txdata, "Taking picture \r\n");
        uart_write_bytes(uart_num, Txdata, strlen(Txdata));
        camera_fb_t *pic = esp_camera_fb_get();

        sprintf (Txdata, "Picture taken! Its size was: : %zu bytes \r\n", , pic->len));
        uart_write_bytes(uart_num, Txdata, strlen(Txdata));
        camera_fb_t *pic = esp_camera_fb_get();
        // use pic->buf to access the image
        esp_camera_fb_return(pic);

        vTaskDelay(5000 / portTICK_RATE_MS);

        sprintf (Txdata, "Hello world %d\r\n", pwm.duty);
        uart_write_bytes(uart_num, Txdata, strlen(Txdata));
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    
        if (pwm.direction)
        {
            pwm.duty += 100;
            if (pwm.duty > LEDC_DUTY)
            {
                pwm.direction = 0;
            }
        }
        else
        {
            pwm.duty -= 100;
            if (pwm.duty < 200)
            {
                pwm.direction = 1;
            }
        }

        update(pwm.duty);

         
    }
}