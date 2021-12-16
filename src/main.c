#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include "pb_encode.h"
#include "pb_decode.h"
#include "temp_pack.pb.h"

// d:\_works\proto\nanopb\generator-bin\nanopb_generator.exe temp_pack.proto
#define RED_LED 		4
#define GREEN_LED 	5

bool test_nanopb();
void heart_task(void *p);
void green_task(void *p);
//=================================================================

void app_main()
{
	test_nanopb();
	xTaskCreate(heart_task, "heart", 1024*8, NULL, 5, NULL);
	xTaskCreate(green_task, "green", 1024*8, NULL, 5, NULL);
}

void heart_task(void *p)
{
	gpio_pad_select_gpio(RED_LED);
	gpio_set_direction(RED_LED, GPIO_MODE_OUTPUT);
	while (1)
	{
		gpio_set_level(RED_LED, 1);
		vTaskDelay(100 / portTICK_PERIOD_MS);
		gpio_set_level(RED_LED, 0);
		vTaskDelay(1900 / portTICK_PERIOD_MS);
		printf("red led\r\n");
	}
}

void green_task(void *p)
{
	gpio_pad_select_gpio(GREEN_LED);
	gpio_set_direction(GREEN_LED, GPIO_MODE_OUTPUT);
	while (1)
	{
		gpio_set_level(GREEN_LED, 1);
		vTaskDelay(100 / portTICK_PERIOD_MS);
		gpio_set_level(GREEN_LED, 0);
		vTaskDelay(3000 / portTICK_PERIOD_MS);
		printf("green led\r\n");
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