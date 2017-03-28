#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>
#include <vector>
#include "Engine.h"


int main()
{
	Engine engine;
	if (!engine.init()) return -1;
	if (!engine.bufferModels()) return -1;
	engine.useShaders();
	engine.gameLoop();
	return 0;

}