/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The Plane class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#ifndef H_PLANE
#define H_PLANE

#include <glm/glm.hpp>
#include "SceneObject.h"

class Plane : public SceneObject
{
private:
	glm::vec3 a_ = glm::vec3(0);   //The vertices of the quad
	glm::vec3 b_ = glm::vec3(0);
	glm::vec3 c_ = glm::vec3(0);
	glm::vec3 d_ = glm::vec3(0);
	int nverts_ = 4;				//Number of vertices (3 or 4)

public:	
	Plane() = default;
	
	Plane(glm::vec3 pa, glm::vec3 pb, glm::vec3 pc, glm::vec3 pd) : 
		a_(pa), b_(pb), c_(pc), d_(pd), nverts_(4) {}

	Plane(glm::vec3 pa, glm::vec3 pb, glm::vec3 pc) :
		a_(pa), b_(pb), c_(pc),  nverts_(3) {}


	bool isInside(glm::vec3 pt);
	
	float intersect(glm::vec3 posn, glm::vec3 dir);

	int getNumVerts();
	
	glm::vec3 normal(glm::vec3 pt);

};

#endif //!H_PLANE
