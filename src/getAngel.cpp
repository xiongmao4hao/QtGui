#include"getAngel.h"
#define PI 3.1415926
#include<iostream>
using namespace std;
//k4abt_skeleton_t skeleton0;

float GetAngel(float ax, float ay, float az, float bx, float by, float bz, float cx, float cy, float cz)
{
	float A, B, C, A2, B2, C2, pos, posAngel, Angle;
	C2 = (ax - bx) * (ax - bx) + (ay - by) * (ay - by) + (az - bz) * (az - bz);
	B2 = (ax - cx) * (ax - cx) + (ay - cy) * (ay - cy) + (az - cz) * (az - cz);
	A2 = (bx - cx) * (bx - cx) + (by - cy) * (by - cy) + (bz - cz) * (bz - cz);
	C = sqrt(C2);
	B = sqrt(B2);
	A = sqrt(A2);
	pos = (A2 + C2 - B2) / (2 * A * C);
	posAngel = acos(pos);
	Angle = (posAngel * 180) / PI;

	return Angle;
}

//水平面
float Get_Angel_xz(k4abt_skeleton_t skeleton0, int hip, int knee)
{

	float Angel_xz = 0;
	float Direction_vector[3] = { (skeleton0.joints[knee].position.xyz.x - skeleton0.joints[hip].position.xyz.x),(skeleton0.joints[knee].position.xyz.y - skeleton0.joints[hip].position.xyz.y),(skeleton0.joints[knee].position.xyz.z - skeleton0.joints[hip].position.xyz.z) };
	//下正 上负
	float Normal_vector[3] = { 0,1,0 };
	float pos = (Direction_vector[0] * Normal_vector[0] + Direction_vector[1] * Normal_vector[1] + Direction_vector[2] * Normal_vector[2])
		/ (sqrt((Direction_vector[0] * Direction_vector[0]) + (Direction_vector[1] * Direction_vector[1]) + (Direction_vector[2] * Direction_vector[2]))
			* sqrt((Normal_vector[0] * Normal_vector[0]) + (Normal_vector[1] * Normal_vector[1]) + (Normal_vector[2] * Normal_vector[2])));
	float Angel = acos(pos);
	Angel_xz = 90 - (Angel * 180) / PI;

	return Angel_xz;
}

// 失状面
float Get_Angel_yz(k4abt_skeleton_t skeleton0, int hip, int knee)
{

	float Angel_yz = 0;
	float Direction_vector[3] = { (skeleton0.joints[knee].position.xyz.x - skeleton0.joints[hip].position.xyz.x),(skeleton0.joints[knee].position.xyz.y - skeleton0.joints[hip].position.xyz.y),(skeleton0.joints[knee].position.xyz.z - skeleton0.joints[hip].position.xyz.z) };
	//右正左负
	float Normal_vector[3] = { 1,0,0 };
	float pos = (Direction_vector[0] * Normal_vector[0] + Direction_vector[1] * Normal_vector[1] + Direction_vector[2] * Normal_vector[2])
		/ (sqrt((Direction_vector[0] * Direction_vector[0]) + (Direction_vector[1] * Direction_vector[1]) + (Direction_vector[2] * Direction_vector[2]))
			* sqrt((Normal_vector[0] * Normal_vector[0]) + (Normal_vector[1] * Normal_vector[1]) + (Normal_vector[2] * Normal_vector[2])));
	float Angel = acos(pos);
	Angel_yz = 90 - (Angel * 180) / PI;

	return Angel_yz;
}


