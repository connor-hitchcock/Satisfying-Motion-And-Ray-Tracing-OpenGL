/*----------------------------------------------------------
* COSC363  Ray Tracer
* CSSE, University of Canterbury.
*
*  The object class
*  This is a generic type for storing objects in the scene.
*  Being an abstract class, this class cannot be instantiated.
*  Sphere, Plane etc, must be defined as subclasses of Object
*      and provide implementations for the virtual functions
*      intersect()  and normal().
-------------------------------------------------------------*/

#ifndef H_SOBJECT
#define H_SOBJECT
#include <glm/glm.hpp>


class SceneObject 
{
protected:
	glm::vec3 color_ = glm::vec3(1);  //material color
	bool refl_ = false;  //reflectivity: true/false
	bool refr_ = false;  //refractivity: true/false
	bool spec_ = true;   //specularity: true/false
	bool tran_ = false;  //transparency: true/false
	float reflc_ = 0.8;  //coefficient of reflection
	float refrc_ = 0.8;  //coefficient of refraction
	float tranc_ = 0.8;  //coefficient of transparency
	float refri_ = 1.0;  //refractive index
	float shin_ = 50.0; //shininess
public:
	SceneObject() {}
    virtual float intersect(glm::vec3 p0, glm::vec3 dir) = 0;
	virtual glm::vec3 normal(glm::vec3 pos) = 0;
	virtual ~SceneObject() {}

	glm::vec3 lighting(glm::vec3 lightPos, glm::vec3 viewVec, glm::vec3 hit);
	void setColor(glm::vec3 col);
	void setReflectivity(bool flag);
	void setReflectivity(bool flag, float refl_coeff);
	void setRefractivity(bool flag);
	void setRefractivity(bool flag, float refr_coeff, float refr_indx);
	void setShininess(float shininess);
	void setSpecularity(bool flag);
	void setTransparency(bool flag);
	void setTransparency(bool flag, float tran_coeff);
	glm::vec3 getColor();
	float getReflectionCoeff();
	float getRefractionCoeff();
	float getTransparencyCoeff();
	float getRefractiveIndex();
	float getShininess();
	bool isReflective();
	bool isRefractive();
	bool isSpecular();
	bool isTransparent();
};

#endif