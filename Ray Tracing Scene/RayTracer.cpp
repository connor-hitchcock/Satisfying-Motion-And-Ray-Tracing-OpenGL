/*==================================================================================
* COSC363 Assignment 2 - Ray Tracing
* Author: Connor Hitchcock
* Student ID: 66119643
*===================================================================================
*/
#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "SceneObject.h"
#include "Ray.h"
#include "Plane.h"
#include "Cylinder.h"
#include "Cone.h"
#include <GL/freeglut.h>
#include "TextureBMP.h"
using namespace std;

const float WIDTH = 20.0;  
const float HEIGHT = 20.0;
const float EDIST = 40.0;
const int NUMDIV = 500;
const int MAX_STEPS = 5;
const float XMIN = -WIDTH * 0.5;
const float XMAX =  WIDTH * 0.5;
const float YMIN = -HEIGHT * 0.5;
const float YMAX =  HEIGHT * 0.5;

TextureBMP texture1;
TextureBMP texture2;
TextureBMP texture3;
vector<SceneObject*> sceneObjects;

//--Global variables for polygon coordinates and sizes
const float WALLX1 = -160.0;
const float WALLX2 = 160.0;
const float WALLY1 = -20.0;
const float WALLY2 = 220.0;
const float WALLZ = -600.0;

const float PLANEX1 = 160.0;
const float PLANEX2 = -160.0;
const float PLANEY = -15.0;
const float PLANEZ1 = -40.0;
const float PLANEZ2 = -600.0;

const float SPHERE1RADIUS = 15.0;
const float SPHERE1X = 0.0;
const float SPHERE1Y = 12.0;
const float SPHERE1Z = -120.0;
const float SPHERE2RADIUS = 7.0;
const float SPHERE2X = 22.0;
const float SPHERE2Y = 4.0;
const float SPHERE2Z = -120.0;
const float SPHERE3RADIUS = 7.0;
const float SPHERE3X = -22.0;
const float SPHERE3Y = 4.0;
const float SPHERE3Z = -120.0;
const float SPHERE4RADIUS = 5.0;
const float SPHERE4X = 0.0;
const float SPHERE4Y = -10.0;
const float SPHERE4Z = -80.0;
const float SPHERE5RADIUS = 4.0;
const float SPHERE5X = 11.0;
const float SPHERE5Y = -11.0;
const float SPHERE5Z = -75.0;
const float SPHERE6RADIUS = 4.0;
const float SPHERE6X = -11.0;
const float SPHERE6Y = -11.0;
const float SPHERE6Z = -75.0;

const float OCTAHEDRONLENGTH = 10.0;
const float OCTAHEDRONX = 11.0;
const float OCTAHEDRONY = -9.0 + OCTAHEDRONLENGTH;
const float OCTAHEDRONZ = -100;

const float CUBELENGTH = 6.0;
const float CUBEX = -11.0 - (CUBELENGTH / 2);
const float CUBEY = -9.0;
const float CUBEZ = -100;

const float CONERADIUS = 8.0;
const float CONEHEIGHT = 6.0;
const float CONE1X = OCTAHEDRONX;
const float CONE1Y = -15.0;
const float CONE1Z = -100.0;
const float CONE2X = CUBEX + (CUBELENGTH / 2);
const float CONE2Y = -15.0;
const float CONE2Z = -100.0;

const float CYLINDERRADIUS = 0.5;
const float CYLINDERHEIGHT = 12.0;
const float CYLINDER1X = SPHERE1X;
const float CYLINDER1Y = -15.0;
const float CYLINDER1Z = SPHERE1Z;
const float CYLINDER2X = SPHERE2X;
const float CYLINDER2Y = -15.0;
const float CYLINDER2Z = SPHERE2Z;
const float CYLINDER3X = SPHERE3X;
const float CYLINDER3Y = -15.0;
const float CYLINDER3Z = SPHERE3Z;

//--Global variables for lighting
float LIGHTINTENSITY = 0.50;
float TRANSPARENTFACTOR = 0.2;
float REFRACTETA = 1.02;
float EXPONENT = 10.0;

