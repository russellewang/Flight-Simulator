//
//  Geometry.hpp
//  CSE 167 Example Code 2
//
//  Created by Petre on 10/30/18.
//  Copyright © 2018 Peter Enescu. All rights reserved.
//

#ifndef Building_h
#define Building_h

#include "Transform.hpp"
#include "Window.h"
#include "Camera.h"

#include <vector>
#include <math.h>

class Building : public Transform {
private:
	/* Raw data from obj */
	std::vector<glm::vec3> rawVertexInputs;
	std::vector<glm::vec3> rawNormalInputs;
	std::vector<glm::vec3> rawTextureInputs;
	/* Parsed f values v/vt/vn */
	std::vector<GLuint> vertexIndices;
	std::vector<GLuint> normalIndices;
	std::vector<GLuint> textureIndices;
	/* Aligned Data */
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> textures;
	std::vector<unsigned int> indices;

	// These variables are needed for the shader program
	GLuint VBO, VAO, EBO;
	GLuint uProjection, uModelView;
	const float PI = 3.1415927;
	GLuint texture[1];
	std::vector<const char *> skyscraper_textures = { 
		"skyscraperGlass.ppm", 
		"bwglass.ppm",
		"highRise.ppm",
		"1.ppm",
		"32.ppm",
		"33.ppm",
		"34.ppm",
		"59.ppm",
		"69.ppm",
		"71.ppm",
		"72.ppm",
		"76.ppm",
		"81.ppm",
	};
	glm::vec3 reshapeVertex(float , float , float , float , glm::vec3*);
	void loadBuffers();
	void parse(const char * , glm::vec3 * , int );
	void loadTexture();
	unsigned char* loadPPM(const char*, int&, int&);

public:
	void loadTexture(const char *);
	const char * texfile;

	void draw(Camera* camera, GLuint shader);

	Building(const char*, glm::vec3*, int);
	~Building();

};

#endif /* Geometry_hpp */