//冠状面
float Get_Angel_xy(k4abt_skeleton_t skeleton0, int hip, int knee)
{

	float Angel_xy = 0;
	float Direction_vector[3] = { (skeleton0.joints[knee].position.xyz.x - skeleton0.joints[hip].position.xyz.x),(skeleton0.joints[knee].position.xyz.y - skeleton0.joints[hip].position.xyz.y),(skeleton0.joints[knee].position.xyz.z - skeleton0.joints[hip].position.xyz.z) };
	//前正后负
	float Normal_vector[3] = { 0,0,1 };
	float pos = (Direction_vector[0] * Normal_vector[0] + Direction_vector[1] * Normal_vector[1] + Direction_vector[2] * Normal_vector[2])
		/ (sqrt((Direction_vector[0] * Direction_vector[0]) + (Direction_vector[1] * Direction_vector[1]) + (Direction_vector[2] * Direction_vector[2]))
			* sqrt((Normal_vector[0] * Normal_vector[0]) + (Normal_vector[1] * Normal_vector[1]) + (Normal_vector[2] * Normal_vector[2])));
	float Angel = acos(pos);
	Angel_xy = 90 - (Angel * 180) / PI;

	return Angel_xy;
}
//肩部
float Get_Angel_Shoulder(k4abt_skeleton_t skeleton0, int shoulder, int elbow, int spine_chest, int spine_naval) {
	float Angel_shoulder = 0;
	float Direction_vector[3] = { (skeleton0.joints[elbow].position.xyz.x - skeleton0.joints[shoulder].position.xyz.x),(skeleton0.joints[elbow].position.xyz.y - skeleton0.joints[shoulder].position.xyz.y),(skeleton0.joints[elbow].position.xyz.z - skeleton0.joints[shoulder].position.xyz.z) };
	float Normal_vector[3] = { (skeleton0.joints[spine_naval].position.xyz.x - skeleton0.joints[spine_chest].position.xyz.x),(skeleton0.joints[spine_naval].position.xyz.y - skeleton0.joints[spine_chest].position.xyz.y),(skeleton0.joints[spine_naval].position.xyz.z - skeleton0.joints[spine_chest].position.xyz.z) };
	float pos = (Direction_vector[0] * Normal_vector[0] + Direction_vector[1] * Normal_vector[1] + Direction_vector[2] * Normal_vector[2])
		/ (sqrt((Direction_vector[0] * Direction_vector[0]) + (Direction_vector[1] * Direction_vector[1]) + (Direction_vector[2] * Direction_vector[2]))
			* sqrt((Normal_vector[0] * Normal_vector[0]) + (Normal_vector[1] * Normal_vector[1]) + (Normal_vector[2] * Normal_vector[2])));
	float Angel = acos(pos);
	Angel_shoulder =  (Angel * 180) / PI;
	return Angel_shoulder;
}
void JointsPositionToAngel(k4abt_skeleton_t skeleton0, float(*Angel)[ANGLE_NUM])
{
	//"[]"优先级低于"()"高于"*"
	//0-11 左腕、左肘、左肩、右腕、右肘、右肩、左膝、左踝、右膝、右踝、脖子、脊柱
	//肩部在上面写出
	(*Angel)[0] = GetAngel(skeleton0.joints[8].position.xyz.x, skeleton0.joints[8].position.xyz.y, skeleton0.joints[8].position.xyz.z, skeleton0.joints[7].position.xyz.x, skeleton0.joints[7].position.xyz.y, skeleton0.joints[7].position.xyz.z, skeleton0.joints[6].position.xyz.x, skeleton0.joints[6].position.xyz.y, skeleton0.joints[6].position.xyz.z);
	(*Angel)[1] = GetAngel(skeleton0.joints[7].position.xyz.x, skeleton0.joints[7].position.xyz.y, skeleton0.joints[7].position.xyz.z, skeleton0.joints[6].position.xyz.x, skeleton0.joints[6].position.xyz.y, skeleton0.joints[6].position.xyz.z, skeleton0.joints[5].position.xyz.x, skeleton0.joints[5].position.xyz.y, skeleton0.joints[5].position.xyz.z);
	(*Angel)[2] = Get_Angel_Shoulder(skeleton0, 5, 6, 2, 1);
	(*Angel)[3] = GetAngel(skeleton0.joints[15].position.xyz.x, skeleton0.joints[15].position.xyz.y, skeleton0.joints[15].position.xyz.z, skeleton0.joints[14].position.xyz.x, skeleton0.joints[14].position.xyz.y, skeleton0.joints[14].position.xyz.z, skeleton0.joints[13].position.xyz.x, skeleton0.joints[13].position.xyz.y, skeleton0.joints[13].position.xyz.z);
	(*Angel)[4] = GetAngel(skeleton0.joints[14].position.xyz.x, skeleton0.joints[14].position.xyz.y, skeleton0.joints[14].position.xyz.z, skeleton0.joints[13].position.xyz.x, skeleton0.joints[13].position.xyz.y, skeleton0.joints[13].position.xyz.z, skeleton0.joints[12].position.xyz.x, skeleton0.joints[12].position.xyz.y, skeleton0.joints[12].position.xyz.z);
	(*Angel)[5] = Get_Angel_Shoulder(skeleton0, 12, 13, 2, 1);
	(*Angel)[6] = GetAngel(skeleton0.joints[18].position.xyz.x, skeleton0.joints[18].position.xyz.y, skeleton0.joints[18].position.xyz.z, skeleton0.joints[19].position.xyz.x, skeleton0.joints[19].position.xyz.y, skeleton0.joints[19].position.xyz.z, skeleton0.joints[20].position.xyz.x, skeleton0.joints[20].position.xyz.y, skeleton0.joints[20].position.xyz.z);
	(*Angel)[7] = GetAngel(skeleton0.joints[19].position.xyz.x, skeleton0.joints[19].position.xyz.y, skeleton0.joints[19].position.xyz.z, skeleton0.joints[20].position.xyz.x, skeleton0.joints[20].position.xyz.y, skeleton0.joints[20].position.xyz.z, skeleton0.joints[21].position.xyz.x, skeleton0.joints[21].position.xyz.y, skeleton0.joints[21].position.xyz.z);
	(*Angel)[8] = GetAngel(skeleton0.joints[22].position.xyz.x, skeleton0.joints[22].position.xyz.y, skeleton0.joints[22].position.xyz.z, skeleton0.joints[23].position.xyz.x, skeleton0.joints[23].position.xyz.y, skeleton0.joints[23].position.xyz.z, skeleton0.joints[24].position.xyz.x, skeleton0.joints[24].position.xyz.y, skeleton0.joints[24].position.xyz.z);
	(*Angel)[9] = GetAngel(skeleton0.joints[23].position.xyz.x, skeleton0.joints[23].position.xyz.y, skeleton0.joints[23].position.xyz.z, skeleton0.joints[24].position.xyz.x, skeleton0.joints[24].position.xyz.y, skeleton0.joints[24].position.xyz.z, skeleton0.joints[25].position.xyz.x, skeleton0.joints[25].position.xyz.y, skeleton0.joints[25].position.xyz.z);
	(*Angel)[10] = GetAngel(skeleton0.joints[26].position.xyz.x, skeleton0.joints[26].position.xyz.y, skeleton0.joints[26].position.xyz.z, skeleton0.joints[3].position.xyz.x, skeleton0.joints[3].position.xyz.y, skeleton0.joints[3].position.xyz.z, skeleton0.joints[2].position.xyz.x, skeleton0.joints[2].position.xyz.y, skeleton0.joints[2].position.xyz.z);
	(*Angel)[11] = GetAngel(skeleton0.joints[2].position.xyz.x, skeleton0.joints[2].position.xyz.y, skeleton0.joints[2].position.xyz.z, skeleton0.joints[1].position.xyz.x, skeleton0.joints[1].position.xyz.y, skeleton0.joints[1].position.xyz.z, skeleton0.joints[0].position.xyz.x, skeleton0.joints[0].position.xyz.y, skeleton0.joints[0].position.xyz.z);
	//左髋关节
	(*Angel)[12] = Get_Angel_xz(skeleton0, 18, 19);
	(*Angel)[13] = Get_Angel_yz(skeleton0, 18, 19);
	(*Angel)[14] = Get_Angel_xy(skeleton0, 18, 19);
	//右髋关节
	(*Angel)[15] = Get_Angel_xz(skeleton0, 22, 23);
	(*Angel)[16] = Get_Angel_yz(skeleton0, 22, 23);
	(*Angel)[17] = Get_Angel_xy(skeleton0, 22, 23);

}