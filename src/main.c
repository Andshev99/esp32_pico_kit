#include <stdio.h>

#include "pb_encode.h"
#include "pb_decode.h"
#include "temp_pack.pb.h"

// d:\_works\proto\nanopb\generator-bin\nanopb_generator.exe temp_pack.proto

void app_main()
{
	int status = 0;

	uint8_t buffer[256];
	pb_ostream_t ostream;
	pb_istream_t istream;
	// size_t written;

	pb_TempEvent temp = pb_TempEvent_init_zero;
	temp.deviceId = 12;
	temp.eventId = 100;
	temp.humidity = 55;
	temp.tempCel = 23;
	temp.heatIdxCel = 28;

	printf("Start coder\r\n");
  ostream = pb_ostream_from_buffer(buffer, 256);
  if (!pb_encode(&ostream, pb_TempEvent_fields, &temp))
	{
		status = 1;
	 	printf("Error coder\r\n");
	}

	if(status == 0)
	{
		printf("Start decoder\r\n");
		pb_TempEvent temp_new = pb_TempEvent_init_zero;
		istream = pb_istream_from_buffer(buffer, ostream.bytes_written);
		if(!pb_decode(&istream, pb_TempEvent_fields, &temp_new))
		{
		 	printf("Error decoder\r\n");
		}
		else
		{
		 	printf("Decode Success\r\n");
			printf("HUE=%.1f  TEMP=%.1f\r\n", temp_new.humidity, temp_new.tempCel); 
		 	printf("Finish\r\n");
		}
	} 
}