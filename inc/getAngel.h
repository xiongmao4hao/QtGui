#pragma once
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<k4abttypes.h>

//#ifdef __cplusplus
//extern "C" {
//#endif
	float GetAngel(float ax, float ay, float az, float bx, float by, float bz, float cx, float cy, float cz);
	void JointsPositionToAngel(k4abt_skeleton_t skeleton0, float(*Angel)[12]);
//#ifdef __cplusplus
//}
//#endif