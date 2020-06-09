#include "get_roecord_config.h"
#include <iostream>;
#include <stdio.h>£»

using namespace std;

void get_record_configint(k4a_record_configuration_t devConfig) 
{
	switch (devConfig.depth_mode) {
	case K4A_DEPTH_MODE_NFOV_2X2BINNED:
		cout << "Depth mode of this record is NFOV_BINNED\n";
		break;
	case K4A_DEPTH_MODE_NFOV_UNBINNED:
		cout << "Depth mode of this record is UNBINNED\n";
		break;
	case K4A_DEPTH_MODE_WFOV_2X2BINNED:
		cout << "Depth mode of this record is WFOV_UNBINNED\n";
		break;
	case K4A_DEPTH_MODE_PASSIVE_IR:
		cout << "Depth mode of this record is PASSIVE_IR\n";
		break;
	}
	switch (devConfig.camera_fps) {
	case K4A_FRAMES_PER_SECOND_30:
		cout << "Frame rate of this record is 30fps\n";
		break;
	case K4A_FRAMES_PER_SECOND_15:
		cout << "Frame rate of this record is 15fps\n";
		break;
	case K4A_FRAMES_PER_SECOND_5:
		cout << "Frame rate of this record is 5fps\n";
		break;
	}
	switch (devConfig.wired_sync_mode) {
	case K4A_WIRED_SYNC_MODE_STANDALONE:
		cout << "This record is standalone!\n";
		break;
	case K4A_WIRED_SYNC_MODE_MASTER:
		cout << "This record is from the master!\n";
		break;
	case K4A_WIRED_SYNC_MODE_SUBORDINATE:
		cout << "This record is from the sub!\n";
		break;
	}
}