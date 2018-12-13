#include "City.h"




City::City(std::vector<Transform*>* objects) : Transform() {

	// Zones Structure
	Zones zone;

	// Generate seed for random number generation using time
	srand(time(0));

	// For Loops simulate city streets.  Determine x, z coordinates of center of city lots.
	for (float x = zone.CCCoordinates.x - cityXRadius * cityScale; x < zone.CCCoordinates.x + cityXRadius * cityScale; x += lotCloseness * cityScale) {
		for (float z = zone.CCCoordinates.z - cityZRadius * cityScale; z < zone.CCCoordinates.z + cityZRadius * cityScale; z += lotCloseness * cityScale) {
			
			// Compute Distance from city center (city origin)
			float distance = sqrt(pow(zone.CCCoordinates.x - x,2) + pow(zone.CCCoordinates.z - z, 2));

			// Set city zone based on distance from city center
			int zoneNum = setZone(distance, zone);
			
			// Set bldgType based on city zone
			int bldgType = setBldgType(zoneNum);

			// Set Grammar based on bldgType
			int grammar = setGrammar(bldgType);

			// Set building height based on city zone
			int height = setHeight(zoneNum, zone) * cityScale;

			// Building Grammar dependent on zone and bldg type
			int base = rand() % bases.size();

			// city y coordinates
			float y = zone.CCCoordinates.y;

			// Construct building using appropriate building grammar
			(*bldgGrammar[bldgType][grammar])( x,  y,  z, bases,  base,  zoneNum,  height, objects);

		}
	}

}

City::~City() {

}

void City::draw(Camera* C, GLuint shader) {
	
}

int City::setZone(float distance, Zones zone)
{
	if (distance <= zone.CCRadiusMax * cityScale) {
		return 0;
	}
	else if (distance <= zone.DTRadiusMax * cityScale) {
		return 1;
	}
	else if (distance <= zone.MTRadiusMax * cityScale) {
		return 2;
	}
	else {
		return 3;
	}
}

int City::setHeight(int zoneNum, Zones zone)
{
	if (zoneNum == 0) {
		return (rand() % zone.CCFloorMax + zone.CCFloorMin);
	}
	else if (zoneNum == 1) {
		return (rand() % zone.DTFloorMax + zone.DTFloorMin);
	}
	else if (zoneNum == 2) {
		return (rand() % zone.MTFloorMax + zone.MTFloorMin);
	}
	else {
		return (rand() % zone.SUBFloorMax + zone.SUBFloorMin);
	}
}

int City::setBldgType(int zoneNum)
{

	int result = rand() % Zoning[zoneNum].size();
	return Zoning[zoneNum][result];
}

int City::setGrammar(int bldgType)
{
	int result = rand() % Grammars[bldgType].size();

	return Grammars[bldgType][result];

}

// SkyScraper Grammars
void sg1(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects)
{
	
	float xscale = 1.0f;
	float yscale = 1.0f;
	Building* b1;
	if (rand() % 4 == 0) {
		xscale = 0.75f;
		yscale = 0.25f;
		b1 = new Building(bases[base].first, &glm::vec3(bases[base].second->x + xscale, bases[base].second->y , bases[base].second->z - yscale), height);
		if (rand() % 4 == 0) {
			b1->SetRotation(glm::vec3(0.0f, 1.0f, 0.0f));
		}
	}
	else
	{
		b1 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * 2.0f, bases[base].second->y, bases[base].second->z* 2.0f), height);
		if (rand() % 4 == 0) {
			b1->SetRotation(glm::vec3(0.0f, 1.0f, 0.0f));
		}
	}

	b1->SetPosition(glm::vec3(x, y, z));
	objects->push_back((Transform*)b1);
}

void sg2(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects)
{
	float xscale = 1.25f;
	float zscale = 1.25f;
	float rescale = 0.65f;
	float translate = 1.0f;
	
	Building* b1 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * xscale, bases[base].second->y, bases[base].second->z * zscale), height);
	b1->SetPosition(glm::vec3(x, y, z));
	objects->push_back((Transform*)b1);

	int rule = rand() % 8;
	if (rule == 0) {

		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * xscale, bases[base].second->y, bases[base].second->z * zscale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b2);
	}
	else if (rule == 1)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * xscale, bases[base].second->y, bases[base].second->z * zscale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b2);
	}
	else if (rule == 2)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * xscale, bases[base].second->y, bases[base].second->z * zscale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x - translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b2);
	}
	else if (rule == 3)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * xscale, bases[base].second->y, bases[base].second->z * zscale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x - translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b2);
	}
	else if (rule == 4)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * xscale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + translate, y - 0.01f, z));
		objects->push_back((Transform*)b2);
	}
	else if (rule == 5)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x - translate, y - 0.01f, z));
		objects->push_back((Transform*)b2);
	}
	else if (rule == 6)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x, y - 0.01f, z + translate));
		objects->push_back((Transform*)b2);
	}
	else if (rule == 7)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x, y - 0.01f, z - translate));
		objects->push_back((Transform*)b2);
	}
	
	
}

