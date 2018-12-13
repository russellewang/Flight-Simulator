#ifndef City_h
#define City_h

#include "Transform.hpp"
#include "Window.h"
#include "Camera.h"

#include <vector>
#include <tuple> 
#include <utility>  
#include <cmath>
#include <time.h>       /* time */

class City : public Transform {
private:
	float cityScale = 2.0f;
	float cityXRadius = 50.0f;
	float cityZRadius = 50.0f;
	float lotCloseness = 5.0f;
	std::vector<Transform*> buildings;
	std::vector< std::pair<const char*, glm::vec3*>> bases;

	struct Zones {
		
		//City Center
		glm::vec3 CCCoordinates = glm::vec3(27.5f, 5.0f, 0.0f);
		float CCRadiusMax = 5.0f;
		int CCFloorMax = 20;
		int CCFloorMin = 15;

		//DownTown
		float DTRadiusMax = 20.0f;
		int DTFloorMax = 12;
		int DTFloorMin = 8;

		//Midtown
		float MTRadiusMax = 40.0f;
		int MTFloorMax = 8;
		int MTFloorMin = 6;

		//Suburbs
		int SUBFloorMax = 3;
		int SUBFloorMin = 1;

	};

public:
	void draw(Camera* camera, GLuint shader);

	City(std::vector<Transform*>*);

	~City();

};

#endif /* Geometry_hpp */
