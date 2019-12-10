#pragma once
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<k4abttypes.h>
#define ANGLE_NUM 18

//#ifdef __cplusplus
//extern "C" {
//#endif
	void JointsPositionToAngel(k4abt_skeleton_t skeleton0, float(*Angel)[ANGLE_NUM]);
//#ifdef __cplusplus
//}
//#endif