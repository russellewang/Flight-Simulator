#include "Building.h"

/* Types of possible buildings in each zone,  Multiple entries are used to have weighted random building chances
	0. Skyscapers
	1. Office Buildings
	2. Houses
	*/
int skyscraper_Grammar = 0;
int officeBldg_Grammar = 1;
int house_Grammar = 2;
std::vector<int> zone1_ShapeGrammar = { 0,1,1,1 }; // 1/4 chance for skyscraper in zone downtown
std::vector<int> zone2_ShapeGrammar = { 0,1,1,1,1,1,1,1 }; // 1/8 chance for skyscraper in midtown
std::vector<int> zone3_ShapeGrammar = { 1,2,2,2 }; // 1/4 chance for office building in suburbs

std::vector<int> skyscraper_angles = { 0, 0, 45, 90, 135};
std::vector<int> officeBldg_angles = { 0, 0, 0, 0, 0, 45, 90, 135 };
std::vector<int> house_angles = { 0, 0, 0, 90 }; // 1/4 chance to rotate 90 degrees.
float squishFactor;
float angle;

Building::Building(const char * filename, glm::vec3* scaleSize, float zone, int height) : Transform() {


	// Choose shape grammar based on city zone
	if (zone == 0)
	{
		skyscraper(filename, scaleSize, height);
	}
	else if (zone == 1)
	{
		int buildGrammar = zone1_ShapeGrammar[rand() % zone1_ShapeGrammar.size()];

		if (buildGrammar == skyscraper_Grammar)
		{
			skyscraper(filename, scaleSize, height);
		}
		else if (buildGrammar == officeBldg_Grammar)
		{
			officeBldg(filename, scaleSize, height);
		}
	}
	else if (zone == 2)
	{
		int buildGrammar = zone2_ShapeGrammar[rand() % zone2_ShapeGrammar.size()];

		if (buildGrammar == skyscraper_Grammar)
		{
			skyscraper(filename, scaleSize, height);
		}
		else if (buildGrammar == officeBldg_Grammar)
		{
			officeBldg(filename, scaleSize, height);
		}
	}
	else if (zone == 3)
	{
		int buildGrammar = zone3_ShapeGrammar[rand() % zone3_ShapeGrammar.size()];

		if (buildGrammar == officeBldg_Grammar)
		{
			officeBldg(filename, scaleSize, height);
		}
		else if (buildGrammar == house_Grammar)
		{
			house(filename, scaleSize, height);
		}
	}


}

Building::~Building() {
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}


void Building::skyscraper(const char* filename, glm::vec3* scaleSize, int height) {

	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
	FILE* fp;     // file pointer
	float x, y, z;  // vertex coordinates
	int c1, c2;    // characters read from file
	angle = (skyscraper_angles[rand() % skyscraper_angles.size()]) * (PI / 180.0f);
	squishFactor = (rand() / (float)RAND_MAX * 1.0f) + 0.5f;

	fp = fopen(filename, "rb");  // make the file name configurable so you can load other files
	if (fp == NULL) { std::cerr << "error loading file " << filename << std::endl; exit(-1); }  // just in case the file can't be found or is corrupt

	c1 = fgetc(fp);
	c2 = fgetc(fp);

	for (int i = 0; c1 != EOF; i++) {
		c1 = fgetc(fp);
		if (c1 != 'v' && c1 != 'f')
			continue;
		c2 = fgetc(fp);

		// read from file
		if ((c1 == 'v') && (c2 == ' ')) {
			fscanf(fp, "%f %f %f", &x, &y, &z);

			// Randomize building scale
			x *= squishFactor;

			float x1 = x * cos(angle) - z * sin(angle);
			float z1 = z * cos(angle) + x * sin(angle);

			glm::vec3 vi = (*scaleSize) * glm::vec3(x1, y, z1);

			if (y > 0)
			{
				vi.y = vi.y + height;
			}
			rawVertexInputs.push_back(vi);

		}
		else if ((c1 == 'v') && (c2 == 'n')) {
			fscanf(fp, "%f %f %f", &x, &y, &z);
			rawNormalInputs.push_back(glm::vec3(x, y, z));
		}
		else if ((c1 == 'v') && (c2 == 't')) {
			fscanf(fp, "%f %f %f", &x, &y, &z);
			rawTextureInputs.push_back(glm::vec3(x, y*0.2f, z));
		}
		else if ((c1 == 'f') && (c2 == ' ')) {
			unsigned int v1, v2, v3, vt1, vt2, vt3; // indices for face values
			unsigned int vn1, vn2, vn3; // throwaway values
			fscanf(fp, "%u/%u/%u %u/%u/%u %u/%u/%u", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
			//std::cout << v1 << "/" << vt1 << std::endl;
			// Due to openGL starting at index 0

			vertexIndices.push_back(v1 - 1);
			vertexIndices.push_back(v2 - 1);
			vertexIndices.push_back(v3 - 1);

			textureIndices.push_back(vt1 - 1);
			textureIndices.push_back(vt2 - 1);
			textureIndices.push_back(vt3 - 1);
		}
	}
	int size = (int)vertexIndices.size();
	for (int i = 0; i < size; i++) {
		vertices.push_back(rawVertexInputs[vertexIndices[i]]);
		vertices.push_back(rawTextureInputs[textureIndices[i]]);
		indices.push_back(i);
	}
	fclose(fp);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	// TEXTURES
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (const GLvoid*)12);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	loadTexture();
	//*scaleSize = glm::vec3(1.0f);
}

