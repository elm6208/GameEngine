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
#include "Model.h"

struct Transform {
	glm::vec3 loc;
	glm::vec3 rot;
	glm::vec3 size;
	glm::mat4 owMatrix;
};

struct RigidBody {
	glm::vec3 velocity;
	glm::vec3 force;
	float currSpeed;
	float mass;
};

struct Object {
	Transform tran;
	char* fileName;
	RigidBody rBody;
	//keeps track of whether the object should move up or down
	bool moveUp;
	enum collider { colliderless, AABB, sphere };
	collider c;
	Model mod;
};

struct Camera {

	// zoom or change aspect ratio
	float zoom = 1.f;
	int width = 800;
	int height = 600;

	float fovy = 3.14159f * .4f / zoom;
	float aspect = (float)width / (float)height;
	float zNear = .01f;
	float zFar = 1000.f;

	glm::mat4 perspectiveMat = glm::perspective(fovy, aspect, zNear, zFar);

	Transform cTran;
	RigidBody cRBody;
	
};

class Engine
{
private:
	GLFWwindow* GLFWwindowPtr;
	ShaderManager shaderMan;
	std::map<int, GLuint> textures;
	float currentTime;
	float previousTime;
	float deltaTime;

	
public:
	Engine();
	~Engine();
	void LoadTextures(char* fileName, int mapIndex);
	bool init();
	bool bufferModels();
	bool collides(Object & o1, Object & o2);
	bool gameLoop();
	bool useShaders();
	void UpdateCam(Camera &c);
	Object* objArr = new Object[5];
	
};

