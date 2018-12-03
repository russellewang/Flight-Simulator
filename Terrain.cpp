#include "Terrain.h"
#include "Window.h"
#define WIDTH 128

Terrain::Terrain() {
	
	toWorld = glm::mat4(1.0f);
	/*
	vertices.push_back(glm::vec3(0.0, 0.0, 0.0));
	vertices.push_back(glm::vec3(2, 1.0, 0));
	vertices.push_back(glm::vec3(0, 2.0, 2));
	vertices.push_back(glm::vec3(2, 3.0, 2));
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);
	indices.push_back(3);
	indices.push_back(2);
	indices.push_back(0);
	*/
	//generate height map
	
	for (int i = 0; i < WIDTH; i++) {
		
		for (int j = 0; j < WIDTH; j++) {
			vertices.push_back(glm::vec3((float)j - (WIDTH/2), 0.0, (float)i - (WIDTH / 2)));
			normals.push_back(glm::vec3(0.0, 1.0, 0.0));
			textures.push_back(glm::vec2(j, i ));
			if (j != WIDTH - 1 || i != WIDTH - 1) {
				int bLeft = i * WIDTH + j;
				int tLeft = (i+1)* WIDTH + j;
				int tRight = tLeft + 1;
				int bRight = bLeft + 1;
				indices.push_back(tLeft);
				indices.push_back(bLeft);
				indices.push_back(tRight);
				indices.push_back(tRight);
				indices.push_back(bLeft);
				indices.push_back(bRight);
			}
			
			//cout << vertices[j].x << +"...." << vertices[j].z << endl;
		}
		
	}
	
	setUpTerrain();
}


Terrain::~Terrain() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Terrain::setUpTerrain() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);

	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindVertexArray(0);
}

unsigned char* Terrain::loadPPM(const char* filename, int& width, int& height) {

}



void Terrain::draw(GLuint shaderProgram) {
	glm::mat4 modelview = Window::V * toWorld;


	// Now send these values to the shader program
	//glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &toWorld[0][0]);
	//glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &Window::V[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "modelview"), 1, GL_FALSE, &modelview[0][0]);

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &Window::P[0][0]);

	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, (GLsizei) indices.size(), GL_UNSIGNED_INT, 0);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}