void Building::officeBldg(const char* filename, glm::vec3* scaleSize, int height) {
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
	FILE* fp;     // file pointer
	float x, y, z;  // vertex coordinates
	int c1, c2;    // characters read from file
	angle = (officeBldg_angles[rand() % officeBldg_angles.size()]) * (PI / 180.0f);
	squishFactor = (rand() / (float)RAND_MAX * 1.0f) + 0.7f;


	fp = fopen(filename, "rb");  // make the file name configurable so you can load other files
	if (fp == NULL) { std::cerr << "error loading file " << filename << std::endl; exit(-1); }  // just in case the file can't be found or is corrupt

	c1 = fgetc(fp);
	c2 = fgetc(fp);

	for (int i = 0; c1 != EOF; i++) {
		c1 = fgetc(fp);
		if (c1 != 'v' && c1 != 'f')
			continue;
		c2 = fgetc(fp);

		// read from file
		if ((c1 == 'v') && (c2 == ' ')) {
			fscanf(fp, "%f %f %f", &x, &y, &z);

			x *= squishFactor;

			float x1 = x * cos(angle) - z * sin(angle);
			float z1 = z * cos(angle) + x * sin(angle);

			glm::vec3 vi = (*scaleSize) * glm::vec3(x1, y, z1);

			if (y > 0)
			{
				vi.y = vi.y + height;
			}
			rawVertexInputs.push_back(vi);
		}
		else if ((c1 == 'v') && (c2 == 'n')) {
			fscanf(fp, "%f %f %f", &x, &y, &z);
			rawNormalInputs.push_back(glm::vec3(x, y, z));
		}
		else if ((c1 == 'v') && (c2 == 't')) {
			fscanf(fp, "%f %f %f", &x, &y, &z);
			rawTextureInputs.push_back(glm::vec3(x, y, z));
		}
		else if ((c1 == 'f') && (c2 == ' ')) {
			unsigned int v1, v2, v3, vt1, vt2, vt3; // indices for face values
			unsigned int vn1, vn2, vn3; // throwaway values
			fscanf(fp, "%u/%u/%u %u/%u/%u %u/%u/%u", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
			//std::cout << v1 << "/" << vt1 << std::endl;
			// Due to openGL starting at index 0

			vertexIndices.push_back(v1 - 1);
			vertexIndices.push_back(v2 - 1);
			vertexIndices.push_back(v3 - 1);

			textureIndices.push_back(vt1 - 1);
			textureIndices.push_back(vt2 - 1);
			textureIndices.push_back(vt3 - 1);
		}
	}
	int size = (int)vertexIndices.size();
	for (int i = 0; i < size; i++) {
		vertices.push_back(rawVertexInputs[vertexIndices[i]]);
		vertices.push_back(rawTextureInputs[textureIndices[i]]);
		indices.push_back(i);
	}
	fclose(fp);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	// TEXTURES
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (const GLvoid*)12);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	loadTexture();
	//*scaleSize = glm::vec3(1.0f);
}

