#pragma once
#include <k4a/k4a.h>
#include <k4arecord/types.h>
#include <k4arecord/playback.h>
#include <k4abt.h>

//´íÎó´¦Àíº¯Êý
#define VERIFY(result, error)                                                                            \
	if(result != K4A_RESULT_SUCCEEDED)                                                                   \
	{                                                                                                    \
		printf("%s \n - (File: %s, Function: %s, Line: %d)\n", error, __FILE__, __FUNCTION__, __LINE__); \
		exit(1);                                                                                         \
	}                                                                                                    \

void get_record_configint(k4a_record_configuration_t devConfig);