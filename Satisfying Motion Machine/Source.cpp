//  ========================================================================
//  COSC363: Computer Graphics (2021) Assignment 1
//
//  FILE NAME: Source.cpp
//  Author: Connor Hitchcock
//  Student ID: 66119643
//  ========================================================================

#include <cstdio>
#include <cmath>
#include <GL/freeglut.h>
#include "loadBMP.h"

#define GL_CLAMP_TO_EDGE 0x812F
#define MAX_PARTICLES 600

//--Global variables for inverted bowl
float bowl_starting_angle = 95.0;
float pendulum_factor = 7.015;
float bowl_angle = bowl_starting_angle;
float bowl_angular_velocity = 360 * 3.14159 / 180 / pendulum_factor;

//--Global variables for pendulum
float gravity = 9.8;					//Acceleration of gravity in m/s^2
float length = 15.0;					//The starting length of the pendulum
float mass = 0.025;						//The mass of the pendulum object in kilograms

float angle_one = 62.233;				//The starting angle
float angular_velocity_one = 0.0;		//Angular velocity of pendulum perpendiculr to the angle in m/s^2
float angular_acceleration_one = 0.0;	//Angular acceleration of pendulum perpendicular to angle in m/s^2
float x_offset_one = 0.0;				//The relative x-axis offset of the pendulum
float y_offset_one = 0.0;				//The relative y-axis offset of the pendulum
float force_one = 0;					//The force in newton meters

float angle_two = -45.19370;				
float angular_velocity_two = -0.67441;		
float angular_acceleration_two = 0.45809;	
float x_offset_two = 10.64242;				
float y_offset_two = -10.57066;				
float force_two = 0.45809;					

float angle_three = -1.10286;				
float angular_velocity_three = -0.99982;	
float angular_acceleration_three = 0.00118;	
float x_offset_three = 0.28875;				
float y_offset_three = -14.99722;			
float force_three = 0.00118;				

float angle_four = 44.37897;				
float angular_velocity_four = -0.67678;		
float angular_acceleration_four = -0.46242;	
float x_offset_four = -10.49098;			
float y_offset_four = -10.72098;			
float force_four = -0.46242;				

//--Global variables for camera
float camera_distance = 50;
float camera_angle = 60;
float camera_x = camera_distance * cos(camera_angle);
float camera_z = camera_distance * sin(camera_angle);

//--Global variables for sweep surface
const int N = 55; //Number of vertices
float vx_init[] = { 0.0, 0.045, 0.183, 0.402, 0.699, 1.076, 1.5, 1.969, 2.484, 3.0, 3.516, 4.031, 4.5, 4.924, 5.301,
5.598, 5.817, 5.955, 6.0, 5.852, 5.409, 4.75, 3.943, 3.057, 2.25, 1.591, 1.148, 1.0, 1.148, 1.591, 2.25, 3.057,
3.943, 4.75, 5.409, 5.852, 6.0, 5.955, 5.817, 5.598, 5.301, 4.924, 4.5, 4.031, 3.516, 3.0, 2.484, 1.969, 1.5,
1.076, 0.709, 0.402, 0.183, 0.045, 0.0 };
float vy_init[] = { 0.0, 0.33, 0.67, 1.0, 1.33, 1.67, 2.0, 2.33, 2.67, 3.0, 3.33,  3.67, 4.0, 4.33, 4.67, 5.0, 5.33,
5.67, 6.0, 6.33, 6.67, 7.0, 7.33, 7.67, 8.0, 8.33, 8.67, 9.0, 9.33, 9.67, 10.0, 10.33, 10.67, 11.0, 11.33, 11.67,
12.0, 12.33, 12.67, 13.0, 13.33, 13.67, 14.0, 14.33, 14.67, 15.0, 15.33, 15.67, 16.0, 16.33, 16.67, 17.0,
17.33, 17.67, 18.0 };
float vz_init[N] = { 0 };

