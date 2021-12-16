#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include "pb_encode.h"
#include "pb_decode.h"
#include "temp_pack.pb.h"

// d:\_works\proto\nanopb\generator-bin\nanopb_generator.exe temp_pack.proto
#define BLINK_GPIO 4
bool test_nanopb();
//=================================================================

void app_main()
{
	bool result;

	gpio_pad_select_gpio(BLINK_GPIO);
	gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
	result = test_nanopb();

	while (1)
	{
		if (result)
		{
			gpio_set_level(BLINK_GPIO, 1);
			vTaskDelay(100 / portTICK_PERIOD_MS);
			gpio_set_level(BLINK_GPIO, 0);
			vTaskDelay(1900 / portTICK_PERIOD_MS);
		}
		else
		{
			gpio_set_level(BLINK_GPIO, 1);
			vTaskDelay(1000 / portTICK_PERIOD_MS);
			gpio_set_level(BLINK_GPIO, 0);
			vTaskDelay(1000 / portTICK_PERIOD_MS);
			printf("Error proto\r\n");
		}
	}
}

bool test_nanopb()
{
	uint8_t buffer[256];

	pb_TempEvent temp = pb_TempEvent_init_zero;
	temp.deviceId = 12;
	temp.eventId = 100;
	temp.humidity = 55;
	temp.tempCel = 23;
	temp.heatIdxCel = 28;

	pb_ostream_t ostream = pb_ostream_from_buffer(buffer, 256);
	if (!pb_encode(&ostream, pb_TempEvent_fields, &temp))
		return false;
	pb_TempEvent temp_new = pb_TempEvent_init_zero;
	pb_istream_t istream = pb_istream_from_buffer(buffer, ostream.bytes_written);
	if (!pb_decode(&istream, pb_TempEvent_fields, &temp_new))
		return false;
	printf("HUE=%.1f  TEMP=%.1f\r\n", temp_new.humidity, temp_new.tempCel);
	return true;
}