//---The most important function in a ray tracer! ---------------------------------- 
//   Computes the colour value obtained by tracing a ray and finding its 
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------
glm::vec3 trace(Ray ray, int step)
{
	glm::vec3 backgroundColor(0);						//Background colour = (0,0,0)
	glm::vec3 lightPos(10, 40, -3);					//Light's position 1
	glm::vec3 lightPos2(-10, 40, -3);				//Light's position 2
	glm::vec3 color(0);
	SceneObject* obj;

	ray.closestPt(sceneObjects);					//Compare the ray with all objects in the scene
	if (ray.index == -1) return backgroundColor;	 //no intersection
	obj = sceneObjects[ray.index];					//object on which the closest point of intersection is found
	color = sceneObjects[ray.index]->getColor();
	color = obj->lighting(lightPos, -ray.dir, ray.hit);
	color += obj->lighting(lightPos2, -ray.dir, ray.hit);
	color *= glm::vec3(0.7);	//Makes the scene darkers
	glm::vec3 normalVec = sceneObjects[ray.index]->normal(ray.hit);
	
	//--Wall texture
	if (ray.index == 0)
	{
		float texcoords = (ray.hit.x - WALLX1) / (WALLX2 - WALLX1);
		float texcoordt = (ray.hit.y - WALLY1) / (WALLY2 - WALLY1);
		if (texcoords > 0 && texcoords < 1 &&
			texcoordt > 0 && texcoordt < 1)
		{
			color = texture1.getColorAt(texcoords, texcoordt);
			obj->setColor(color);
		}
	}

	//--Floor texture
	if (ray.index == 1)
	{
		float x = (ray.hit.x + 50) / 4;
		float z = (ray.hit.z + 200) / 4;
		int modulox = int(x) % 2;
		int moduloz = int(z) % 2;
		if ((!modulox && !moduloz) || (modulox && moduloz))
		{
			color = glm::vec3(0.0, 1.0, 1.0);
			obj->setColor(color);
		}
		else
		{
			color = glm::vec3(1.0, 1.0, 0.0);
			obj->setColor(color);
		}
	}

	//--Sphere texture 1
	if (ray.index == 3)
	{
		glm::vec3 center = glm::vec3(SPHERE2X, SPHERE2Y, SPHERE2Z);
		glm::vec3 dist = glm::normalize(ray.hit - center);
		float texcoords = 0.5 + (atan2(dist.x, dist.z) / (2 * M_PI));
		float texcoordt = 0.5 + (asin(dist.y) / M_PI);
		color = texture2.getColorAt(texcoords, texcoordt);
		obj->setColor(color);
	}

	//-Sphere texture 2
	if (ray.index == 4)
	{
		glm::vec3 center = glm::vec3(SPHERE3X, SPHERE3Y, SPHERE3Z);
		glm::vec3 dist = glm::normalize(ray.hit - center);
		float texcoords = 0.5 + (atan2(dist.x, dist.z) / (2 * M_PI));
		float texcoordt = 0.5 + (asin(dist.y) / M_PI);
		color = texture3.getColorAt(texcoords, texcoordt);
		obj->setColor(color);
	}

	//--Cone textures
	if (ray.index == 8 || ray.index == 9)
	{
		float x = ray.hit.x ;
		float y = ray.hit.y * (ray.hit.y * 2);
		if ((int(x + y) % 4) == 0 || (int(x + y) % 4) == 1)
		{
			color = glm::vec3(0.7, 1.0, 0.7);
			obj->setColor(color);
		}
		else
		{
			color = glm::vec3(0.0, 0.8, 0.0);
			obj->setColor(color);
		}
	}

	//--Lighting
	glm::vec3 ambientTerm(0.6);
	glm::vec3 specularTerm(0.0);

	glm::vec3 lightVec = lightPos - ray.hit;
	float lightDist = glm::length(lightVec);
	lightVec = glm::normalize(lightVec);
	float lightDotNormal = glm::dot(lightVec, normalVec);
	glm::vec3 reflectVec = glm::reflect(-lightVec, normalVec);
	float reflectDotVec = glm::dot(reflectVec, -ray.dir);
	reflectDotVec = max(reflectDotVec, (float)0.0);

	glm::vec3 lightVec2 = lightPos2 - ray.hit;
	float lightDist2 = glm::length(lightVec2);
	lightVec2 = glm::normalize(lightVec2);
	float lightDotNormal2 = glm::dot(lightVec2, normalVec);
	glm::vec3 reflectVec2 = glm::reflect(-lightVec2, normalVec);
	float reflectDotVec2 = glm::dot(reflectVec2, -ray.dir);
	reflectDotVec = max(reflectDotVec, (float)0.0);

	//--Shadows
	Ray shadowRay(ray.hit, lightVec);
	shadowRay.closestPt(sceneObjects);
	Ray shadowRay2(ray.hit, lightVec2);
	shadowRay2.closestPt(sceneObjects);

	//--Shadows from first light source
	if (reflectDotVec >= 0) specularTerm = glm::vec3(1.0) * pow(reflectDotVec, EXPONENT);
	if ((lightDotNormal <= 0) || ((shadowRay.index > -1) && (shadowRay.dist < lightDist)))
	{
		color = ambientTerm * color;
		if (shadowRay.index == 10) //Creates a lighter shadow for the transparent sphere
		{
			glm::vec3 sphereColor = sceneObjects[10]->getColor() * glm::vec3(0.0125);
			color += (glm::vec3(0.4) * (color * lightDotNormal)) + sphereColor;
		}
		else if (shadowRay.index == 11) //Creates a lighter shadow for the first refractive sphere
		{
			glm::vec3 sphereColor = sceneObjects[11]->getColor() * glm::vec3(0.0125);
			color += (glm::vec3(0.4) * (color * lightDotNormal)) + sphereColor;
		}
		else if (shadowRay.index == 12) //Creates a lighter shadow for the second refractive sphere
		{
			glm::vec3 sphereColor = sceneObjects[12]->getColor() * glm::vec3(0.0125);
			color += (glm::vec3(0.4) * (color * lightDotNormal)) + sphereColor;
		}
		else
		{
			color = (ambientTerm * color) + (((color * lightDotNormal) + specularTerm) * LIGHTINTENSITY);
		}
	}

	//-Shadows from second light source
	if (reflectDotVec2 >= 0) specularTerm = glm::vec3(1.0) * pow(reflectDotVec2, EXPONENT);
	if ((lightDotNormal2 <= 0) || ((shadowRay2.index > -1) && (shadowRay2.dist < lightDist2)))
	{
		color = ambientTerm * color;
		if (shadowRay2.index == 10) //Creates a lighter shadow for the transparent sphere
		{
			glm::vec3 sphereColor = sceneObjects[10]->getColor() * glm::vec3(0.0125);
			color += (glm::vec3(0.4) * (color * lightDotNormal2)) + sphereColor;
		}
		else if (shadowRay2.index == 11) //Creates a lighter shadow for the first refractive sphere
		{
			glm::vec3 sphereColor = sceneObjects[11]->getColor() * glm::vec3(0.0125);
			color += (glm::vec3(0.4) * (color * lightDotNormal2)) + sphereColor;
		}
		else if (shadowRay2.index == 12) //Creates a lighter shadow for the second refractive sphere
		{
			glm::vec3 sphereColor = sceneObjects[12]->getColor() * glm::vec3(0.0125);
			color += (glm::vec3(0.4) * (color * lightDotNormal2)) + sphereColor;
		}
		else
		{
			color = (ambientTerm * color) + (((color * lightDotNormal2) + specularTerm) * LIGHTINTENSITY);
		}
	}

	//--Transparency
	if (obj->isTransparent() && (step < MAX_STEPS))
	{
		Ray transparentRay1(ray.hit, ray.dir);
		transparentRay1.closestPt(sceneObjects);
		if (transparentRay1.index == -1) return backgroundColor;
		Ray transparentRay2(transparentRay1.hit, transparentRay1.dir);
		transparentRay2.closestPt(sceneObjects);
		if (transparentRay2.index == -1) return backgroundColor;
		glm::vec3 transparentColor = trace(transparentRay2, step + 1);
		color = (transparentColor * (1 - TRANSPARENTFACTOR)) + (color * TRANSPARENTFACTOR);
	}

	//--Reflectivitiy
	if (obj->isReflective() && (step < MAX_STEPS))
	{
		float rho = obj->getReflectionCoeff();
		glm::vec3 normalVec = obj->normal(ray.hit);
		glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVec);
		Ray reflectedRay(ray.hit, reflectedDir);
		glm::vec3 reflectedColor = trace(reflectedRay, step + 1);
		color = color + (rho * reflectedColor);
	}

	//--Refractivitiy
	if (obj->isRefractive() && (step < MAX_STEPS))
	{
		glm::vec3 refractDir1 = glm::refract(ray.dir, normalVec, ((float)1.0) / REFRACTETA);
		Ray refractRay1(ray.hit, refractDir1);
		refractRay1.closestPt(sceneObjects);
		if (refractRay1.index == -1) return backgroundColor;
		glm::vec3 normalVec2 = sceneObjects[refractRay1.index]->normal(refractRay1.hit);
		glm::vec3 refractDir2 = glm::refract(refractDir1, -normalVec2, REFRACTETA);
		Ray refractRay2(refractRay1.hit, refractDir2);
		refractRay2.closestPt(sceneObjects);
		if (refractRay2.index == -1) return backgroundColor;
		glm::vec3 refractColor = trace(refractRay2, step + 1);
		color = (refractColor * (1 - TRANSPARENTFACTOR)) + (color * TRANSPARENTFACTOR);
	}
	
	//--Final color value
	return color;
}


