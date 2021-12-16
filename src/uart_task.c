#include "uart_task.h"

void uart_task(void *p)
{
	uart_config_t uart_config = {
		.baud_rate = 115200,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
		.source_clk = UART_SCLK_APB,
	};

	uart_driver_install(UART_NUM_1, BUF_SIZE * 2, 0, 0, NULL, 0);
	uart_param_config(UART_NUM_1, &uart_config);
	uart_set_pin(UART_NUM_1, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS);

#ifdef UART_EN
  uint8_t *data = (uint8_t *)malloc(BUF_SIZE);
  while (1)
  {
    int len = uart_read_bytes(UART_NUM_1, data, BUF_SIZE, 20 / portTICK_RATE_MS);
    if (len > 0)
    {
      uart_write_bytes(UART_NUM_1, (const char *)data, len);
      // data[len] = 0;
      // printf("DATA: %s", data);
    }
  }
#else
  char buf[] = "Hello, world\r\n";
  int len = strlen(buf);
  while(1)
  {
    uart_write_bytes(UART_NUM_1, (const char *)buf, len);
		vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
#endif  
}
