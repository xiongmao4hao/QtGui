#pragma once
#ifndef  _MYJOINTS_H
#define  _MYJOINTS_H

#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <k4abt.hpp>
#include <k4a/k4a.hpp>
#include <GL/freeglut.h>
#include <gl/gl.h>
#include <gl/GLU.h>

using namespace std;

class MyJoints {
public:
	void getData(k4abt_skeleton_t skeleton)
	{
		for (int j = 0; j < 26; j++)
		{
			joints[j].xyz.x = skeleton.joints[j].position.xyz.x;
			joints[j].xyz.y = skeleton.joints[j].position.xyz.y;
			joints[j].xyz.z = skeleton.joints[j].position.xyz.z;
			//cout << "x: " << joints[j].xyz.x << " y: " << joints[j].xyz.y << endl;
		}
	}

	void joint_lookat()
	{
		gluLookAt(eye[0], eye[1], eye[2],
			center[0], center[1], center[2],
			0, 1, 0);
	}

	void joint_rotate()
	{
		glRotatef(xrotate, 1.0, 0.0, 0.0);
		glRotatef(yrotate, 0.0, 1.0, 0.0);
	}

	void joint_key(unsigned char c, int x, int y)
	{
		switch (c)
		{
		case 'w':
			eye[2] += 20.0f;
			break;
		case 's':
			eye[2] -= 20.0f;
			break;
		case 'a':
			eye[0] += 20.0f;
			break;
		case 'd':
			eye[0] -= 20.0f;
			break;
		case 'r':
			eye[0] = 0.0f;
			eye[2] = 0.0f;
			xrotate = 0;
			yrotate = 0;
			break;
			//case 27:
			//	exit(0);
		default:
			break;
		}
		glutPostRedisplay();
	}

	void joint_mouse(int button, int state, int x, int y)
	{
		if (state == GLUT_DOWN)
		{
			mousedown = GL_TRUE;
		}
		mousex = x, mousey = y;
	}

	void joint_motion(int x, int y)
	{
		if (mousedown == GL_TRUE)
		{       /// �����Ե������ǵ�����ת�ٶȵģ�������ã��ﵽ�Լ���Ҫ�ٶȼ���
			xrotate -= (x - mousex) / 10.0f;
			yrotate -= (y - mousey) / 10.0f;
		}
		mousex = x, mousey = y;
		glutPostRedisplay();
	}

	void setFlag()
	{
		flag = 1;
	}

	void resetFlag()
	{
		flag = 0;
	}

	void RenderBone(float x0, float y0, float z0, float x1, float y1, float z1)
	{
		GLdouble  dir_x = x1 - x0;
		GLdouble  dir_y = y1 - y0;
		GLdouble  dir_z = z1 - z0;
		GLdouble  bone_length = sqrt(dir_x*dir_x + dir_y * dir_y + dir_z * dir_z);
		static GLUquadricObj *  quad_obj = NULL;
		if (quad_obj == NULL)
			quad_obj = gluNewQuadric();
		gluQuadricDrawStyle(quad_obj, GLU_FILL);
		gluQuadricNormals(quad_obj, GLU_SMOOTH);
		glPushMatrix();
		// ƽ�Ƶ���ʼ��
		glTranslated(x0, y0, z0);
		// ���㳤��
		double  length;
		length = sqrt(dir_x*dir_x + dir_y * dir_y + dir_z * dir_z);
		if (length < 0.0001) {
			dir_x = 0.0; dir_y = 0.0; dir_z = 1.0;  length = 1.0;
		}
		dir_x /= length;  dir_y /= length;  dir_z /= length;
		GLdouble  up_x, up_y, up_z;
		up_x = 0.0;
		up_y = 1.0;
		up_z = 0.0;
		double  side_x, side_y, side_z;
		side_x = up_y * dir_z - up_z * dir_y;
		side_y = up_z * dir_x - up_x * dir_z;
		side_z = up_x * dir_y - up_y * dir_x;
		length = sqrt(side_x*side_x + side_y * side_y + side_z * side_z);
		if (length < 0.0001) {
			side_x = 1.0; side_y = 0.0; side_z = 0.0;  length = 1.0;
		}
		side_x /= length;  side_y /= length;  side_z /= length;
		up_x = dir_y * side_z - dir_z * side_y;
		up_y = dir_z * side_x - dir_x * side_z;
		up_z = dir_x * side_y - dir_y * side_x;
		// ����任����
		GLdouble  m[16] = { side_x, side_y, side_z, 0.0,
			up_x,   up_y,   up_z,   0.0,
			dir_x,  dir_y,  dir_z,  0.0,
			0.0,    0.0,    0.0,    1.0 };
		glMultMatrixd(m);
		// Բ�������
		GLdouble radius = 1;		// �뾶
		GLdouble slices = 8.0;		//	����
		GLdouble stack = 3.0;		// �ݹ����
		gluCylinder(quad_obj, radius, radius, bone_length, slices, stack);
		glPopMatrix();
	}