//--Global variables for setup
bool sun = false;
GLUquadric* q;
GLuint txId[10];

//--Global variables for particles
float slowness = 2900.0;		//slows down particle speed
float min_y_speed = 1;			//minimum y speed
float min_fade_speed = 0.0005;	//minimum fade speed

//----Structure for particle -----------------------------------------------
typedef struct
{
	float x;			// the x position of the particle
	float z;			// the z position of the particle
	float y;			// the y position of the particle
	float x_direction;	// speed on the x-axis of the particle
	float z_direction;	// speed on the z-axis of the particle
	float y_direction;	// speed on the y-axis of the particle
	float fade;			// the speed at which the particle fades
	float life;			// The lifespan of the particle, 1 is full
} Particles;

// --Array of particle info--
Particles particle[MAX_PARTICLES];

//--Draw the particles and update there positions --------------------------
void update_particles()
{
	glColor3f(0, 0.8, 0.8);
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		if (particle[i].life > 0)
		{
			glTranslatef(particle[i].x, particle[i].y, particle[i].z);
			gluSphere(q, 0.02, 4, 4);
			glTranslatef(-particle[i].x, -particle[i].y, -particle[i].z);
			particle[i].x += particle[i].x_direction / slowness;
			particle[i].z += particle[i].z_direction / slowness;
			particle[i].y += particle[i].y_direction / slowness;
			particle[i].life -= particle[i].fade;		
			if (particle[i].y > 1 || particle[i].y < 0 || particle[i].life == 0)
			{
				particle[i].life = 1.0;	
				particle[i].fade = float(rand() % 100) / 1000.0 + min_fade_speed;
				particle[i].x = 0.0;
				particle[i].z = 0.0;
				particle[i].y = 0.15;
				particle[i].x_direction = float((rand() % 59) - 28.0);
				particle[i].z_direction = float((rand() % 59) - 28.0);
				particle[i].y_direction = float((rand() % 99) - 28.0) + min_y_speed;
			}
		}
	}
}

//--Initialise particles ---------------------------------------------------
void particle_init()
{
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		particle[i].x_direction = float((rand() % 49) - 24.0);
		particle[i].z_direction = float((rand() % 49) - 24.0);
		particle[i].y_direction = float((rand() % 81) - 24.0) + min_y_speed;
		particle[i].fade = float(rand() % 90) / 900 + min_fade_speed;
		particle[i].life = 1.0;
	}
}

//--Pendulum calculations for first cone and tether ------------------------
void pendulumCalcOne(void)
{
	force_one = gravity * sin((angle_one + 180.0) * 3.14159 / 180) / length;
	angular_acceleration_one = force_one;
	angular_velocity_one += angular_acceleration_one * mass;
	angle_one += angular_velocity_one;

	x_offset_one = length * sin((angle_one + 180.0) * 3.14159 / 180);
	y_offset_one = length * cos((angle_one + 180.0) * 3.14159 / 180);
}

//--Pendulum calculations for second cone and tether------------------------
void pendulumCalcTwo(void)
{
	force_two = gravity * sin((angle_two + 180.0) * 3.14159 / 180) / length;
	angular_acceleration_two = force_two;
	angular_velocity_two += angular_acceleration_two * mass;
	angle_two += angular_velocity_two;

	x_offset_two = length * sin((angle_two + 180.0) * 3.14159 / 180);
	y_offset_two = length * cos((angle_two + 180.0) * 3.14159 / 180);
}

//--Pendulum calculations for third cone and tether-------------------------
void pendulumCalcThree(void)
{
	force_three = gravity * sin((angle_three + 180.0) * 3.14159 / 180) / length;
	angular_acceleration_three = force_three;
	angular_velocity_three += angular_acceleration_three * mass;
	angle_three += angular_velocity_three;

	x_offset_three = length * sin((angle_three + 180.0) * 3.14159 / 180);
	y_offset_three = length * cos((angle_three + 180.0) * 3.14159 / 180);
}

