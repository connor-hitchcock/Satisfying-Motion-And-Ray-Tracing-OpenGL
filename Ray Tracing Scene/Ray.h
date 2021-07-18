/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The ray class
-------------------------------------------------------------*/

#ifndef H_RAY
#define H_RAY
#include <glm/glm.hpp>
#include <vector>
#include "SceneObject.h"

class Ray
{

public:
	glm::vec3 p0 = glm::vec3(0);		//The source point of the ray
	glm::vec3 dir = glm::vec3(0,0,-1);	//The UNIT direction of the ray
	glm::vec3 hit = glm::vec3(0);		//The closest point of intersection on the ray
	int index = -1;						//The index of the object that gives the closet point of intersection
	float dist = 0;						//The distance from the p0 to hit along the ray.

	Ray() {}		//Default constructor


	Ray(glm::vec3 source, glm::vec3 direction)
	{
		const float RSTEP = 0.005f;
		p0 = source;
		dir = glm::normalize(direction);
		p0 = p0 + RSTEP * dir;   //Ray stepping
	}

	void closestPt(std::vector<SceneObject*>& sceneObjects);

};
#endif
