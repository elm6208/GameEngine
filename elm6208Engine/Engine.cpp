#include "Engine.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

std::map<int, bool> keyIsDown;
void mouseClick(GLFWwindow *windowPtr,
	int button, int action, int mods)
{
	keyIsDown[button] = action;
}

void keyCallBack(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	keyIsDown[key] = action;
}







void Engine::UpdateCam(Camera & c)
{
	c.cRBody.velocity += (c.cRBody.force / c.cRBody.mass) *deltaTime;
	// this line is necessaruy
	c.cTran.loc += c.cRBody.velocity * deltaTime;
	glm::mat3 rotMat = (glm::mat3)glm::yawPitchRoll(c.cTran.rot.y, c.cTran.rot.x, c.cTran.rot.z);

	// Move or turn camera
	//glm::vec3 camLoc = { 0,0,2 };
	//glm::vec3 camRot = { 0,0,0 };
	//glm::mat3 cRotMat = (glm::mat3)glm::yawPitchRoll(camRot.y, camRot.x, camRot.z);

	glm::vec3 eye = c.cTran.loc;
	glm::vec3 center = eye + rotMat * glm::vec3(0, 0, -1);
	glm::vec3 up = rotMat * glm::vec3(0, 1, 0);

	glm::mat4 lookAtMat = glm::lookAt(eye, center, up);
	glm::mat4 camMat = c.perspectiveMat * lookAtMat;
	glUniformMatrix4fv(4, 1, GL_FALSE, &camMat[0][0]);
	glUniform3f(6, c.cTran.loc.x, c.cTran.loc.y, c.cTran.loc.z);
}



Engine::Engine()
{

}


Engine::~Engine()
{
}

bool Engine::init()
{
	if (glfwInit() == GL_FALSE)
		return false;

	GLFWwindowPtr = glfwCreateWindow(800, 600, "elm6208", NULL, NULL);

	if (GLFWwindowPtr != nullptr)
		glfwMakeContextCurrent(GLFWwindowPtr);
	else {
		glfwTerminate();
		return false;
	}

	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return false;
	}

	glfwSetMouseButtonCallback(GLFWwindowPtr, mouseClick);
	glfwSetKeyCallback(GLFWwindowPtr, keyCallBack);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	return true;
}

bool Engine::bufferModels()
{
	//set these to models other than the default
	//objArr[4].mod.objFile = "models/dog.obj";
	objArr[3].mod.objFile = "models/sphere.obj";
	bool buffBool;
	for (int i = 0; i < 5; i++)
	{
		buffBool = objArr[i].mod.buffer();
	}
	return buffBool;
	
}

void Engine::LoadTextures(char* fileName, int mapIndex)
{
	//Use FreeImage to load texture and check if it worked
	FIBITMAP* image = FreeImage_Load(FreeImage_GetFileType(fileName, 0), fileName);
	if (image == nullptr)
	{
		//load failed
	}

	// Use FreeImage to convert the original image to a 32bit bmp
	// then upload the original image from memory
	FIBITMAP* image32Bit = FreeImage_ConvertTo32Bits(image);
	FreeImage_Unload(image);

	//Generate and bind a new texture
	glGenTextures(1, &textures[mapIndex]);
	glBindTexture(GL_TEXTURE_2D, textures[mapIndex]);

	//upload texture bytes
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_SRGB_ALPHA, FreeImage_GetWidth(image32Bit), FreeImage_GetHeight(image32Bit), 0, GL_BGRA,
		GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(image32Bit));

	//Set min filter to linear instead of mipmap linear
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Clear the texture from RAM (not VRAM)
	FreeImage_Unload(image32Bit);

	//Unbind the texture when finished uploading
	glBindTexture(GL_TEXTURE_2D, 0);
}


