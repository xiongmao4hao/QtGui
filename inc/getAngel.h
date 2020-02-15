#pragma once

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "kinect_angle.h"


//注释部分展示了如果不是.cpp而是.c时为了避免出错的声明（声明之后用c的编译方式）。
//#ifdef __cplusplus
//extern "C" {
//#endif
	void JointsPositionToAngel(k4abt_skeleton_t skeleton0, float(*Angel)[ANGLE_NUM]);
//#ifdef __cplusplus
//}
//#endif