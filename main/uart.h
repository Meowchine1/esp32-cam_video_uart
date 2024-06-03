#include "driver/uart.h"

#define TXD_PIN (GPIO_NUM_1)
#define RXD_PIN (GPIO_NUM_3)
const uart_port_t uart_num = UART_NUM_0;

char* Txdata = (char*) malloc(100);


void init_uart()
{
     
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
    // Setup UART buffered IO with event queue
    uart_param_config(uart_num, &uart_config);
    uart_set_pin(uart_num, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(uart_num, 1024, 0, 0, NULL, 0);

}

void TX_message(char* message){
sprintf (Txdata, "Taking picture \r\n");
        uart_write_bytes(uart_num, Txdata, strlen(Txdata));

}
