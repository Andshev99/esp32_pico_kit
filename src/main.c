#include "esp_system.h"
#include "esp_spi_flash.h"

#include "pb_encode.h"
#include "pb_decode.h"
#include "temp_pack.pb.h"
#include "uart_task.h"


void heart_task(void *p);
void green_task(void *p);
bool test_nanopb();
void chip_info();
//=================================================================

void app_main()
{
	chip_info();
	test_nanopb();
	xTaskCreate(heart_task, "heart", 1024 * 8, NULL, 5, NULL);
	xTaskCreate(green_task, "green", 512, NULL, 5, NULL);
	xTaskCreate(uart_task, "uart_echo_task", 1024, NULL, 10, NULL);
}

void heart_task(void *p)
{
	printf("heart task\n");
	gpio_pad_select_gpio(RED_LED);
	gpio_set_direction(RED_LED, GPIO_MODE_OUTPUT);
	while (1)
	{
		gpio_set_level(RED_LED, 1);
		vTaskDelay(100 / portTICK_PERIOD_MS);
		gpio_set_level(RED_LED, 0);
		vTaskDelay(1900 / portTICK_PERIOD_MS);
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

void chip_info()
{
	esp_chip_info_t chip_info;

	printf("\n\nChip info\n");
	esp_chip_info(&chip_info);
	printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
				 chip_info.cores,
				 (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
				 (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");
	printf("silicon revision %d, ", chip_info.revision);
	printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
				 (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
	// fflush(stdout);
	// esp_restart();
}