void sg3(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects)
{
	float xscale = 1.25f;
	float zscale = 1.25f;
	float rescale = 0.65f;
	float translate = 1.0f;

	if (rand() % 2 == 0)
	{
		xscale = 1.5f;
		zscale = 1.5f;
	}

	Building* b1 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * xscale, bases[base].second->y, bases[base].second->z * zscale), height);
	b1->SetPosition(glm::vec3(x, y, z));
	objects->push_back((Transform*)b1);

	int rule = rand() % 8;
	if (rule == 0) {

		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + translate, y - 0.01f, z));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x - translate, y - 0.01f, z));
		objects->push_back((Transform*)b3);
	}
	else if (rule == 1)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x - translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b3);
	}
	else if (rule == 2)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x , y - 0.01f, z + translate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x , y - 0.01f, z - translate));
		objects->push_back((Transform*)b3);
	}
	else if (rule == 3)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x - translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x + translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b3);
	}
	else if (rule == 4)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x - translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b3);
	}
	else if (rule == 5)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x + translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b3);
	}
	else if (rule == 6)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x - translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b3);
	}
	else if (rule == 7)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x - translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x - translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b3);
	}
}

void sg5(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects)
{
	int rule = rand() % 5;
	//int rule = 4;

	if (rule == 0) { 
		float xscale = 1.25f;
		float zscale = 1.25f;
		float rescale = 0.75f;
		float translate = 1.0f;

		Building* b1 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * xscale, bases[base].second->y, bases[base].second->z * zscale), height);
		b1->SetPosition(glm::vec3(x, y, z));
		objects->push_back((Transform*)b1);

		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + translate, y - 0.01f, z));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x - translate, y - 0.01f, z));
		objects->push_back((Transform*)b3);

		Building* b4 = new Building(*b2);
		b4->SetPosition(glm::vec3(x , y - 0.01f, z + translate));
		objects->push_back((Transform*)b4);

		Building* b5 = new Building(*b2);
		b5->SetPosition(glm::vec3(x , y - 0.01f, z - translate));
		objects->push_back((Transform*)b5);

	}
	else if (rule == 1)
	{
		float xscale = 1.25f;
		float zscale = 1.25f;
		float rescale = 0.75f;
		float translate = 1.0f;

		Building* b1 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * xscale, bases[base].second->y, bases[base].second->z * zscale), height);
		b1->SetPosition(glm::vec3(x, y, z));
		objects->push_back((Transform*)b1);

		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.60f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x + translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b3);

		Building* b4 = new Building(*b2);
		b4->SetPosition(glm::vec3(x - translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b4);

		Building* b5 = new Building(*b2);
		b5->SetPosition(glm::vec3(x - translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b5);
	}
	else if (rule == 2)
	{
		
		float xscale = 1.25f;
		float zscale = 0.0f;
		float rescale = 0.66f;
		float xtranslate = 1.0f;
		float ztranslate = 1.0f;

		Building* b1 = new Building(bases[0].first, &glm::vec3(bases[0].second->x + 2.0f, bases[0].second->y, bases[0].second->z), height);
		b1->SetPosition(glm::vec3(x, y, z));
		objects->push_back((Transform*)b1);
		
		Building* b2 = new Building(bases[6].first, &glm::vec3(bases[6].second->x * rescale, bases[6].second->y, bases[6].second->z * rescale), height *= 0.80f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + xtranslate, y - 0.01f, z + ztranslate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x + xtranslate, y - 0.01f, z - ztranslate));
		objects->push_back((Transform*)b3);

		Building* b4 = new Building(*b2);
		b4->SetPosition(glm::vec3(x - xtranslate, y - 0.01f, z + ztranslate));
		objects->push_back((Transform*)b4);

		Building* b5 = new Building(*b2);
		b5->SetPosition(glm::vec3(x - xtranslate, y - 0.01f, z - ztranslate));
		objects->push_back((Transform*)b5);
	}
	else if (rule == 3)
	{
		float xscale = 1.5f;
		float zscale = 0.75f;
		float rescale = 1.0f;
		float xtranslate = 1.5f;

		Building* b1 = new Building(bases[0].first, &glm::vec3(bases[0].second->x + xscale, bases[0].second->y, bases[0].second->z - zscale), height);
		b1->SetPosition(glm::vec3(x, y, z));
		objects->push_back((Transform*)b1);

		Building* b2 = new Building(bases[0].first, &glm::vec3(bases[0].second->x * rescale, bases[0].second->y, bases[0].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + xtranslate, y - 0.01f, z));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x - xtranslate, y - 0.01f, z));
		objects->push_back((Transform*)b3);
	}
	else if (rule == 4)
	{
	float xscale = 1.5f;
	float zscale = 0.75f;
	float rescale = 1.5f;

		Building* b1 = new Building(bases[0].first, &glm::vec3(bases[0].second->x + xscale, bases[0].second->y, bases[0].second->z - zscale), height);
		b1->SetPosition(glm::vec3(x, y, z));
		objects->push_back((Transform*)b1);

		Building* b2 = new Building(bases[4].first, &glm::vec3(bases[4].second->x * rescale, bases[4].second->y, bases[4].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x , y - 0.01f, z));
		objects->push_back((Transform*)b2);
	}
}


