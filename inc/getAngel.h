#pragma once

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "kinect_angle.h"


//ע�Ͳ���չʾ���������.cpp����.cʱΪ�˱�����������������֮����c�ı��뷽ʽ����
//#ifdef __cplusplus
//extern "C" {
//#endif
	void JointsPositionToAngel(k4abt_skeleton_t skeleton0, float(*Angel)[ANGLE_NUM]);
//#ifdef __cplusplus
//}
//#endif