//test for collisions
bool Engine::collides(Object & o1, Object & o2)
{
	if (o1.c == o1.colliderless || o2.c == o2.colliderless)
	{
		return false;
	}

	if (o1.c == o1.sphere && o2.c == o2.sphere)
	{
		//(x1 - x2)^2 + (y1-y2)^2
		if ((o1.tran.loc.x - o2.tran.loc.x) * (o1.tran.loc.x - o2.tran.loc.x) + (o1.tran.loc.y - o2.tran.loc.y) * (o1.tran.loc.y - o2.tran.loc.y) + (o1.tran.loc.z - o2.tran.loc.z) * (o1.tran.loc.z - o2.tran.loc.z) > (o1.tran.size.x + o2.tran.size.x) * (o1.tran.size.x + o2.tran.size.x))
		{
			return false;
		}
		
	}

	if (o1.c == o1.AABB && o2.c == o2.AABB)
	{
		if ((o1.tran.loc.x - o2.tran.loc.x) * (o1.tran.loc.x - o2.tran.loc.x) > (o1.tran.size.x + o2.tran.size.x) * (o1.tran.size.x + o2.tran.size.x))
		{
			return false;
		}
		if ((o1.tran.loc.y - o2.tran.loc.y) * (o1.tran.loc.y - o2.tran.loc.y) > (o1.tran.size.y + o2.tran.size.y) * (o1.tran.size.y + o2.tran.size.y))
		{
			return false;
		}
		if ((o1.tran.loc.z - o2.tran.loc.z) * (o1.tran.loc.z - o2.tran.loc.z) > (o1.tran.size.z + o2.tran.size.z) * (o1.tran.size.z + o2.tran.size.z))
		{
			return false;
		}
		
	}

	if (o1.c == o1.AABB && o2.c == o2.sphere)
	{
		//if((o1.tran.loc.x - o2.tran.loc.x) * (o1.tran.loc.x - o2.tran.loc.x) + (o1.tran.loc.y - o2.tran.loc.y) * (o1.tran.loc.y - o2.tran.loc.y))
		if ((o1.tran.loc.x - o2.tran.loc.x) * (o1.tran.loc.x - o2.tran.loc.x) > (o1.tran.size.x + o2.tran.size.x) * (o1.tran.size.x + o2.tran.size.x))
		{
			return false;
		}
		if ((o1.tran.loc.y - o2.tran.loc.y) * (o1.tran.loc.y - o2.tran.loc.y) > (o1.tran.size.y + o2.tran.size.y) * (o1.tran.size.y + o2.tran.size.y))
		{
			return false;
		}
		if ((o1.tran.loc.z - o2.tran.loc.z) * (o1.tran.loc.z - o2.tran.loc.z) > (o1.tran.size.z + o2.tran.size.z) * (o1.tran.size.z + o2.tran.size.z))
		{
			return false;
		}
		
	}

	if (o1.c == o1.sphere && o2.c == o2.AABB)
	{
		//glm::vec3 dist = (o1.tran.loc - o2.tran.loc) * (o1.tran.loc - o2.tran.loc);
		
		
		if ((o1.tran.loc.x - o2.tran.loc.x) * (o1.tran.loc.x - o2.tran.loc.x) > (o1.tran.size.x + o2.tran.size.x) * (o1.tran.size.x + o2.tran.size.x))
		{
			return false;
		}
		if ((o1.tran.loc.y - o2.tran.loc.y) * (o1.tran.loc.y - o2.tran.loc.y) > (o1.tran.size.y + o2.tran.size.y) * (o1.tran.size.y + o2.tran.size.y))
		{
			return false;
		}
		if ((o1.tran.loc.z - o2.tran.loc.z) * (o1.tran.loc.z - o2.tran.loc.z) > (o1.tran.size.z + o2.tran.size.z) * (o1.tran.size.z + o2.tran.size.z))
		{
			return false;
		}
		
	}

	return true;

}

