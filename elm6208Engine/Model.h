#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>
#include <string>
#include <vector>
#include "ShaderManager.h"
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
using namespace std;

struct Vertex {
	glm::vec3 location;
	glm::vec2 uv;
	glm::vec3 normal;
};
/*struct Vertex {
	glm::vec3 loc;
	glm::vec2 uv;

	Vertex() {
		loc = { 0, 0, 0 };
		uv = { 0, 0 };
	}

	Vertex(glm::vec3 loc1, glm::vec2 uv1) {
		loc = loc1;
		uv = uv1;
	}
};*/

struct VertInd {
	unsigned int locInd;
	unsigned int uvInd;
	unsigned int normInd;
};

class Model
{
private:
	GLuint vertArr;
	unsigned int vertCount;

public:
	Model();
	Model(std::string obFil);
	~Model();
	std::string objFile;
	bool buffer();
	void render();
	
};