//--Pendulum calculations for fourth cone and tether------------------------
void pendulumCalcFour(void)
{
	force_four = gravity * sin((angle_four + 180.0) * 3.14159 / 180) / length;
	angular_acceleration_four = force_four;
	angular_velocity_four += angular_acceleration_four * mass;
	angle_four += angular_velocity_four;

	x_offset_four = length * sin((angle_four + 180.0) * 3.14159 / 180);
	y_offset_four = length * cos((angle_four + 180.0) * 3.14159 / 180);
}

//--Inverted bowls rotation calculations -----------------------------------
void bowlCalc(void)
{
	bowl_angle += bowl_angular_velocity;
	if (bowl_angle - bowl_starting_angle >= 360) {
		bowl_angle = bowl_starting_angle;
	}
}

//--Function to load a texture in bmp format -------------------------------
void loadTexture()
{
	glGenTextures(10, txId); 

	// --Load textures--
	glBindTexture(GL_TEXTURE_2D, txId[0]);
	loadBMP("skybox_wall_one.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, txId[1]);
	loadBMP("skybox_wall_two.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, txId[2]);
	loadBMP("skybox_wall_three.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, txId[3]);
	loadBMP("skybox_wall_four.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, txId[4]);
	loadBMP("skybox_ceiling.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, txId[5]);
	loadBMP("skybox_floor.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_2D, txId[6]);
	loadBMP("bluetile.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, txId[7]);
	loadBMP("tile.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, txId[8]);
	loadBMP("plywood.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glBindTexture(GL_TEXTURE_2D, txId[9]);
	loadBMP("plank.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// --End loading textures--
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

//--Computes the normal vector of a triangle/quad --------------------------
void normal(float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3)
{
	float nx, ny, nz;
	nx = y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
	ny = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
	nz = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);

	glNormal3f(nx, ny, nz);
}

//--Camera update ----------------------------------------------------------
void cameraUpdate() {
	camera_x = camera_distance * cos(camera_angle * 3.14159 / 180);
	camera_z = camera_distance * sin(camera_angle * 3.14159 / 180);
}

//--Special keyboard event callback function -------------------------------
void special(int key, int x, int y)
{
	if (key == GLUT_KEY_UP) camera_distance--;
	else if (key == GLUT_KEY_DOWN) camera_distance++;
	else if (key == GLUT_KEY_LEFT) camera_angle++;
	else if (key == GLUT_KEY_RIGHT) camera_angle--;
	cameraUpdate();

	if (key == 's' || key == 'S') {

		if (sun) sun = false;
		else sun = true;
	}

	glutPostRedisplay();
}

//-- Extra key functions----------------------------------------------------
void key(unsigned char key, int x, int y)
{
	if (key == 's' || key == 'S') {
		if (sun) sun = false;
		else sun = true;
	}

	glutPostRedisplay();
}

int timerr = 0;

//--Time function ----------------------------------------------------------
void myTimer(int value)
{
	pendulumCalcOne();
	pendulumCalcTwo();
	pendulumCalcThree();
	pendulumCalcFour();
	bowlCalc();
	glutPostRedisplay();
	glutTimerFunc(20, myTimer, 20);
}

//--Draw custom sweep surface of a funny looking yoyo ----------------------
void drawYoyo()
{
	float vx[N], vy[N], vz[N], wx[N], wy[N], wz[N];
	float angleStep = -5 * 3.1415926535 / 180;
	glColor3f(0.0, 0.4, 0.7);

	for (int i = 0; i < N; i++) {
		vx[i] = vx_init[i];
		vy[i] = vy_init[i];
		vz[i] = vz_init[i];
	}

	for (int j = 0; j < 72; j++) {
		glBegin(GL_TRIANGLE_STRIP);
		for (int i = 0; i < N; i++)
		{
			wx[i] = vx[i] * cos(angleStep) + vz[i] * sin(angleStep);
			wy[i] = vy[i];
			wz[i] = -vx[i] * sin(angleStep) + vz[i] * cos(angleStep);
			if (i > 0) normal(wx[i - 1], wy[i - 1], wz[i - 1],
				vx[i - 1], vy[i - 1], vz[i - 1],
				vx[i], vy[i], vz[i]);
			glVertex3f(vx[i], vy[i], vz[i]);
			if (i > 0) normal(wx[i - 1], wy[i - 1], wz[i - 1],
				vx[i], vy[i], vz[i],
				wx[i], wy[i], wz[i]);
			glVertex3f(wx[i], wy[i], wz[i]);
		}
		glEnd();

		for (int i = 0; i < N; i++) {
			vx[i] = wx[i];
			vy[i] = wy[i];
			vz[i] = wz[i];
		}
	}
}

//--Draws skybox -----------------------------------------------------------
void skybox() {
	float white[4] = { 1.0, 1.0, 1.0, 1.0 };
	float black[4] = { 0.0, 0.0, 0.0, 1.0 };

	glEnable(GL_BLEND);
	glColor4f(1.0, 1.0, 1.0, 0.9);

	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_SPECULAR, black);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glEnable(GL_TEXTURE_2D);
	
		// Left Wall
		glBindTexture(GL_TEXTURE_2D, txId[0]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-256.0, -256.0, 256.0);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-256.0, -256.0, -256.0);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-256.0, 256.0, -256.0);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-256.0, 256.0, 256.0);
		glEnd();

		// Front Wall
		glBindTexture(GL_TEXTURE_2D, txId[1]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);		
		glVertex3f(256.0, -256.0, 256.0);
		glTexCoord2f(1.0, 0.0);		
		glVertex3f(-256.0, -256.0, 256.0);
		glTexCoord2f(1.0, 1.0);		
		glVertex3f(-256.0, 256.0, 256.0);
		glTexCoord2f(0.0, 1.0);		
		glVertex3f(256.0, 256.0, 256.0);
		glEnd();

		// Right Wall
		glBindTexture(GL_TEXTURE_2D, txId[2]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(256.0, -256.0, -256.0);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(256.0, -256.0, 256.0);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(256.0, 256.0, 256.0);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(256.0, 256.0, -256.0);
		glEnd();
		
		// Back Wall
		glBindTexture(GL_TEXTURE_2D, txId[3]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-256.0, -256.0, -256.0);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(256.0, -256.0, -256.0);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(256.0, 256.0, -256.0);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-256.0, 256.0, -256.0);
		glEnd();

		// Ceiling
		glBindTexture(GL_TEXTURE_2D, txId[4]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);		
		glVertex3f(256.0, 256.0, -256.0);
		glTexCoord2f(1.0, 0.0);		
		glVertex3f(256.0, 256.0, 256.0);
		glTexCoord2f(1.0, 1.0);		
		glVertex3f(-256.0, 256.0, 256.0);
		glTexCoord2f(0.0, 1.0);		
		glVertex3f(-256.0, 256.0, -256.0);
		glEnd();

		// Floor
		glBindTexture(GL_TEXTURE_2D, txId[5]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(256.0, -256.0, -256.0);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(256.0, -256.0, 256.0);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-256.0, -256.0, 256.0);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-256.0, -256.0, -256.0);
		glEnd();

		glDisable(GL_TEXTURE_2D);
		glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glPopMatrix();
	glDisable(GL_BLEND);
}

//--Draws scene structure for floor -----------------------------------------
void draw_scene_floor()
{
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, txId[7]);
		gluQuadricTexture(q, true);

		glTranslatef(0, -19, 0);
		glRotatef(-90, 1, 0, 0);
		gluCylinder(q, 25, 25, 3, 50, 50);
		gluDisk(q, 0, 25, 50, 50);
		glTranslatef(0, 0, 3);
		gluDisk(q, 0, 25, 50, 50);

		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

//--Draws scene structure pendulum support ----------------------------------
void draw_pendulum_support()
{
	//draws cylindrical structs
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, txId[8]);
		gluQuadricTexture(q, true);

		glTranslatef(20, 10, 0);
		glRotatef(90, 1, 0, 0);
		gluCylinder(q, 0.8, 0.5, 30, 20, 20);

		glTranslatef(-40, 0, 0);
		gluCylinder(q, 0.8, 0.5, 30, 20, 20);

		glRotatef(90, 1, 0, 0);
		glRotatef(90, 0, 1, 0);
		gluCylinder(q, 0.8, 0.5, 40, 20, 20);

		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//draws spherical structs
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, txId[9]);
		gluQuadricTexture(q, true);
		
		glTranslatef(0, 10, 0);
		gluSphere(q, 2, 20, 20);

		glTranslatef(20, 0, 0);
		gluSphere(q, 2, 20, 20);

		glTranslatef(-40, 0, 0);
		gluSphere(q, 2, 20, 20);

		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

//--Draws cone --------------------------------------------------------------
void draw_cone()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[6]);
	gluQuadricTexture(q, true);
	gluCylinder(q, 0.0, 2.5, 5, 20, 20);
	glDisable(GL_TEXTURE_2D);
}

//--Draws tether -----------------------------------------------------------
void draw_tether()
{
	glColor3f(1.0, 1.0, 1.0);
	gluCylinder(q, 0.1, 0.1, length + 0.1, 20, 20);
}


//Draws disc ---------------------------------------------------------------
void draw_disc()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[6]);
	gluQuadricTexture(q, true);
	gluDisk(q, 0, 2.5, 20, 20);
	glDisable(GL_TEXTURE_2D);
}