bool Engine::gameLoop()
{
	std::cout << "The console will print 'colliding' if the player collides with an enemy." << std::endl;

	
	objArr[0].fileName = "images/dirt.png";
	objArr[0].tran.loc = glm::vec3(0, -.8, 0);
	objArr[0].tran.rot = glm::vec3(0, 0, 0);
	objArr[0].tran.size = glm::vec3(1, 0.5, 0);
	objArr[0].rBody.velocity = glm::vec3(0,0,0);
	objArr[0].rBody.force = glm::vec3(0,0,0);
	objArr[0].rBody.currSpeed = 0;
	objArr[0].rBody.mass = 1;
	objArr[0].c = objArr[0].AABB;
	

	objArr[1].fileName = "images/waluigi.png";
	objArr[1].tran.loc = glm::vec3(-.7, -.15, 0);
	objArr[1].tran.rot = glm::vec3(0, 0, 0);
	objArr[1].tran.size = glm::vec3(.15, .15, 0);
	objArr[1].rBody.velocity = glm::vec3(0, 0, 0);
	objArr[1].rBody.force = glm::vec3(0, 0, 0);
	objArr[1].rBody.currSpeed = 0;
	objArr[1].rBody.mass = 1;
	objArr[1].c = objArr[1].AABB;
	

	objArr[2].fileName = "images/bat1.jpg";
	objArr[2].tran.loc = glm::vec3(-.3, -.6, 0);
	objArr[2].tran.rot = glm::vec3(0, 0, 0);
	objArr[2].tran.size = glm::vec3(.1, .1, 0);
	objArr[2].rBody.velocity = glm::vec3(0, 0, 0);
	objArr[2].rBody.force = glm::vec3(0, 0, 0);
	objArr[2].rBody.currSpeed = 0;
	objArr[2].rBody.mass = 1;
	objArr[2].c = objArr[2].AABB;
	

	objArr[3].fileName = "images/bat3.jpg";
	objArr[3].tran.loc = glm::vec3(.3, .6, 0);
	objArr[3].tran.rot = glm::vec3(0, 0, 0);
	objArr[3].tran.size = glm::vec3(.1, .1, 0);
	objArr[3].rBody.velocity = glm::vec3(0, 0, 0);
	objArr[3].rBody.force = glm::vec3(0, 0, 0);
	objArr[3].rBody.currSpeed = 0;
	objArr[3].rBody.mass = 1;
	objArr[3].c = objArr[3].sphere;
	

	objArr[4].fileName = "images/bat2.jpg";
	objArr[4].tran.loc = glm::vec3(0, -.8, 0);
	objArr[4].tran.rot = glm::vec3(0, 0, 0);
	objArr[4].tran.size = glm::vec3(.1, .1, 0);
	objArr[4].rBody.velocity = glm::vec3(0, 0, 0);
	objArr[4].rBody.force = glm::vec3(0, 0, 0);
	objArr[4].rBody.currSpeed = 0;
	objArr[4].rBody.mass = 1;
	objArr[4].c = objArr[4].sphere;
	
	

	LoadTextures("images/dirt.png", 0);
	LoadTextures("images/waluigi.png", 1);
	LoadTextures("images/bat1.jpg", 2);
	LoadTextures("images/bat3.jpg", 3);
	LoadTextures("images/bat2.jpg", 4);

	Camera cam;

	//camera values
	cam.cTran.loc = glm::vec3(0, 0, 2);
	cam.cTran.rot = glm::vec3(0, .1, 0);
	cam.cTran.size = glm::vec3(0, 0, 0);
	cam.cRBody.force = glm::vec3(0, 0, 0);
	cam.cRBody.velocity = glm::vec3(0, 0, 0);
	cam.cRBody.mass = 1;

	int texIndex = 0;

	currentTime = (float)(glfwGetTime());

	bool jumping = false;
	for (int i = 0; i < 5; i++)
	{
		objArr[i].moveUp = false;
	}
	glm::vec3 lightL = { 0, .5, .7 };

	glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	// Loop until the user closes the window
	while (!glfwWindowShouldClose(GLFWwindowPtr))
	{
		// Update physical simulation
		previousTime = currentTime;
		currentTime = (float)(glfwGetTime());
		deltaTime = currentTime - previousTime;

		if (deltaTime > currentTime)
		{
			deltaTime = currentTime;
		}

		// test for collisions
		if (collides(objArr[1], objArr[2]))
		{
			std::cout << "colliding" << std::endl;
		}
		if (collides(objArr[1], objArr[3]))
		{
			std::cout << "colliding" << std::endl;
		}
		if (collides(objArr[1], objArr[4]))
		{
			std::cout << "colliding" << std::endl;
		}
		

		

		// Clear the canvas.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		for (int i = 0; i < 5; i++)
		{
			
			//player
			if (i == 1)
			{
				objArr[1].rBody.force.x = 0;
				objArr[1].rBody.force.y = 0;



				//make the player jump
				if (keyIsDown[GLFW_KEY_W] && jumping == false)
				{
					jumping = true;
					objArr[1].rBody.velocity.y = 3;
					
					
				}


				//test for input, move player left or right
				if (keyIsDown[GLFW_KEY_D])
				{
					//objArr[1].tran.loc.x += objArr[1].rBody.velocity.x * deltaTime;
					objArr[1].rBody.force.x = 1;
				}

				if (keyIsDown[GLFW_KEY_A])
				{
					//objArr[1].tran.loc.x -= objArr[1].rBody.velocity.x * deltaTime;
					objArr[1].rBody.force.x = -1;
				}
				
				//if not pressing a button, slow down
				if (keyIsDown[GLFW_KEY_D] == false && keyIsDown[GLFW_KEY_A] == false)
				{
					objArr[1].rBody.force.x = 0;
					objArr[1].rBody.velocity.x = objArr[1].rBody.velocity.x * .01;
					//object force is veloctiy * some negative constant (drag coefficient)
					// add that to net force every frame (force += new amount)
				}

				
				//gravity FIX THIS do jump with velocity instead of force but keep this
				objArr[1].rBody.force.y -= 4;


			}
			
			//enemies
			if (i > 1 && i <= 4)
			{
				if (objArr[i].moveUp == true)
				{
					objArr[i].rBody.force.y = 1;
				}

				if (objArr[i].moveUp == false)
				{
					objArr[i].rBody.force.y = -1;
				}
				
				
				
				
			}

			objArr[i].rBody.velocity += (objArr[i].rBody.force / objArr[i].rBody.mass) * deltaTime;

			

			//cap velocity
			if (objArr[i].rBody.velocity.x >= 50)
			{
				objArr[i].rBody.velocity.x = 50;
			}
			if (objArr[i].rBody.velocity.x <= -50)
			{
				objArr[i].rBody.velocity.x = -50;
			}

			if (objArr[i].rBody.velocity.y >= 500)
			{
				objArr[i].rBody.velocity.y = 500;
			}

			if (objArr[i].rBody.velocity.y <= -500)
			{
				objArr[i].rBody.velocity.y = -500;
			}

			//cap enemies
			if (i > 1)
			{
				if (objArr[i].rBody.velocity.y >= 50)
				{
					objArr[i].rBody.velocity.y = 50;
				}

				if (objArr[i].rBody.velocity.y <= -50)
				{
					objArr[i].rBody.velocity.y = -50;
				}
			}

			
			//add velocity to location
			objArr[i].tran.loc += objArr[i].rBody.velocity * deltaTime;
			

			if (i > 0 && i <= 4)
			{
				//if touching the ground, stops moving down
				if (collides(objArr[i], objArr[0]))
				{
					objArr[i].tran.loc.y = -.15;
					objArr[i].rBody.velocity.y = 0;
					objArr[i].rBody.force.y = 0;
					if (i == 1)
					{
						jumping = false;
					}
					else {
						objArr[i].moveUp = true;
						objArr[i].rBody.velocity.y = 0;
					}

					
					
				}

				if (i > 1 && objArr[i].tran.loc.y > 1)
				{
					objArr[i].moveUp = false;
					objArr[i].rBody.velocity.y = 0;
				}
			}
			
			glm::mat4 scaleMat = glm::scale(objArr[i].tran.size);
			glm::mat4 rotMat = glm::yawPitchRoll(0, 0, 0);
			glm::mat4 tranMat = glm::translate(objArr[i].tran.loc);
			glm::mat4 mat = tranMat * rotMat * scaleMat;
			objArr[i].tran.owMatrix = mat;
			

			

			

		}

		
		//update camera
		UpdateCam(cam);

		//glUniformMatrix4fv(3, 1, GL_FALSE, &objArr[i].tran.owMatrix[0][0]);
		glUniform3f(5, lightL.x, lightL.y, lightL.z);
		// Render game objects.
		for (int i = 0; i < 5; i++)
		{
			glUniformMatrix4fv(3, 1, GL_FALSE, &objArr[i].tran.owMatrix[0][0]);
			glBindTexture(GL_TEXTURE_2D, textures[i]);
			objArr[i].mod.render();
			
		}

		
		

		glBindVertexArray(0);
		// Unbind an object after drawing it.

		// Swap the front (what the screen displays) and
		//back (what OpenGL draws to) buffers.
		glfwSwapBuffers(GLFWwindowPtr);

		
		
		// Process input/window events
		// Process queued window, mouse & keyboard callback events.
		glfwPollEvents();
	}

	delete[] objArr;
	glfwTerminate();
	return true;
}

bool Engine::useShaders()
{
	bool value;
	value = shaderMan.loadShaders("shaders/vShader.glsl", "shaders/fShader.glsl");
	if (value == true)
	{
		glUseProgram(shaderMan.getProgram());
		return true;
	}
	return false;
}


