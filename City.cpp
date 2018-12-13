#include "City.h"
#include <tuple> 


City::City(std::vector<Transform*>* objects) : Transform() {

	// Zones Structure
	Zones zone;

	srand(time(0));

    //std::vector< std::pair<const char*, glm::vec3*>> bases;

    glm::vec3 object1 = glm::vec3(2.0f, 2.0f, 2.0f) * cityScale;
    glm::vec3 object2 = glm::vec3(1.0f, 1.0f, 1.0f) * cityScale;
    glm::vec3 object3 = glm::vec3(20.0f, 20.0f, 20.0f) * cityScale;
    
	/* shapes contains a list of objects/scale values to use in building construction */
    bases.push_back(std::make_pair("../cube.obj", &object1));
    bases.push_back(std::make_pair("../body_s.obj", &object2));
    bases.push_back(std::make_pair("../eyeball_s.obj", &object2));
	//bases.push_back(std::make_pair("cube.obj", &(glm::vec3(2.0f, 2.0f, 2.0f) * cityScale)));
	//bases.push_back(std::make_pair("body_s.obj", &(glm::vec3(1.0f, 1.0f, 1.0f) * cityScale)));
	//bases.push_back(std::make_pair("eyeball_s.obj", &(glm::vec3(20.0f, 20.0f, 20.0f) * cityScale)));



	for (float x = zone.CCCoordinates.x - cityXRadius * cityScale; x < zone.CCCoordinates.x + cityXRadius * cityScale; x += lotCloseness * cityScale) {
		for (float z = zone.CCCoordinates.z - cityZRadius * cityScale; z < zone.CCCoordinates.z + cityZRadius * cityScale; z += lotCloseness * cityScale) {
			

			int base = rand() % bases.size();
			//Building* b = new Building(bases[base].first, bases[base].second);
			//b->SetPosition(glm::vec3(x, zone.CCCoordinates.y , z));
			//objects->push_back((Transform*)b);


			// Compute Distance from city center (city origin)
			float distance = sqrt(pow(zone.CCCoordinates.x - x,2) + pow(zone.CCCoordinates.z - z, 2));
			int height;
			int zoneNum;

			// Assign building height depending on zone building is located
			if (distance <= zone.CCRadiusMax * cityScale) {
				height = rand() % zone.CCFloorMax + zone.CCFloorMin;
				zoneNum = 0;
			}
			else if (distance <= zone.DTRadiusMax * cityScale) {
				height = rand() % zone.DTFloorMax + zone.DTFloorMin;
				zoneNum = 1;
			}
			else if (distance <= zone.MTRadiusMax * cityScale) {
				height = rand() % zone.MTFloorMax + zone.MTFloorMin;
				zoneNum = 2;
			}
			else
			{
				height = rand() % zone.SUBFloorMax + zone.SUBFloorMin;
				zoneNum = 3;

			}

			Building* b2 = new Building(bases[base].first, bases[base].second, zoneNum, height * cityScale);
			b2->SetPosition(glm::vec3(x, zone.CCCoordinates.y, z));
			objects->push_back((Transform*)b2);
/*
			for (float y = zone.CCCoordinates.y; y < zone.CCCoordinates.y + (2 + height)* cityScale; y += 2 * cityScale) {
				Building* b2 = new Building(bases[base].first, bases[base].second, distance, height);
				b2->SetPosition(glm::vec3(x, y, z));
				objects->push_back((Transform*)b2);
			}
*/
		}
	}

}

City::~City() {

}

void City::draw(Camera* C, GLuint shader) {
	
}