void obg1(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects)
{
	Building* b1;
	if (rand() % 4 == 0) {
	
		b1 = new Building(bases[base].first, &glm::vec3(bases[base].second->x, bases[base].second->y, bases[base].second->z), height);
		if (rand() % 4 == 0) {
			b1->SetRotation(glm::vec3(0.0f, 2.0f, 0.0f));
		}
	}
	else
	{
		b1 = new Building(bases[base].first, &glm::vec3(bases[base].second->x , bases[base].second->y, bases[base].second->z ), height);
		if (rand() % 4 == 0) {
			b1->SetRotation(glm::vec3(0.0f, 1.0f, 0.0f));
		}
	}

	b1->SetPosition(glm::vec3(x, y, z));
	objects->push_back((Transform*)b1);
}

void obg2(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects)
{
	float xscale = 0.9f;
	float zscale = 0.9f;
	float rescale = 0.65f;
	Building* b1 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * xscale, bases[base].second->y, bases[base].second->z * zscale), height);
	b1->SetPosition(glm::vec3(x, y, z));
	objects->push_back((Transform*)b1);

	int rule = rand() % 8;
	if (rule == 0) {

		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * xscale, bases[base].second->y, bases[base].second->z * zscale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + 0.75f, y - 0.01f, z + 0.75f));
		objects->push_back((Transform*)b2);
	}
	else if (rule == 1)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * xscale, bases[base].second->y, bases[base].second->z * zscale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + 0.75f, y - 0.01f, z - 0.75f));
		objects->push_back((Transform*)b2);
	}
	else if (rule == 2)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * xscale, bases[base].second->y, bases[base].second->z * zscale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x - 0.75f, y - 0.01f, z + 0.75f));
		objects->push_back((Transform*)b2);
	}
	else if (rule == 3)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * xscale, bases[base].second->y, bases[base].second->z * zscale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x - 0.75f, y - 0.01f, z - 0.75f));
		objects->push_back((Transform*)b2);
	}
	else if (rule == 4)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + 0.75f, y - 0.01f, z));
		objects->push_back((Transform*)b2);
	}
	else if (rule == 5)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x - 0.75f, y - 0.01f, z));
		objects->push_back((Transform*)b2);
	}
	else if (rule == 6)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x, y - 0.01f, z + 0.75f));
		objects->push_back((Transform*)b2);
	}
	else if (rule == 7)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x, y - 0.01f, z - 0.75f));
		objects->push_back((Transform*)b2);
	}

}

void obg3(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects)
{
	float xscale = 1.0f;
	float zscale = 1.0f;
	float rescale = 0.65f;
	float translate = 1.0f;

	if (rand() % 2 == 0)
	{
		xscale = 1.25f;
		zscale = 1.25f;
	}

	Building* b1 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * xscale, bases[base].second->y, bases[base].second->z * zscale), height);
	b1->SetPosition(glm::vec3(x, y, z));
	objects->push_back((Transform*)b1);

	int rule = rand() % 8;
	if (rule == 0) {

		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + translate, y - 0.01f, z));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x - translate, y - 0.01f, z));
		objects->push_back((Transform*)b3);
	}
	else if (rule == 1)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x - translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b3);
	}
	else if (rule == 2)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x, y - 0.01f, z + translate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x, y - 0.01f, z - translate));
		objects->push_back((Transform*)b3);
	}
	else if (rule == 3)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x - translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x + translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b3);
	}
	else if (rule == 4)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x - translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b3);
	}
	else if (rule == 5)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x + translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b3);
	}
	else if (rule == 6)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x + translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x - translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b3);
	}
	else if (rule == 7)
	{
		Building* b2 = new Building(bases[base].first, &glm::vec3(bases[base].second->x * rescale, bases[base].second->y, bases[base].second->z * rescale), height *= 0.75f);
		b2->loadTexture(b1->texfile);
		b2->SetPosition(glm::vec3(x - translate, y - 0.01f, z + translate));
		objects->push_back((Transform*)b2);

		Building* b3 = new Building(*b2);
		b3->SetPosition(glm::vec3(x - translate, y - 0.01f, z - translate));
		objects->push_back((Transform*)b3);
	}
}

void h1(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects)
{
	float xscale = 0.5f;
	float zscale = 0.5f;
	Building* b1;
	
	b1 = new Building(bases[base].first, &glm::vec3(bases[base].second->x + xscale, bases[base].second->y, bases[base].second->z - zscale), height);
	if ((float)(rand() % 4) == 0) {
		b1->SetRotation(glm::vec3(0.0f, 2.0f , 0.0f));
	}
	b1->SetPosition(glm::vec3(x, y, z));
	objects->push_back((Transform*)b1);
}

void ng(float x, float y, float z, std::vector< std::pair<const char*, glm::vec3*>> bases, int base, int zoneNum, int height, std::vector<Transform*>* objects)
{
	// No grammar, don't render anything
}