// Creates the cube object
//---------------------------------------------------------------------------------------
void createCube(float x, float y, float z, float length, glm::vec3 color)
{
	glm::vec3 p1 = glm::vec3(x, y, z);
	glm::vec3 p2 = glm::vec3(x + length, y, z);
	glm::vec3 p3 = glm::vec3(x + length, y + length, z);
	glm::vec3 p4 = glm::vec3(x, y + length, z);
	glm::vec3 p5 = glm::vec3(x + length, y, z - length);
	glm::vec3 p6 = glm::vec3(x + length, y + length, z - length);
	glm::vec3 p7 = glm::vec3(x, y + length, z - length);
	glm::vec3 p8 = glm::vec3(x, y, z - length);

	Plane* plane1 = new Plane(p1, p2, p3, p4);
	Plane* plane2 = new Plane(p2, p5, p6, p3);
	Plane* plane3 = new Plane(p5, p8, p7, p6);
	Plane* plane4 = new Plane(p4, p7, p8, p1);
	Plane* plane5 = new Plane(p4, p3, p6, p7);
	Plane* plane6 = new Plane(p8, p5, p2, p1);

	plane1->setColor(color);
	plane2->setColor(color);
	plane3->setColor(color);
	plane4->setColor(color);
	plane5->setColor(color);
	plane6->setColor(color);

	sceneObjects.push_back(plane1);
	sceneObjects.push_back(plane2);
	sceneObjects.push_back(plane3);
	sceneObjects.push_back(plane4);
	sceneObjects.push_back(plane5);
	sceneObjects.push_back(plane6);
}