void Building::house(const char* filename, glm::vec3* scaleSize, int height) {
	//TODO parse the OBJ file
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
	FILE* fp;     // file pointer
	float x, y, z;  // vertex coordinates
	int c1, c2;    // characters read from file
	angle = (house_angles[rand() % house_angles.size()]) * (PI / 180.0f);
	squishFactor = (rand() / (float)RAND_MAX * 1.0f) + 0.9f;

	fp = fopen(filename, "rb");  // make the file name configurable so you can load other files
	if (fp == NULL) { std::cerr << "error loading file " << filename << std::endl; exit(-1); }  // just in case the file can't be found or is corrupt

	c1 = fgetc(fp);
	c2 = fgetc(fp);

	for (int i = 0; c1 != EOF; i++) {
		c1 = fgetc(fp);
		if (c1 != 'v' && c1 != 'f')
			continue;
		c2 = fgetc(fp);

		// read from file
		if ((c1 == 'v') && (c2 == ' ')) {
			fscanf(fp, "%f %f %f", &x, &y, &z);

			x *= squishFactor;

			float x1 = x * cos(angle) - z * sin(angle);
			float z1 = z * cos(angle) + x * sin(angle);

			glm::vec3 vi = (*scaleSize) * glm::vec3(x1, y, z1);

			if (y > 0)
			{
				vi.y = vi.y + height;
			}
			rawVertexInputs.push_back(vi);

		}
		else if ((c1 == 'v') && (c2 == 'n')) {
			fscanf(fp, "%f %f %f", &x, &y, &z);
			rawNormalInputs.push_back(glm::vec3(x, y, z));
		}
		else if ((c1 == 'v') && (c2 == 't')) {
			fscanf(fp, "%f %f %f", &x, &y, &z);
			rawTextureInputs.push_back(glm::vec3(x, y, z));
		}
		else if ((c1 == 'f') && (c2 == ' ')) {
			unsigned int v1, v2, v3, vt1, vt2, vt3; // indices for face values
			unsigned int vn1, vn2, vn3; // throwaway values
			fscanf(fp, "%u/%u/%u %u/%u/%u %u/%u/%u", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
			//std::cout << v1 << "/" << vt1 << std::endl;
			// Due to openGL starting at index 0

			vertexIndices.push_back(v1 - 1);
			vertexIndices.push_back(v2 - 1);
			vertexIndices.push_back(v3 - 1);

			textureIndices.push_back(vt1 - 1);
			textureIndices.push_back(vt2 - 1);
			textureIndices.push_back(vt3 - 1);
		}
	}
	int size = (int)vertexIndices.size();
	for (int i = 0; i < size; i++) {
		vertices.push_back(rawVertexInputs[vertexIndices[i]]);
		vertices.push_back(rawTextureInputs[textureIndices[i]]);
		indices.push_back(i);
	}
	fclose(fp);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	// TEXTURES
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (const GLvoid*)12);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	loadTexture();
	//*scaleSize = glm::vec3(1.0f);
}

void Building::draw(Camera* C, GLuint shader) {
	// Set this texture to be the one we are working with
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	uProjection = glGetUniformLocation(shader, "projection");
	uModelView = glGetUniformLocation(shader, "modelview");

	glm::mat4 view = C->GetView();
	glm::mat4 projection = C->GetProjection();

	// Now send these values to the shader program
	glm::mat4 mv = view * GetToWorld();
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(uModelView, 1, GL_FALSE, &mv[0][0]);

	glUniform3f(glGetUniformLocation(shader, "camPos"), C->GetPosition().x, C->GetPosition().y, C->GetPosition().z);
	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, 0);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}

/** Load a ppm file from disk.
 @input filename The location of the PPM file.  If the file is not found, an error message
 will be printed and this function will return 0
 @input width This will be modified to contain the width of the loaded image, or 0 if file not found
 @input height This will be modified to contain the height of the loaded image, or 0 if file not found
 @return Returns the RGB pixel data as interleaved unsigned chars (R0 G0 B0 R1 G1 B1 R2 G2 B2 .... etc) or 0 if an error ocured
 **/
unsigned char* Building::loadPPM(const char* filename, int& width, int& height)
{
	const int BUFSIZE = 128;
	FILE* fp;
	unsigned int read;
	unsigned char* rawData;
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;

	if ((fp = fopen(filename, "rb")) == NULL)
	{
		std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
		width = 0;
		height = 0;
		return NULL;
	}

	// Read magic number:
	retval_fgets = fgets(buf[0], BUFSIZE, fp);

	// Read width and height:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	retval_sscanf = sscanf(buf[0], "%s %s", buf[1], buf[2]);
	width = atoi(buf[1]);
	height = atoi(buf[2]);

	// Read maxval:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');

	// Read image data:
	rawData = new unsigned char[width * height * 3];
	read = fread(rawData, width * height * 3, 1, fp);
	fclose(fp);
	if (read != 1)
	{
		std::cerr << "error parsing ppm file, incomplete data" << std::endl;
		delete[] rawData;
		width = 0;
		height = 0;
		return NULL;
	}

	return rawData;
}

// load image file into texture object
void Building::loadTexture()
{
	//GLuint texture[3];     // storage for one texture
	int twidth, theight;   // texture width/height [pixels]
	unsigned char* tdata;  // texture pixel data

	int index = rand() % skyscraper_textures.size();

	// Load image file
	tdata = loadPPM(skyscraper_textures[index], twidth, theight);
	if (tdata == NULL) {
		std::cout << "tdata is NULL" << std::endl;
		return;
	}
	
	// Create ID for texture1
	glGenTextures(1, &texture[0]);
	// Set this texture to be the one we are working with
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	// Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set bi-linear filtering for both minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/*
	tdata = loadPPM("frontendlarge.ppm", twidth, theight);
	if (tdata == NULL) {
		std::cout << "tdata is NULL" << std::endl;
		return;
	}
	// Create ID for texture2
	glGenTextures(1, &texture[1]);
	// Set this texture to be the one we are working with
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	// Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set bi-linear filtering for both minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	tdata = loadPPM("skyscraper.ppm", twidth, theight);
	if (tdata == NULL) {
		std::cout << "tdata is NULL" << std::endl;
		return;
	}
	// Create ID for texture 3
	glGenTextures(1, &texture[2]);
	// Set this texture to be the one we are working with
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	// Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set bi-linear filtering for both minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	*/
}