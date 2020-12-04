#include "Render.h"

#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include <cmath>

const double PI = 3.14159265358979323846;


double bezier(double p1, double p2, double p3, double p4, double t) {
	return pow(1 - t, 3) * p1 + 3 * t * pow(1 - t, 2) * p2 + 3 * pow(t, 2) * (1 - t) * p3 + pow(t, 3) * p4;
}


double ermit(double p1, double p4, double r1, double r4, double t) {
	return p1 * (2 * pow(t, 3) - 3 * pow(t, 2) + 1) + p4 * (-2 * pow(t, 3) + 3 * pow(t, 2)) + r1 * (pow(t, 3) - 2 * pow(t, 2) + t) + r4 * (pow(t, 3) - pow(t, 2));
}


void cube() {
	glPushMatrix();
	glRotated(90, 0, 1, 0);

	glBegin(GL_POLYGON);

	glColor3d(1.0, 0.0, 0.0);     glVertex3d(0.5, -0.5, -0.5);     
	glColor3d(0.0, 1.0, 0.0);     glVertex3d(0.5, 0.5, -0.5);      
	glColor3d(0.0, 0.0, 1.0);     glVertex3d(-0.5, 0.5, -0.5);      
	glColor3d(1.0, 0.0, 1.0);     glVertex3d(-0.5, -0.5, -0.5);    

	glEnd();

	
	glBegin(GL_POLYGON);
	glVertex3d(0.5, -0.5, 0.5);
	glVertex3d(0.5, 0.5, 0.5);
	glVertex3d(-0.5, 0.5, 0.5);
	glVertex3d(-0.5, -0.5, 0.5);
	glEnd();


	glBegin(GL_POLYGON);
	glColor3d(1.0, 0.0, 1.0);
	glVertex3d(0.5, -0.5, -0.5);
	glVertex3d(0.5, 0.5, -0.5);
	glVertex3d(0.5, 0.5, 0.5);
	glVertex3d(0.5, -0.5, 0.5);
	glEnd();


	glBegin(GL_POLYGON);
	glColor3d(0.0, 1.0, 0.0);
	glVertex3d(-0.5, -0.5, 0.5);
	glVertex3d(-0.5, 0.5, 0.5);
	glVertex3d(-0.5, 0.5, -0.5);
	glVertex3d(-0.5, -0.5, -0.5);
	glEnd();


	glBegin(GL_POLYGON);
	glColor3d(0.0, 0.0, 1.0);
	glVertex3d(0.5, 0.5, 0.5);
	glVertex3d(0.5, 0.5, -0.5);
	glVertex3d(-0.5, 0.5, -0.5);
	glVertex3d(-0.5, 0.5, 0.5);
	glEnd();

	
	glBegin(GL_POLYGON);
	glColor3d(1.0, 0.0, 0.0);
	glVertex3d(0.5, -0.5, -0.5);
	glVertex3d(0.5, -0.5, 0.5);
	glVertex3d(-0.5, -0.5, 0.5);
	glVertex3d(-0.5, -0.5, -0.5);
	glEnd();

	glPopMatrix();
}

void Ermit(double P1[3], double P4[3], double R1[3], double R4[3]) {
	double P[3];

	glLineWidth(3); 
	glColor3d(0, 1, 0);
	glBegin(GL_LINE_STRIP);
	for (double t = 0; t <= 1.001; t += 0.01)
	{
		P[0] = ermit(P1[0], P4[0], R1[0], R4[0], t);
		P[1] = ermit(P1[1], P4[1], R1[1], R4[1], t);
		P[2] = ermit(P1[2], P4[2], R1[2], R4[2], t);
		glVertex3dv(P); 
	}

	glEnd();


	R1[0] = P1[0] + R1[0];
	R1[1] = P1[1] + R1[1];
	R1[2] = P1[2] + R1[2];

	R4[0] = P4[0] + R4[0];
	R4[1] = P4[1] + R4[1];
	R4[2] = P4[2] + R4[2];

	glColor3d(1, 0, 1);
	glLineWidth(1); 
	glPointSize(10);

	glBegin(GL_LINES); 
	glVertex3dv(P1);
	glVertex3dv(R1);

	glVertex3dv(P4);
	glVertex3dv(R4);
	glEnd();

	glColor3d(1, 0, 0);
	glBegin(GL_POINTS); 
	glVertex3dv(P1);
	glVertex3dv(P4);

	glColor3d(1, 0, 1);
	glVertex3dv(R1);
	glVertex3dv(R4);
	glEnd();
}

double t_anim = 0;
double direction = 1;
void BezierAnim(double P1[3], double P2[3], double P3[3], double P4[3], double delta_time) {
	const int duration = 5; 
	t_anim += delta_time / duration * direction;

	if (t_anim > 1) direction = -1;
	if (t_anim < 0) direction = 1;

	double P[3];
	P[0] = bezier(P1[0], P2[0], P3[0], P4[0], t_anim);
	P[1] = bezier(P1[1], P2[1], P3[1], P4[1], t_anim);
	P[2] = bezier(P1[2], P2[2], P3[2], P4[2], t_anim);

	glPushMatrix();
		glTranslated(P[0], P[1], P[2]);
		cube();
	glPopMatrix();

}

void Bezier(double P1[3], double P2[3], double P3[3], double P4[3]) {
	double P[3];

	glLineWidth(3); 
	glColor3d(0, 1, 0);
	glBegin(GL_LINE_STRIP);
	for (double t = 0; t <= 1.001; t += 0.01)
	{
		P[0] = bezier(P1[0], P2[0], P3[0], P4[0], t);
		P[1] = bezier(P1[1], P2[1], P3[1], P4[1], t);
		P[2] = bezier(P1[2], P2[2], P3[2], P4[2], t);
		glVertex3dv(P); 
	}
	glEnd();


	glColor3d(1, 0, 1);
	glLineWidth(1); 
	glPointSize(10);
	glBegin(GL_LINE_STRIP); 
	glVertex3dv(P1);
	glVertex3dv(P2);
	glVertex3dv(P3);
	glVertex3dv(P4);
	glEnd();

	glColor3d(1, 0, 0);
	glBegin(GL_POINTS); 
	glVertex3dv(P1);
	glVertex3dv(P2);
	glVertex3dv(P3);
	glVertex3dv(P4);
	glEnd();
}


void Render(double delta_time)
{

	double P1_1[] = { 0, 0, 0 }; 
	double P2_1[] = { -4, 1, 3 };
	double P3_1[] = { 7, 1, 3 };
	double P4_1[] = { -4, 4, 3 };
	Bezier(P1_1, P2_1, P3_1, P4_1);
	BezierAnim(P1_1, P2_1, P3_1, P4_1, delta_time);

	double P1_2[] = { 3, 2, 5 }; 
	double P2_2[] = { 5, 4, 6 };
	double P3_2[] = { 9, 4, 3 };
	double P4_2[] = { 13, 8, 3 };
	Bezier(P1_2, P2_2, P3_2, P4_2);

	double P1_3[] = { -2, -4, -1 };
	double P4_3[] = { -1, -4, 6 };
	double R1_1[] = { 3, 2, 0 };
	double R4_1[] = { 5, -1, 2 };
	Ermit(P1_3, P4_3, R1_1, R4_1);

	double P1_4[] = { -3, 2, -7 };
	double P4_4[] = { -4, -5, -1 };
	double R1_2[] = { 4, 5, 2 };
	double R4_2[] = { 3, 0, 1 };
	Ermit(P1_4, P4_4, R1_2, R4_2);

}