// Creates the octahedron object
//---------------------------------------------------------------------------------------
void createOctahedron(float x, float y, float z, float length, glm::vec3 color)
{
	float halflen = length / 2;

	glm::vec3 p1 = glm::vec3(x, y, z);
	glm::vec3 p2 = glm::vec3(x - halflen, y - halflen, z);
	glm::vec3 p3 = glm::vec3(x, y - halflen, z - halflen);
	glm::vec3 p4 = glm::vec3(x + halflen, y - halflen, z);
	glm::vec3 p5 = glm::vec3(x, y - halflen, z + halflen);
	glm::vec3 p6 = glm::vec3(x, y - length, z);

	Plane* triangle1 = new Plane(p1, p3, p2);
	Plane* triangle2 = new Plane(p1, p4, p4);
	Plane* triangle3 = new Plane(p1, p5, p4);
	Plane* triangle4 = new Plane(p1, p2, p5);
	Plane* triangle5 = new Plane(p6, p2, p3);
	Plane* triangle6 = new Plane(p6, p3, p4);
	Plane* triangle7 = new Plane(p6, p4, p5);
	Plane* triangle8 = new Plane(p6, p5, p2);

	triangle1->setColor(color);
	triangle2->setColor(color);
	triangle3->setColor(color);
	triangle4->setColor(color);
	triangle5->setColor(color);
	triangle6->setColor(color);
	triangle7->setColor(color);
	triangle8->setColor(color);
	
	sceneObjects.push_back(triangle1);
	sceneObjects.push_back(triangle2);
	sceneObjects.push_back(triangle3);
	sceneObjects.push_back(triangle4);
	sceneObjects.push_back(triangle5);
	sceneObjects.push_back(triangle6);
	sceneObjects.push_back(triangle7);
	sceneObjects.push_back(triangle8);
}

