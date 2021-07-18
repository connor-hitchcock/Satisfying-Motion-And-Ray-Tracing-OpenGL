/*-------------------------------------------------------------
* Author: Connor Hitchcock
*
* The class for a cylinder object.
* A subclass of Object, implements intersect() and normal().
*------------------------------------------------------------*/

#ifndef H_CYLINDER
#define H_CYLINDER
#include <glm/glm.hpp>
#include "SceneObject.h"

/**
 * Defines a simple Cylinder located at 'center'
 * with the specified radius
 */
class Cylinder : public SceneObject
{

private:
    glm::vec3 center;
    float radius;
    float height;

public:
    Cylinder() : center(glm::vec3(0)), radius(1.5), height(1.5) {};

    Cylinder(glm::vec3 c, float r, float h) : center(c), radius(r), height(h) {}

    float intersect(glm::vec3 pos, glm::vec3 dir);

    glm::vec3 normal(glm::vec3 p);

};

#endif //!H_CYLINDER
