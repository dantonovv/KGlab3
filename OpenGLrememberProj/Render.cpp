#include "Render.h"

#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include <cmath>

const double PI = 3.14159265358979323846;

double _factorial(double num) {
	double res = 1;
	for (int i = 1; i <= num; i++) {
		res *= i;
	}
	return res;
}

double _bernstein(double n, double i, double u) {
	return (_factorial(n) / (_factorial(i) * _factorial(n - i))) * pow(u, i) * pow(1 - u, n - i);
}


double* _bezierSurface(double points[4][4][3], double u, double v) {
	double* P = new double[3];

	P[0] = 0;
	P[1] = 0;
	P[2] = 0;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int c = 0; c < 3; c++) {
				P[c] += _bernstein(3, i, u) * _bernstein(3, j, v) * points[i][j][c];
			}
		}
	}
	return P;
}


double _bezier(double p1, double p2, double p3, double p4, double t) {
	return pow(1 - t, 3) * p1 + 3 * t * pow(1 - t, 2) * p2 + 3 * pow(t, 2) * (1 - t) * p3 + pow(t, 3) * p4;
}

double _ermit(double p1, double p4, double r1, double r4, double t) {
	return p1 * (2 * pow(t, 3) - 3 * pow(t, 2) + 1) + p4 * (-2 * pow(t, 3) + 3 * pow(t, 2)) + r1 * (pow(t, 3) - 2 * pow(t, 2) + t) + r4 * (pow(t, 3) - pow(t, 2));
}

double _dotProduct(double V1[3], double V2[3]) {
	double c = V1[0] * V2[0] + V1[1] * V2[1] + V1[2] * V2[2];
	double d = sqrt(pow(V1[0], 2) + pow(V1[1], 2) + pow(V1[2], 2)) * sqrt(pow(V2[0], 2) + pow(V2[1], 2) + pow(V2[2], 2));
	return c / d;
}

double* _crossProduct(double V1[3], double V2[3]) {
	double* V = new double[3];
	V[0] = V1[1] * V2[2] - V1[2] * V2[1];
	V[1] = V1[2] * V2[0] - V1[0] * V2[2];
	V[2] = V1[0] * V2[1] - V1[1] * V2[0];
	return V;
}

double  _pointsDistance(double P1[3], double P2[3]) {
	return sqrt(pow(P2[0] - P1[0], 2) + pow(P2[1] - P1[0], 2) + pow(P2[2] - P1[2], 2));
}

double* _toVector(double P1[3], double P2[3]) {
	double* V = new double[3];
	V[0] = P2[0] - P1[0];
	V[1] = P2[1] - P1[1];
	V[2] = P2[2] - P1[2];

	return V;
}

double* _normalizeVec(double V1[3]) {
	double nulcord[3] = { 0, 0, 0 };
	double length = _pointsDistance(nulcord, V1);

	double relative_length = (1 / length);
	double x = V1[0] * relative_length;
	double y = V1[1] * relative_length;
	double z = V1[2] * relative_length;
	
	double* normalized = new double[3]{ x, y, z };
	return normalized;
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
		P[0] = _ermit(P1[0], P4[0], R1[0], R4[0], t);
		P[1] = _ermit(P1[1], P4[1], R1[1], R4[1], t);
		P[2] = _ermit(P1[2], P4[2], R1[2], R4[2], t);
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
	P[0] = _bezier(P1[0], P2[0], P3[0], P4[0], t_anim);
	P[1] = _bezier(P1[1], P2[1], P3[1], P4[1], t_anim);
	P[2] = _bezier(P1[2], P2[2], P3[2], P4[2], t_anim);

	double next_P[3];
	next_P[0] = _bezier(P1[0], P2[0], P3[0], P4[0], t_anim + 0.01 * direction);
	next_P[1] = _bezier(P1[1], P2[1], P3[1], P4[1], t_anim + 0.01 * direction);
	next_P[2] = _bezier(P1[2], P2[2], P3[2], P4[2], t_anim + 0.01 * direction);

	double* dirV = _toVector(P, next_P);
	double* dir = _normalizeVec(dirV);

	double orig[3] = { 1, 0, 0 };
	double rotXV[3] = { dir[0], dir[1], 0 };
	double* rotX = _normalizeVec(rotXV);

	double cosU = _dotProduct(orig, rotX);
	double* vecpr = _crossProduct(orig, rotX);

	double sinSign = vecpr[2] / abs(vecpr[2]);
	double U = acos(cosU) * 180 / PI * sinSign;

	double origZ[3] = { 0, 0, 1 };
	double cosZU = _dotProduct(origZ, dir);
	double ZU = acos(dir[2]) * 180.0 / PI - 90;
	
	glPushMatrix();
		glTranslated(P[0], P[1], P[2]);

		glRotated(U, 0, 0, 1);
		glRotated(ZU, 0, 1, 0);

		cube();
	glPopMatrix();

	delete[] dirV;
	delete[] dir;
	delete[] rotX;
	delete[] vecpr;
}

void Bezier(double P1[3], double P2[3], double P3[3], double P4[3]) {
	double P[3];

	glLineWidth(3); 
	glColor3d(0, 1, 0);
	glBegin(GL_LINE_STRIP);
	for (double t = 0; t <= 1.001; t += 0.01)
	{
		P[0] = _bezier(P1[0], P2[0], P3[0], P4[0], t);
		P[1] = _bezier(P1[1], P2[1], P3[1], P4[1], t);
		P[2] = _bezier(P1[2], P2[2], P3[2], P4[2], t);
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

void BezierSurface(double points[4][4][3]) {
	const double step = 0.1;
	const int size = 1 / 0.1 + 1; 

	double* P;
	double calculated[size][size][3];
	int i = 0, j = 0;

	for (double u = 0; u <= 1; u += step) {
		for (double v = 0; v <= 1; v += step) {
			P = _bezierSurface(points, u, v);
			calculated[i][j][0] = P[0];
			calculated[i][j][1] = P[1];
			calculated[i][j][2] = P[2];

			delete[] P;
			j++;
		}
		j = 0;
		i++;
	}

	glPointSize(4);
	glColor3d(0, 0, 0);
	glBegin(GL_POINTS);
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			glVertex3dv(calculated[i][j]);
		}
	}
	glEnd();

	glColor3d(1, 0, 0);
	for (i = 0; i < size; i++) {
		glBegin(GL_LINE_STRIP);
		for (j = 0; j < size; j++) {
			glVertex3dv(calculated[i][j]);
		}
		glEnd();
	}

	for (j = 0; j < size; j++) {
		glBegin(GL_LINE_STRIP);
		for (i = 0; i < size; i++) {
			glVertex3dv(calculated[i][j]);
		}
		glEnd();
	}
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

	double points[4][4][3] = {
		{{1, 5, 0}, {2, 5, -3}, {4, 5, -1}, {6, 5, 1}},
		{{1, 3, -5}, {2, 3, -4}, {4, 3, -2}, {6, 3, -5}},
		{{1, 2, -6}, {2, 2, -7}, {4, 2, 7}, {6, 2, -1}},
		{{1, 1, -2}, {2, 1, -7}, {4, 1, -1}, {6, 1, -4}}
	};
	BezierSurface(points);

}