// Anti-aliasing
//---------------------------------------------------------------------------------------
glm::vec3 antiAliasing(glm::vec3 eye, float pixel_size, float xp, float yp)
{
	glm::vec3 average(0.25);
	glm::vec3 color(0);
	
	Ray ray = Ray(eye, glm::vec3((pixel_size * 0.25) + xp, (pixel_size * 0.25) + yp, -EDIST));
	ray.dir = glm::normalize(ray.dir);			
	color += trace(ray, 1);
	ray = Ray(eye, glm::vec3((pixel_size * 0.25) + xp, (pixel_size * 0.75) + yp, -EDIST));
	ray.dir = glm::normalize(ray.dir);
	color += trace(ray, 1);
	ray = Ray(eye, glm::vec3((pixel_size * 0.75) + xp, (pixel_size * 0.25) + yp, -EDIST));
	ray.dir = glm::normalize(ray.dir);
	color+=trace(ray, 1);
	ray = Ray(eye, glm::vec3((pixel_size * 0.75) + xp, (pixel_size * 0.75) + yp, -EDIST));
	ray.dir = glm::normalize(ray.dir);
	color += trace(ray, 1);
	
	color *= average;
	return color;
}

//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each cell as a quad.
//---------------------------------------------------------------------------------------
void display()
{
	float xp, yp;							//grid point
	float cellX = (XMAX - XMIN) / NUMDIV;	//cell width
	float cellY = (YMAX - YMIN) / NUMDIV;	//cell height
	glm::vec3 eye(0., 0., 0.);

	glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glBegin(GL_QUADS);  //Each cell is a tiny quad.

	for(int i = 0; i < NUMDIV; i++)	//Scan every cell of the image plane
	{
		xp = XMIN + i * cellX;
		for(int j = 0; j < NUMDIV; j++)
		{
			yp = YMIN + j * cellY;

		    glm::vec3 dir(xp + 0.5 * cellX, yp + 0.5 * cellY, -EDIST);	//direction of the primary ray

		    Ray ray = Ray(eye, dir);

			//anti-aliasing
			ray.dir = glm::normalize(ray.dir); //COMMENT OUT for anti-aliasing off
			glm::vec3 col = antiAliasing(eye, cellX, xp, yp); //COMMENT OUT for anti-aliasing off
		    //glm::vec3 col = trace (ray, 1); //COMMENT OUT for anti-aliasing on

			glColor3f(col.r, col.g, col.b);
			glVertex2f(xp, yp);				//Draw each cell with its color value
			glVertex2f(xp + cellX, yp);
			glVertex2f(xp + cellX, yp + cellY);
			glVertex2f(xp, yp + cellY);
        }
    }

    glEnd();
    glFlush();
}