//--Display: ---------------------------------------------------------------
//--This is the main display module containing function calls for generating
//--the scene.
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(camera_x, 0, camera_z, 0, -5, 0, 0, 1, 0);

	// --Sun Spolight:
	float lpossun[4] = { 0.0, 30, 30, 1.0};
	float centerpos[3] = { 0.0, -10.0, 0.0 };
	float spotdirsun[3] = { centerpos[0] - lpossun[0], centerpos[1] - lpossun[1], centerpos[2] - lpossun[2] };
	glLightfv(GL_LIGHT1, GL_POSITION, lpossun);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotdirsun);

	//Checks if sun spotlight should be on
	if (sun) glEnable(GL_LIGHT1);
	else glDisable(GL_LIGHT1);

	// --Spotlights--
	float lposone[4] = { 20, 10.0, 0, 1.0 };  //light's position of spotlight 1
	float lpostwo[4] = { -20, 10.0, 0, 1.0 };  //light's position of spotlight 2
	float targetpos[3] = { x_offset_two * 0.7071, y_offset_two + 9.0, x_offset_two * 0.7071 };
	float spotdirone[3] = { targetpos[0] - lposone[0], targetpos[1] - lposone[1], targetpos[2] - lposone[2] };
	float spotdirtwo[3] = { targetpos[0] - lpostwo[0], targetpos[1] - lpostwo[1], targetpos[2] - lpostwo[2] };
	glLightfv(GL_LIGHT2, GL_POSITION, lposone);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotdirone);
	glLightfv(GL_LIGHT3, GL_POSITION, lpostwo);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spotdirtwo);

	// --Draws Skybox
	glDisable(GL_LIGHTING);	
	skybox();
	glEnable(GL_LIGHTING);

	// --Draw Scene
	draw_scene_floor();
	draw_pendulum_support();

	// --Setup Particles--
	glPushMatrix();
		glTranslatef(13, -16, 0);
		glScalef(20, 10, 20);
		update_particles();
		particle_init();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-13, -16, 0);
		glScalef(20, 10, 20);
		update_particles();
		particle_init();
	glPopMatrix();

	// --Cone & Tether 1--
	glPushMatrix();
		glTranslatef(x_offset_one, y_offset_one + 5, 0);
		glRotatef(-90, 1, 0, 0);
		draw_cone();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0, 10, 0);
		glRotatef(90, 0, 1, 0);
		glRotatef(angle_one + 90, 1, 0, 0);
		draw_tether();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(x_offset_one, y_offset_one + 10, 0);
		glRotatef(-90, 1, 0, 0);
		draw_disc();
	glPopMatrix();

	// --Cone & Tether 2--
	glPushMatrix();
		glTranslatef(x_offset_two * 0.7071, y_offset_two + 5, x_offset_two * 0.7071);
		glRotatef(-90, 1, 0, 0);
		draw_cone();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0, 10, 0);
		glRotatef(45, 0, 1, 0);
		glRotatef(angle_two + 90, 1, 0, 0);
		draw_tether();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(x_offset_two * 0.7071, y_offset_two + 10, x_offset_two * 0.7071);
		glRotatef(-90, 1, 0, 0);
		draw_disc();
	glPopMatrix();

	// --Cone & Tether 3--
	glPushMatrix();
		glTranslatef(0, y_offset_three + 5, x_offset_three);
		glRotatef(-90, 1, 0, 0);
		draw_cone();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0, 10, 0);
		glRotatef(angle_three + 90, 1, 0, 0);
		draw_tether();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0, y_offset_three + 10, x_offset_three);
		glRotatef(-90, 1, 0, 0);
		draw_disc();
	glPopMatrix();

	// --Cone & Tether 4--
	glPushMatrix();
		glTranslatef(-x_offset_four * 0.7071, y_offset_four + 5, x_offset_four * 0.7071);
		glRotatef(-90, 1, 0, 0);
		draw_cone();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0, 10, 0);
		glRotatef(-45, 0, 1, 0);
		glRotatef(angle_four + 90, 1, 0, 0);
		draw_tether();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-x_offset_four * 0.7071, y_offset_four + 10, x_offset_four * 0.7071);
		glRotatef(-90, 1, 0, 0);
		draw_disc();
	glPopMatrix();

	// --Inverted Bowl--
	glPushMatrix();
		glTranslatef(0.0, -11.0, -0.0);
		glRotatef(90, 1, 0, 0);
		glTranslatef(0.0, 0.0, 0.0);
		glRotatef(bowl_angle, 0, 0, 1);
		glTranslatef(0.0, -9.0, 0.0);
		drawYoyo();
	glPopMatrix();

	glutSwapBuffers();
}

//----------------------------------------------------------------------
void initialize(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	float grey[4] = { 0.2, 0.2, 0.2, 1.0 };
	float white[4] = { 1.0, 1.0, 1.0, 1.0 };
	q = gluNewQuadric();

	//Initialise starting camera position
	cameraUpdate();

	//Load Texture
	loadTexture();

	glEnable(GL_LIGHTING);		//Enable OpenGL states		
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	//Sun Spotlight Setup
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 91.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 40.0);

	//Spotlight Setup
	glEnable(GL_LIGHT2);
	glLightfv(GL_LIGHT2, GL_AMBIENT, grey);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT2, GL_SPECULAR, white);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30.0);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 50.0);
	glEnable(GL_LIGHT3);
	glLightfv(GL_LIGHT3, GL_AMBIENT, grey);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT3, GL_SPECULAR, white);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 30.0);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 50.0);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 10.0, 1000.0);   //Camera Frustum
}

//  ------- Main: Initialize glut window and register call backs -------
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Yoyo");
	initialize();
	glutDisplayFunc(display);
	glutSpecialFunc(special);
	glutKeyboardFunc(key);
	glutTimerFunc(50, myTimer, 0);
	glutMainLoop();
	return 0;
}