	void drawskeleton()
	{
		clock_t start, stop;
		double duration;
		if (flag == 1)
		{

		/*ofstream dataFile;
		dataFile.open("D:\\Users\\Desktop\\dataFile.txt", ofstream::app);
		for (int i = 0; i < 25; i++) {
			dataFile << -(joints[i].xyz.x) / 10.f << ' ' << -(joints[i].xyz.y) / 10.f << ' ' << (joints[i].xyz.z) / 10.f << ' ';
		}
		dataFile << -(joints[25].xyz.x) / 10.f << ' ' << -(joints[25].xyz.y) / 10.f << ' ' << (joints[25].xyz.z) / 10.f << endl;
		dataFile.close();*/

			start = clock();
			cout << "thread:" << start << " x:" << joints[2].xyz.x << " y:" << joints[2].xyz.y << endl;

			glPushMatrix();
			joint_rotate();
			// glColor4f(1.0, 0.0, 0.0, 1.0);
			RenderBone(-(joints[20].xyz.x) / 10.f, -(joints[20].xyz.y) / 10.f, (joints[20].xyz.z) / 10.f, -(joints[3].xyz.x) / 10.f, -(joints[3].xyz.y) / 10.f, (joints[3].xyz.z) / 10.f);
			glPopMatrix();

			glPushMatrix();
			joint_rotate();
			// glColor4f(1.0, 0.0, 0.0, 1.0);
			RenderBone(-(joints[3].xyz.x) / 10.f, -(joints[3].xyz.y) / 10.f, (joints[3].xyz.z) / 10.f, -(joints[2].xyz.x) / 10.f, -(joints[2].xyz.y) / 10.f, (joints[2].xyz.z) / 10.f);
			glPopMatrix();

			glPushMatrix();
			joint_rotate();
			// glColor4f(1.0, 0.0, 0.0, 1.0);
			RenderBone(-(joints[2].xyz.x) / 10.f, -(joints[2].xyz.y) / 10.f, (joints[2].xyz.z) / 10.f, -(joints[8].xyz.x) / 10.f, -(joints[8].xyz.y) / 10.f, (joints[8].xyz.z) / 10.f);
			glPopMatrix();

			glPushMatrix();
			joint_rotate();
			// glColor4f(1.0, 0.0, 0.0, 1.0);
			RenderBone(-(joints[8].xyz.x) / 10.f, -(joints[8].xyz.y) / 10.f, (joints[8].xyz.z) / 10.f, -(joints[9].xyz.x) / 10.f, -(joints[9].xyz.y) / 10.f, (joints[9].xyz.z) / 10.f);
			glPopMatrix();

			glPushMatrix();
			joint_rotate();
			// glColor4f(1.0, 0.0, 0.0, 1.0);
			RenderBone(-(joints[9].xyz.x) / 10.f, -(joints[9].xyz.y) / 10.f, (joints[9].xyz.z) / 10.f, -(joints[10].xyz.x) / 10.f, -(joints[10].xyz.y) / 10.f, (joints[10].xyz.z) / 10.f);
			glPopMatrix();

			glPushMatrix();
			joint_rotate();
			// glColor4f(1.0, 0.0, 0.0, 1.0);
			RenderBone(-(joints[10].xyz.x) / 10.f, -(joints[10].xyz.y) / 10.f, (joints[10].xyz.z) / 10.f, -(joints[11].xyz.x) / 10.f, -(joints[11].xyz.y) / 10.f, (joints[11].xyz.z) / 10.f);
			glPopMatrix();

			glPushMatrix();
			joint_rotate();
			// glColor4f(1.0, 0.0, 0.0, 1.0);
			RenderBone(-(joints[2].xyz.x) / 10.f, -(joints[2].xyz.y) / 10.f, (joints[2].xyz.z) / 10.f, -(joints[4].xyz.x) / 10.f, -(joints[4].xyz.y) / 10.f, (joints[4].xyz.z) / 10.f);
			glPopMatrix();

			glPushMatrix();
			joint_rotate();
			// glColor4f(1.0, 0.0, 0.0, 1.0);
			RenderBone(-(joints[4].xyz.x) / 10.f, -(joints[4].xyz.y) / 10.f, (joints[4].xyz.z) / 10.f, -(joints[5].xyz.x) / 10.f, -(joints[5].xyz.y) / 10.f, (joints[5].xyz.z) / 10.f);
			glPopMatrix();

			glPushMatrix();
			joint_rotate();
			// glColor4f(1.0, 0.0, 0.0, 1.0);
			RenderBone(-(joints[5].xyz.x) / 10.f, -(joints[5].xyz.y) / 10.f, (joints[5].xyz.z) / 10.f, -(joints[6].xyz.x) / 10.f, -(joints[6].xyz.y) / 10.f, (joints[6].xyz.z) / 10.f);
			glPopMatrix();

			glPushMatrix();
			joint_rotate();
			// glColor4f(1.0, 0.0, 0.0, 1.0);
			RenderBone(-(joints[6].xyz.x) / 10.f, -(joints[6].xyz.y) / 10.f, (joints[6].xyz.z) / 10.f, -(joints[7].xyz.x) / 10.f, -(joints[7].xyz.y) / 10.f, (joints[7].xyz.z) / 10.f);
			glPopMatrix();

			glPushMatrix();
			joint_rotate();
			// glColor4f(1.0, 0.0, 0.0, 1.0);
			RenderBone(-(joints[2].xyz.x) / 10.f, -(joints[2].xyz.y) / 10.f, (joints[2].xyz.z) / 10.f, -(joints[1].xyz.x) / 10.f, -(joints[1].xyz.y) / 10.f, (joints[1].xyz.z) / 10.f);
			glPopMatrix();

			glPushMatrix();
			joint_rotate();
			// glColor4f(1.0, 0.0, 0.0, 1.0);
			RenderBone(-(joints[1].xyz.x) / 10.f, -(joints[1].xyz.y) / 10.f, (joints[1].xyz.z) / 10.f, -(joints[0].xyz.x) / 10.f, -(joints[0].xyz.y) / 10.f, (joints[0].xyz.z) / 10.f);
			glPopMatrix();

			glPushMatrix();
			joint_rotate();
			// glColor4f(1.0, 0.0, 0.0, 1.0);
			RenderBone(-(joints[0].xyz.x) / 10.f, -(joints[0].xyz.y) / 10.f, (joints[0].xyz.z) / 10.f, -(joints[12].xyz.x) / 10.f, -(joints[12].xyz.y) / 10.f, (joints[12].xyz.z) / 10.f);
			glPopMatrix();

			glPushMatrix();
			joint_rotate();
			// glColor4f(1.0, 0.0, 0.0, 1.0);
			RenderBone(-(joints[12].xyz.x) / 10.f, -(joints[12].xyz.y) / 10.f, (joints[12].xyz.z) / 10.f, -(joints[13].xyz.x) / 10.f, -(joints[13].xyz.y) / 10.f, (joints[13].xyz.z) / 10.f);
			glPopMatrix();

			glPushMatrix();
			joint_rotate();
			// glColor4f(1.0, 0.0, 0.0, 1.0);
			RenderBone(-(joints[13].xyz.x) / 10.f, -(joints[13].xyz.y) / 10.f, (joints[13].xyz.z) / 10.f, -(joints[14].xyz.x) / 10.f, -(joints[14].xyz.y) / 10.f, (joints[14].xyz.z) / 10.f);
			glPopMatrix();

			glPushMatrix();
			joint_rotate();
			// glColor4f(1.0, 0.0, 0.0, 1.0);
			RenderBone(-(joints[14].xyz.x) / 10.f, -(joints[14].xyz.y) / 10.f, (joints[14].xyz.z) / 10.f, -(joints[15].xyz.x) / 10.f, -(joints[15].xyz.y) / 10.f, (joints[15].xyz.z) / 10.f);
			glPopMatrix();

			glPushMatrix();
			joint_rotate();
			// glColor4f(1.0, 0.0, 0.0, 1.0);
			RenderBone(-(joints[0].xyz.x) / 10.f, -(joints[0].xyz.y) / 10.f, (joints[0].xyz.z) / 10.f, -(joints[16].xyz.x) / 10.f, -(joints[16].xyz.y) / 10.f, (joints[16].xyz.z) / 10.f);
			glPopMatrix();

			glPushMatrix();
			joint_rotate();
			// glColor4f(1.0, 0.0, 0.0, 1.0);
			RenderBone(-(joints[16].xyz.x) / 10.f, -(joints[16].xyz.y) / 10.f, (joints[16].xyz.z) / 10.f, -(joints[17].xyz.x) / 10.f, -(joints[17].xyz.y) / 10.f, (joints[17].xyz.z) / 10.f);
			glPopMatrix();

			glPushMatrix();
			joint_rotate();
			// glColor4f(1.0, 0.0, 0.0, 1.0);
			RenderBone(-(joints[17].xyz.x) / 10.f, -(joints[17].xyz.y) / 10.f, (joints[17].xyz.z) / 10.f, -(joints[18].xyz.x) / 10.f, -(joints[18].xyz.y) / 10.f, (joints[18].xyz.z) / 10.f);
			glPopMatrix();

			glPushMatrix();
			joint_rotate();
			// glColor4f(1.0, 0.0, 0.0, 1.0);
			RenderBone(-(joints[18].xyz.x) / 10.f, -(joints[18].xyz.y) / 10.f, (joints[18].xyz.z) / 10.f, -(joints[19].xyz.x) / 10.f, -(joints[19].xyz.y) / 10.f, (joints[19].xyz.z) / 10.f);
			glPopMatrix();

			glPushMatrix();
			joint_rotate();
			// glColor4f(1.0, 0.0, 0.0, 1.0);
			RenderBone(-(joints[20].xyz.x) / 10.f, -(joints[20].xyz.y) / 10.f, (joints[20].xyz.z) / 10.f, -(joints[3].xyz.x) / 10.f, -(joints[3].xyz.y) / 10.f, (joints[3].xyz.z) / 10.f);
			glPopMatrix();

			glPushMatrix();
			joint_rotate();
			// glColor4f(1.0, 0.0, 0.0, 1.0);
			RenderBone(-(joints[20].xyz.x) / 10.f, -(joints[20].xyz.y) / 10.f, (joints[20].xyz.z) / 10.f, -(joints[21].xyz.x) / 10.f, -(joints[21].xyz.y) / 10.f, (joints[21].xyz.z) / 10.f);
			glPopMatrix();

			glPushMatrix();
			joint_rotate();
			// glColor4f(1.0, 0.0, 0.0, 1.0);
			RenderBone(-(joints[21].xyz.x) / 10.f, -(joints[21].xyz.y) / 10.f, (joints[21].xyz.z) / 10.f, -(joints[22].xyz.x) / 10.f, -(joints[22].xyz.y) / 10.f, (joints[22].xyz.z) / 10.f);
			glPopMatrix();

			glPushMatrix();
			joint_rotate();
			// glColor4f(1.0, 0.0, 0.0, 1.0);
			RenderBone(-(joints[22].xyz.x) / 10.f, -(joints[22].xyz.y) / 10.f, (joints[22].xyz.z) / 10.f, -(joints[23].xyz.x) / 10.f, -(joints[23].xyz.y) / 10.f, (joints[23].xyz.z) / 10.f);
			glPopMatrix();

			glPushMatrix();
			joint_rotate();
			// glColor4f(1.0, 0.0, 0.0, 1.0);
			RenderBone(-(joints[21].xyz.x) / 10.f, -(joints[21].xyz.y) / 10.f, (joints[21].xyz.z) / 10.f, -(joints[24].xyz.x) / 10.f, -(joints[24].xyz.y) / 10.f, (joints[24].xyz.z) / 10.f);
			glPopMatrix();

			glPushMatrix();
			joint_rotate();
			// glColor4f(1.0, 0.0, 0.0, 1.0);
			RenderBone(-(joints[24].xyz.x) / 10.f, -(joints[24].xyz.y) / 10.f, (joints[24].xyz.z) / 10.f, -(joints[25].xyz.x) / 10.f, -(joints[25].xyz.y) / 10.f, (joints[25].xyz.z) / 10.f);
			glPopMatrix();



			//glPushMatrix();
			//joint_rotate();
			//// glColor4f(1.0, 0.0, 0.0, 1.0);
			//RenderBone(-3.67846, 54.6998, 156.505, -3.72324, 47.1217, 158.278);
			//glPopMatrix();

			//glPushMatrix();
			//joint_rotate();
			//// glColor4f(1.0, 0.0, 0.0, 1.0);
			//RenderBone(-3.72324, 47.1217, 158.278, -3.19965, 26.7036, 155.006);
			//glPopMatrix();

			//glPushMatrix();
			//joint_rotate();
			//// glColor4f(1.0, 0.0, 0.0, 1.0);
			//RenderBone(-3.19965, 26.7036, 155.006, -0.938763, 43.6405, 158.939);
			//glPopMatrix();

			//glPushMatrix();
			//joint_rotate();
			//// glColor4f(1.0, 0.0, 0.0, 1.0);
			//RenderBone(-0.938763, 43.6405, 158.939, 11.5671, 43.7773, 159.703);
			//glPopMatrix();

			//glPushMatrix();
			//joint_rotate();
			//// glColor4f(1.0, 0.0, 0.0, 1.0);
			//RenderBone(11.5671, 43.7773, 159.703, 33.573, 28.4679, 158.671);
			//glPopMatrix();

			//glPushMatrix();
			//joint_rotate();
			//// glColor4f(1.0, 0.0, 0.0, 1.0);
			//RenderBone(33.573, 28.4679, 158.671, 52.8012, 20.981, 150.011);
			//glPopMatrix();

			//glPushMatrix();
			//joint_rotate();
			//// glColor4f(1.0, 0.0, 0.0, 1.0);
			//RenderBone(-3.19965, 26.7036, 155.006, -5.80177, 43.7011, 158.718);
			//glPopMatrix();

			//glPushMatrix();
			//joint_rotate();
			//// glColor4f(1.0, 0.0, 0.0, 1.0);
			//RenderBone(-5.80177, 43.7011, 158.718, -19.3679, 44.9117, 158.758);
			//glPopMatrix();

			//glPushMatrix();
			//joint_rotate();
			//// glColor4f(1.0, 0.0, 0.0, 1.0);
			//RenderBone(-19.3679, 44.9117, 158.758, -39.9785, 28.54, 158.13);
			//glPopMatrix();

			//glPushMatrix();
			//joint_rotate();
			//// glColor4f(1.0, 0.0, 0.0, 1.0);
			//RenderBone(-39.9785, 28.54, 158.13, -58.512, 18.0099, 152.303);
			//glPopMatrix();

			//glPushMatrix();
			//joint_rotate();
			//// glColor4f(1.0, 0.0, 0.0, 1.0);
			//RenderBone(-3.19965, 26.7036, 155.006, -3.23406, 13.1468, 154.071);
			//glPopMatrix();

			//glPushMatrix();
			//joint_rotate();
			//// glColor4f(1.0, 0.0, 0.0, 1.0);
			//RenderBone(-3.23406, 13.1468, 154.071, -3.79016, -3.8321, 153.487);
			//glPopMatrix();

			//glPushMatrix();
			//joint_rotate();
			//// glColor4f(1.0, 0.0, 0.0, 1.0);
			//RenderBone(-3.79016, -3.8321, 153.487, -12.4671, -3.53715, 153.174);
			//glPopMatrix();

			//glPushMatrix();
			//joint_rotate();
			//// glColor4f(1.0, 0.0, 0.0, 1.0);
			//RenderBone(-12.4671, -3.53715, 153.174, -13.8667, -41.4571, 149.301);
			//glPopMatrix();

			//glPushMatrix();
			//joint_rotate();
			//// glColor4f(1.0, 0.0, 0.0, 1.0);
			//RenderBone(-13.8667, -41.4571, 149.301, -16.1053, -76.7182, 158.27);
			//glPopMatrix();

			//glPushMatrix();
			//joint_rotate();
			//// glColor4f(1.0, 0.0, 0.0, 1.0);
			//RenderBone(-16.1053, -76.7182, 158.27, -20.5413, -87.3472, 144.634);
			//glPopMatrix();

			//glPushMatrix();
			//joint_rotate();
			//// glColor4f(1.0, 0.0, 0.0, 1.0);
			//RenderBone(-3.79016, -3.8321, 153.487, 4.03419, -4.09838, 153.768);
			//glPopMatrix();

			//glPushMatrix();
			//joint_rotate();
			//// glColor4f(1.0, 0.0, 0.0, 1.0);
			//RenderBone(4.03419, -4.09838, 153.768, 4.1133, -42.1492, 151.801);
			//glPopMatrix();

			//glPushMatrix();
			//joint_rotate();
			//// glColor4f(1.0, 0.0, 0.0, 1.0);
			//RenderBone(4.1133, -42.1492, 151.801, 2.69245, -77.5196, 161.99);
			//glPopMatrix();

			//glPushMatrix();
			//joint_rotate();
			//// glColor4f(1.0, 0.0, 0.0, 1.0);
			//RenderBone(2.69245, -77.5196, 161.99, 6.96773, -88.6183, 150.666);
			//glPopMatrix();

			//glPushMatrix();
			//joint_rotate();
			//// glColor4f(1.0, 0.0, 0.0, 1.0);
			//RenderBone(-3.67846, 54.6998, 156.505, -3.72324, 47.1217, 158.278);
			//glPopMatrix();

			//glPushMatrix();
			//joint_rotate();
			//// glColor4f(1.0, 0.0, 0.0, 1.0);
			//RenderBone(-3.67846, 54.6998, 156.505, -2.36372, 65.3475, 145.583);
			//glPopMatrix();

			//glPushMatrix();
			//joint_rotate();
			//// glColor4f(1.0, 0.0, 0.0, 1.0);
			//RenderBone(-2.36372, 65.3475, 145.583, -6.23141, 66.7193, 148.988);
			//glPopMatrix();

			//glPushMatrix();
			//joint_rotate();
			//// glColor4f(1.0, 0.0, 0.0, 1.0);
			//RenderBone(-6.23141, 66.7193, 148.988, -12.7858, 61.312, 157.573);
			//glPopMatrix();

			//glPushMatrix();
			//joint_rotate();
			//// glColor4f(1.0, 0.0, 0.0, 1.0);
			//RenderBone(-2.36372, 65.3475, 145.583, 0.384706, 66.765, 149.422);
			//glPopMatrix();

			//glPushMatrix();
			//joint_rotate();
			//// glColor4f(1.0, 0.0, 0.0, 1.0);
			//RenderBone(0.384706, 66.765, 149.422, 5.24261, 61.1097, 159.434);
			//glPopMatrix();

			stop = clock();
			duration = (double)(stop - start) / CLOCKS_PER_SEC;
			//cout << "start:" << start << " stop:" << stop << " duration:" << duration << endl;

			glFlush();
		}
	}

private:
	static k4a_float3_t joints[26];
	static int flag;
	static GLfloat eye[3]; /// eye's position
	static GLfloat center[3]; /// center position
	static GLfloat yrotate; /// angle between y-axis and look direction
	static GLfloat xrotate; /// angle between x-axis and look direction
	/// record the state of mouse
	GLboolean mousedown = GL_FALSE;
	/// when a mouse-key is pressed, record current mouse position 
	static GLint mousex, mousey;
};

k4a_float3_t MyJoints::joints[26];
int MyJoints::flag = 0;
GLfloat MyJoints::eye[3] = { 0.0f, 0.0f, 0.0f }; /// eye's position
GLfloat MyJoints::center[3] = { 0.0f, 0.0f, 5000.0f }; /// center position
GLfloat MyJoints::yrotate = 0; /// angle between y-axis and look direction
GLfloat MyJoints::xrotate = 0; /// angle between x-axis and look direction
GLint MyJoints::mousex = 0, MyJoints::mousey = 0;

#endif // ! _MYJOINTS_H
