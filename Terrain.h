#ifndef TERRAIN_H
#define TERRAIN_H

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>   
#include <iostream> 

using namespace std;

class Terrain {

private:
	vector<glm::vec3> vertices;
	vector<glm::vec3> normals;
	vector<glm::vec2> textures;
	vector<unsigned int> indices;
	GLuint VAO, VBO, EBO;
	float * heightMap;
	void setUpTerrain();

public:

	glm::mat4 toWorld;

	Terrain();
	~Terrain();
	unsigned char* loadPPM(const char* filename, int& width, int& height);
	void draw(GLuint);




};


#endif