//---This function initializes the scene ------------------------------------------- 
//   Specifically, it creates scene objects (spheres, planes, cones, cylinders etc)
//     and add them to the list of scene objects.
//   It also initializes the OpenGL orthographc projection matrix for drawing the
//     the ray traced image.
//----------------------------------------------------------------------------------
void initialize()
{
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(XMIN, XMAX, YMIN, YMAX);

	glClearColor(0, 0, 0, 1);

	//--Initialise textures
	texture1 = TextureBMP("Leaves.bmp");
	texture2 = TextureBMP("Wood.bmp");
	texture3 = TextureBMP("OldWood.bmp");

	//--Plane
	Plane* plane = new Plane(glm::vec3(PLANEX2, PLANEY, PLANEZ1),		//Point A
		glm::vec3(PLANEX1, PLANEY, PLANEZ1),		//Point B
		glm::vec3(PLANEX1, PLANEY, PLANEZ2),		//Point C
		glm::vec3(PLANEX2, PLANEY, PLANEZ2));		//Point D
	plane->setColor(glm::vec3(0.8, 0.8, 0));
	plane->setSpecularity(false);

	//--Wall
	Plane* wall = new Plane(glm::vec3(WALLX1, WALLY1, WALLZ),	//Point A
		glm::vec3(WALLX2, WALLY1, WALLZ),	//Point B
		glm::vec3(WALLX2, WALLY2, WALLZ),	//Point C
		glm::vec3(WALLX1, WALLY2, WALLZ));	//Point D
	wall->setColor(glm::vec3(1));
	wall->setSpecularity(false);

	//--Big sphere
	Sphere* sphere1 = new Sphere(glm::vec3(SPHERE1X, SPHERE1Y, SPHERE1Z), SPHERE1RADIUS);
	sphere1->setColor(glm::vec3(0, 1, 1));
	sphere1->setSpecularity(true);
	sphere1->setReflectivity(true, 0.8);
	
	//--Textured sphere 1
	Sphere* sphere2 = new Sphere(glm::vec3(SPHERE2X, SPHERE2Y, SPHERE2Z), SPHERE2RADIUS);
	sphere2->setColor(glm::vec3(1, 0, 0)); 

	//--Textured sphere 2
	Sphere* sphere3 = new Sphere(glm::vec3(SPHERE3X, SPHERE3Y, SPHERE3Z), SPHERE3RADIUS);
	sphere3->setColor(glm::vec3(1, 0, 0));

	//--Cone 1
	Cone* cone1 = new Cone(glm::vec3(CONE1X, CONE1Y, CONE1Z), CONERADIUS, CONEHEIGHT);
	cone1->setColor(glm::vec3(0, 0.5, 0));

	//--Cone 2
	Cone* cone2 = new Cone(glm::vec3(CONE2X, CONE2Y, CONE2Z), CONERADIUS, CONEHEIGHT);
	cone2->setColor(glm::vec3(0, 0.5, 0));

	//--Cylinder for big sphere
	Cylinder* cylinder1 = new Cylinder(glm::vec3(CYLINDER1X, CYLINDER1Y, CYLINDER1Z), CYLINDERRADIUS, CYLINDERHEIGHT);
	cylinder1->setColor(glm::vec3(1.0, 1.0, 0.0));
	
	//--Cylinder for textured sphere
	Cylinder* cylinder2 = new Cylinder(glm::vec3(CYLINDER2X, CYLINDER2Y, CYLINDER2Z), CYLINDERRADIUS, CYLINDERHEIGHT);
	cylinder2->setColor(glm::vec3(0.8, 0.8, 0.0));

	//--Cylinder for second textured sphere
	Cylinder* cylinder3 = new Cylinder(glm::vec3(CYLINDER3X, CYLINDER3Y, CYLINDER3Z), CYLINDERRADIUS, CYLINDERHEIGHT);
	cylinder3->setColor(glm::vec3(0.8, 0.8, 0.0));

	//--Transparent Sphere
	Sphere* sphere4 = new Sphere(glm::vec3(SPHERE4X, SPHERE4Y, SPHERE4Z), SPHERE4RADIUS);
	sphere4->setColor(glm::vec3(1, 0, 0));
	sphere4->setTransparency(true);

	//--Refractive Sphere 1
	Sphere* sphere5 = new Sphere(glm::vec3(SPHERE5X, SPHERE5Y, SPHERE5Z), SPHERE5RADIUS);
	sphere5->setColor(glm::vec3(1, 0, 0));
	sphere5->setRefractivity(true);

	//--Refractive Sphere 2
	Sphere* sphere6 = new Sphere(glm::vec3(SPHERE6X, SPHERE6Y, SPHERE6Z), SPHERE6RADIUS);
	sphere6->setColor(glm::vec3(1, 0, 0));
	sphere6->setRefractivity(true);

	//--Push to scene objects
	sceneObjects.push_back(wall);		//index 0
	sceneObjects.push_back(plane);		//index 1
	sceneObjects.push_back(sphere1);	//index 2
	sceneObjects.push_back(sphere2);	//index 3
	sceneObjects.push_back(sphere3);	//index 4
	sceneObjects.push_back(cylinder1);	//index 5
	sceneObjects.push_back(cylinder2);	//index 6
	sceneObjects.push_back(cylinder3);	//index 7
	sceneObjects.push_back(cone1);		//index 8
	sceneObjects.push_back(cone2);		//index 9
	sceneObjects.push_back(sphere4);	//index 10
	sceneObjects.push_back(sphere5);	//index 11
	sceneObjects.push_back(sphere6);	//index 12

	//--Cube
	createCube(CUBEX, CUBEY, CUBEZ, CUBELENGTH, glm::vec3(0.0, 0.5, 0.5));
	//--Octahedron
	createOctahedron(OCTAHEDRONX, OCTAHEDRONY, OCTAHEDRONZ, OCTAHEDRONLENGTH, glm::vec3(0.0, 0.5, 0.5));
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Raytracing");

    glutDisplayFunc(display);
    initialize();

    glutMainLoop();
    return 0;
}
