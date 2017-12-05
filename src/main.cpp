/*
Base code
Currently will make 2 FBOs and textures (only uses one in base code)
and writes out frame as a .png (Texture_output.png)

Winter 2017 - ZJW (Piddington texture write)
2017 integration with pitch and yaw camera lab (set up for texture mapping lab)
*/

#include <iostream>
#include <glad/glad.h>

#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"
#include "Shape.h"
#include "WindowManager.h"
#include "GLTextureWriter.h"

#include "base/GameLogic.h"

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GAMEBOARD_MID_X -2
#define GAMEBOARD_MID_Z 3
#define GAMEBOARD_Y 1
#define SETTLEMENT_SIZE .2
#define ROAD_SIZE .5
#define CITY_SIZE .2
#define THIEF_SIZE .2

using namespace std;
using namespace glm;

class Application : public EventCallbacks
{

public:

	WindowManager * windowManager = nullptr;

	// Our shader program
	std::shared_ptr<Program> prog;
	std::shared_ptr<Program> texProg;
	std::shared_ptr<Program> floorProg;
	std::shared_ptr<Program> cubeProg;
	std::shared_ptr<Program> hudProg0;
	std::shared_ptr<Program> clickProg;
	std::shared_ptr<Program> hudProg1;

	std::shared_ptr<Texture> texture0;
	std::shared_ptr<Texture> texture1;
	std::shared_ptr<Texture> texture2;
	std::shared_ptr<Texture> texture3;

	// Shape to be used (from obj file)
	shared_ptr<Shape> shape;
	shared_ptr<Shape> shape2;
	shared_ptr<Shape> cube;
	shared_ptr<Shape> bunny;
	shared_ptr<Shape> dog;
	shared_ptr<Shape> sphere;
	shared_ptr<Shape> flower;
	shared_ptr<Shape> hex;
	shared_ptr<Shape> pineForest;
	shared_ptr<Shape> quarry;
	shared_ptr<Shape> hills;
	shared_ptr<Shape> mountain;
	shared_ptr<Shape> farm;
	shared_ptr<Shape> pasture;
	shared_ptr<Shape> pyramid;
	shared_ptr<Shape> water;
	shared_ptr<Shape> dock;
	shared_ptr<Shape> ship;
	shared_ptr<Shape> barn;
	shared_ptr<Shape> road1;
	shared_ptr<Shape> square;
	shared_ptr<Shape> city;
	shared_ptr<Shape> cap;
	shared_ptr<Shape> shield;
	shared_ptr<Shape> wood;
	shared_ptr<Shape> brick;
	shared_ptr<Shape> wheat;
	shared_ptr<Shape> wool;
	shared_ptr<Shape> ore;
	shared_ptr<Shape> knight;
	shared_ptr<Shape> roadBuilding;
	shared_ptr<Shape> yearPlenty;
	shared_ptr<Shape> monopoly;
	shared_ptr<Shape> victoryPoint;

	//Game variables
	//********************
	int numPlayers = 0;
	int curIndex = 0;
	Player *curPlayer;
	Player *tempPlayer;
	Player *largestArmy = 0;
	Player *longestRoad = 0;
	Player playerList[4];
	std::vector<MyEnum::DevCard> developmentCards = std::vector<MyEnum::DevCard>(25);
	//bool exit = false;
	char action;
	int longestRoadCount = 0;
	int temp = 0;
	GameBoard *board;

	MyEnum::STATE gameState;
	//****************

	std::vector<vec3> hexCoordinates = std::vector<vec3>(37);

	// Contains vertex information for OpenGL
	GLuint VertexArrayID;

	//square info
	GLuint squareBuffObj, squareNorBuffObj, squareTexBuffObj, SIndxBuffObj;
	int gGiboLen;

	// Data necessary to give our triangle to OpenGL
	GLuint VertexBufferID;

	//geometry for texture render
	GLuint quad_VertexArrayID;
	GLuint quad_vertexbuffer;

	GLuint floor_VertexArrayID;
	GLuint floor_VertexBufferID;

	GLuint texBufID;
	std::vector<float> testTexBuf = std::vector<float>(600);

	//reference to texture FBO
	GLuint frameBuf[2];
	GLuint texBuf[2];
	GLuint depthBuf;

	GLuint frameBufTV;
	GLuint texBufTV;
	GLuint depthBufTV;

	bool freecamera = false;
	bool testClick = false;

	double testClickX, testClickY;

	std::vector<float> lightPos = {0, 5, -3};

	float PI = 3.14159265359;

	bool FirstTime = true;
	int count = 0;
	bool anotherFirst = true;
	bool Moving = false;
	int gMat = 0;

	float cTheta = -1 * PI / 2;
	bool mouseDown = false;

	vec3 eye = vec3(0, 5, 0);
	vec3 lookAtPoint = vec3(0, 3, -3);
	//vec3 eye = vec3(0, 0, 0);
	//vec3 lookAtPoint = vec3(0, 0, -1);

	float alpha = PI, beta = 0;
	

	double mouseX_1, mouseY_1, mouseX_2, mouseY_2;

	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		else if (key == GLFW_KEY_P && action == GLFW_PRESS)
		{
			gameState = MyEnum::GAME;
		}
		else if (key == GLFW_KEY_M && action == GLFW_PRESS)
		{
			gMat = (gMat + 1) % 4;
		}
		else if (key == GLFW_KEY_A && action != GLFW_RELEASE)
		{
			cTheta = -.5;
			vec3 view = lookAtPoint - eye;
			vec3 strafe = cross(view, vec3(0, 1, 0));
			lookAtPoint += cTheta * strafe;
			eye += cTheta * strafe;

		}
		else if (key == GLFW_KEY_D && action != GLFW_RELEASE)
		{
			cTheta = .5;
			vec3 view = lookAtPoint - eye;
			vec3 strafe = cross(view, vec3(0, 1, 0));
			lookAtPoint += cTheta * strafe;
			eye += cTheta * strafe;
		}
		else if (key == GLFW_KEY_W && action != GLFW_RELEASE)
		{
			cTheta = .5;
			vec3 view = lookAtPoint - eye;
			if (freecamera)
			{
				eye += view * cTheta;
				lookAtPoint += view * cTheta;
			}
			else
			{
				vec3 strafe = cross(view, vec3(0, 1, 0));
				vec3 strafe2 = cross(strafe, vec3(0, 1, 0));
				lookAtPoint += -cTheta * strafe2;
				eye += -cTheta * strafe2;
			}
		}
		else if (key == GLFW_KEY_S && action != GLFW_RELEASE)
		{
			cTheta = -.5;
			vec3 view = lookAtPoint - eye;
			if (freecamera)
			{
				eye += view * cTheta;
				lookAtPoint += view * cTheta;
			}
			else
			{
				vec3 strafe = cross(view, vec3(0, 1, 0));
				vec3 strafe2 = cross(strafe, vec3(0, 1, 0));
				lookAtPoint += -cTheta * strafe2;
				eye += -cTheta * strafe2;
			}
		}
		else if (key == GLFW_KEY_Q && action != GLFW_RELEASE) {
            lightPos[0] -= 1;
        }
        else if (key == GLFW_KEY_E && action != GLFW_RELEASE) {
         	lightPos[0] += 1;
        }
        else if (key == GLFW_KEY_F && action == GLFW_PRESS)
        {
        	freecamera = !freecamera;
        }
	}

	void scrollCallback(GLFWwindow* window, double deltaX, double deltaY)
	{
		cTheta += (float) deltaX;
	}

	void mouseCallback(GLFWwindow *window, int button, int action, int mods)
	{
		if (action == GLFW_PRESS)
		{
			mouseDown = true;
			glfwGetCursorPos(window, &mouseX_1, &mouseY_1);
			testClickX = mouseX_1;
			testClickY = mouseY_1;
			Moving = true;
			testClick = true;
		}

		if (action == GLFW_RELEASE)
		{
			Moving = false;
			mouseDown = false;
		}
	}

	void resizeCallback(GLFWwindow *window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void initTexture(const std::string &resourceDirectory)
	{
		texture0 = make_shared<Texture>();
		texture0->setFilename(resourceDirectory + "/jpg/test.jpg");
		texture0->init();
		texture0->setUnit(1);
		texture0->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

		texture1 = make_shared<Texture>();
		texture1->setFilename(resourceDirectory + "/jpg/progress_greenBorderSmall.jpg");
		texture1->init();
		texture1->setUnit(1);
		texture1->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

		texture2 = make_shared<Texture>();
		texture2->setFilename(resourceDirectory + "/jpg/progress_redBorderSmall.jpg");
		texture2->init();
		texture2->setUnit(1);
		texture2->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

		texture3 = make_shared<Texture>();
		texture3->setFilename(resourceDirectory + "/jpg/round_woodWear.jpg");
		texture3->init();
		texture3->setUnit(1);
		texture3->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		
	}

	void init(const std::string &resourceDirectory)
	{
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		GLSL::checkVersion();

		cTheta = 0;
		// Set background color.
		glClearColor(80.0/255.0, 195.0/255.0, 1.0f, 1.0f);
		// Enable z-buffer test.
		glEnable(GL_DEPTH_TEST);

		// Initialize the GLSL program.
		prog = make_shared<Program>();
		prog->setVerbose(true);
		prog->setShaderNames(
			resourceDirectory + "/simple_vert.glsl",
			resourceDirectory + "/simple_frag.glsl");
		if (! prog->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
		prog->addUniform("P");
		prog->addUniform("M");
		prog->addUniform("V");
		prog->addUniform("MatAmb");
		prog->addUniform("MatDif");
		prog->addUniform("MatSpec");
		prog->addUniform("LightColor");
		prog->addUniform("Lpos");
		prog->addUniform("S");
		prog->addAttribute("vertPos");
		prog->addAttribute("vertNor");
		prog->addAttribute("vertTex");

		//Init shader for floor
		floorProg = make_shared<Program>();
		floorProg->setVerbose(true);
		floorProg->setShaderNames(
			resourceDirectory + "/floor_vert.glsl",
			resourceDirectory + "/floor_frag.glsl");
		if (!floorProg->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}

		floorProg->addUniform("P");
		floorProg->addUniform("M");
		floorProg->addUniform("V");
		floorProg->addAttribute("vertPos");


		//create two frame buffer objects to toggle between
		glGenFramebuffers(2, frameBuf);
		glGenTextures(2, texBuf);
		glGenRenderbuffers(1, &depthBuf);
		createFBO(frameBuf[0], texBuf[0]);

		//set up depth necessary as rendering a mesh that needs depth test
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);

		//more FBO set up
		GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
		glDrawBuffers(1, DrawBuffers);

		//create another FBO so we can swap back and forth
		createFBO(frameBuf[1], texBuf[1]);
		//this one doesn't need depth

		//TV FBO
		glGenFramebuffers(1, &frameBufTV);
		glGenTextures(1, &texBufTV);
		glGenRenderbuffers(1, &depthBufTV);
		createFBO(frameBufTV, texBufTV);

		glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);



		//set up the shaders to blur the FBO just a placeholder pass thru now
		//next lab modify and possibly add other shaders to complete blur
		texProg = make_shared<Program>();
		texProg->setVerbose(true);
		texProg->setShaderNames(
			resourceDirectory + "/pass_vert.glsl",
			resourceDirectory + "/tex_fragH.glsl");
		if (! texProg->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
		texProg->addUniform("texBuf");
		texProg->addAttribute("vertPos");
		texProg->addUniform("dir");


		cubeProg = make_shared<Program>();
		cubeProg->setVerbose(true);
		cubeProg->setShaderNames(
			resourceDirectory + "/mySimpleVert.glsl",
			resourceDirectory + "/tex_fragSobel.glsl");
		if (! cubeProg->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
		cubeProg->addUniform("texBuf");
		cubeProg->addUniform("P");
		cubeProg->addUniform("M");
		cubeProg->addUniform("V");
		cubeProg->addAttribute("vertPos");

		floorProg->setVerbose(true);
		floorProg->setShaderNames(
			resourceDirectory + "/floor_vert.glsl",
			resourceDirectory + "/floor_frag.glsl");
		if (!floorProg->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}

		initTexture(resourceDirectory);

		hudProg0 = make_shared<Program>();
		hudProg0->setVerbose(true);
		hudProg0->setShaderNames(
			resourceDirectory + "/texVertTest.glsl",
			resourceDirectory + "/texFragTest.glsl");
		if (! hudProg0->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
 		hudProg0->addUniform("P");
		hudProg0->addUniform("MV");
		hudProg0->addAttribute("vertPos");
		hudProg0->addAttribute("vertNor");
		hudProg0->addAttribute("vertTex");
		hudProg0->addUniform("Texture0");

		hudProg1 = make_shared<Program>();
		hudProg1->setVerbose(true);
		hudProg1->setShaderNames(
			resourceDirectory + "/texVertTestTemp.glsl",
			resourceDirectory + "/texFragTest.glsl");
		if (! hudProg1->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
 		hudProg1->addUniform("P");
		hudProg1->addUniform("M");
		hudProg1->addUniform("V");
		hudProg1->addAttribute("vertPos");
		hudProg1->addAttribute("vertNor");
		hudProg1->addAttribute("vertTex");
		hudProg1->addUniform("Texture0");

		clickProg = make_shared<Program>();
		clickProg->setVerbose(true);
		clickProg->setShaderNames(
			resourceDirectory + "/pickingVert.glsl",
			resourceDirectory + "/pickingFrag.glsl");
		if (! clickProg->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
 		clickProg->addUniform("P");
		clickProg->addUniform("MV");
		clickProg->addUniform("pickingColor");
	}

	void initGame()
	{
		srand (time(NULL));
		GameLogic::initDevCards(developmentCards);
		initHexCoordinates(hexCoordinates);
		board = new GameBoard();

		gameState = MyEnum::PICK_NUM_PLAYERS;
	}

	void initGeom(const std::string& resourceDirectory)
	{
		//Initialize the geometry to render a quad to the screen
		initQuad();
		initFloor();

		sphere = make_shared<Shape>();
		sphere->loadMesh(resourceDirectory + "/obj/sphere.obj");
		sphere->resize();
		sphere->init();

		hex = make_shared<Shape>();
		hex->loadMesh(resourceDirectory + "/obj/tile_grass.obj");
		hex->resize();
		hex->init();

		pineForest = make_shared<Shape>();
		pineForest->loadMesh(resourceDirectory + "/obj/decorator_natural_pineForest.obj");
		pineForest->resize();
		pineForest->init();

		quarry = make_shared<Shape>();
		quarry->loadMesh(resourceDirectory + "/obj/tile_sand_OpenPitMine.obj");
		quarry->resize();
		quarry->init();

		hills = make_shared<Shape>();
		hills->loadMesh(resourceDirectory + "/obj/decorator_natural_hills.obj");
		hills->resize();
		hills->init();

		mountain = make_shared<Shape>();
		mountain->loadMesh(resourceDirectory + "/obj/tile_mountain.obj");
		mountain->resize();
		mountain->init();

		farm = make_shared<Shape>();
		farm->loadMesh(resourceDirectory + "/obj/decorator_natural_farm.obj");
		farm->resize();
		farm->init();

		pasture = make_shared<Shape>();
		pasture->loadMesh(resourceDirectory + "/obj/decorator_building_pasture.obj");
		pasture->resize();
		pasture->init();

		pyramid = make_shared<Shape>();
		pyramid->loadMesh(resourceDirectory + "/obj/decorator_building_pyramid.obj");
		pyramid->resize();
		pyramid->init();

		water = make_shared<Shape>();
		water->loadMesh(resourceDirectory + "/obj/tile_ocean1.obj");
		water->resize();
		water->init();

		dock = make_shared<Shape>();
		dock->loadMesh(resourceDirectory + "/obj/decorator_object_dock.obj");
		dock->resize();
		dock->init();

		ship = make_shared<Shape>();
		ship->loadMesh(resourceDirectory + "/obj/decorator_object_ship_sail.obj");
		ship->resize();
		ship->init();

		barn = make_shared<Shape>();
		barn->loadMesh(resourceDirectory + "/obj/decorator_building_barn.obj");
		barn->resize();
		barn->init();

		road1 = make_shared<Shape>();
		road1->loadMesh(resourceDirectory + "/obj/decorator_road1.obj");
		road1->resize();
		road1->init();

		city = make_shared<Shape>();
		city->loadMesh(resourceDirectory + "/obj/decorator_building_city1.obj");
		city->resize();
		city->init();

		square = make_shared<Shape>();
		square->loadMesh(resourceDirectory + "/obj/square.obj");
		square->resize();
		square->init();

		cap = make_shared<Shape>();
		cap->loadMesh(resourceDirectory + "/obj/decorator_object_catapult.obj");
		cap->resize();
		cap->init();

		shield = make_shared<Shape>();
		shield->loadMesh(resourceDirectory + "/obj/shieldBlue.obj");
		shield->resize();
		shield->init();

		wood = make_shared<Shape>();
		wood->loadMesh(resourceDirectory + "/obj/Wood.obj");
		wood->resize();
		wood->init();

		brick = make_shared<Shape>();
		brick->loadMesh(resourceDirectory + "/obj/cube.obj");
		brick->resize();
		brick->init();

		wheat = make_shared<Shape>();
		wheat->loadMesh(resourceDirectory + "/obj/shieldBlue.obj");
		wheat->resize();
		wheat->init();

		wool = make_shared<Shape>();
		wool->loadMesh(resourceDirectory + "/obj/SHEEP.OBJ");
		wool->resize();
		wool->init();

		ore = make_shared<Shape>();
		ore->loadMesh(resourceDirectory + "/obj/naturePack_059.obj");
		ore->resize();
		ore->init();

		knight = make_shared<Shape>();
		knight->loadMesh(resourceDirectory + "/obj/sword.obj");
		knight->resize();
		knight->init();

		roadBuilding = make_shared<Shape>();
		roadBuilding->loadMesh(resourceDirectory + "/obj/decorator_building_temple.obj");
		roadBuilding->resize();
		roadBuilding->init();

		yearPlenty = make_shared<Shape>();
		yearPlenty->loadMesh(resourceDirectory + "/obj/flagWhiteWide.obj");
		yearPlenty->resize();
		yearPlenty->init();

		monopoly = make_shared<Shape>();
		monopoly->loadMesh(resourceDirectory + "/obj/cross.obj");
		monopoly->resize();
		monopoly->init();

		victoryPoint = make_shared<Shape>();
		victoryPoint->loadMesh(resourceDirectory + "/obj/decorator_building_ruins.obj");
		victoryPoint->resize();
		victoryPoint->init();
	}

	void initFloor()
	{
		glGenVertexArrays(1, &floor_VertexArrayID);
		glBindVertexArray(floor_VertexArrayID);

		float size = 50;
		float depth = -4;

		static const GLfloat floor[] = {
			-size, depth, -size,
			-size, depth, size,
			size, depth, size,
			-size, depth, -size,
			size, depth, -size,
			size, depth, size
		};

		glGenBuffers(1, &floor_VertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, floor_VertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);
	}

	void updateLookAt()
	{
		if (mouseDown && freecamera)
		{
			int width, height;
			glfwGetCursorPos(windowManager->getHandle(), &mouseX_2, &mouseY_2);
			glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
			float scale = 4;

			alpha += (mouseX_2 / width - mouseX_1 / width) * scale;
			beta += (mouseY_2 / height - mouseY_1 / height) * scale;

			//cap for looking up and down
			float radianMax = 1.4;
			if (beta > radianMax)
				beta = radianMax;
			else if (beta < -radianMax)
				beta = -radianMax;

			float x = eye[0] + 1 * cos(beta) * sin(alpha);
			float y = eye[1] + 1 * sin(1 * beta);
			float z = eye[2] + 1 * cos(beta) * cos(alpha);

			lookAtPoint = vec3(x, y, z);

			mouseX_1 = mouseX_2;
			mouseY_1 = mouseY_2;
		}
	}

	vec3 computeRay(shared_ptr<MatrixStack> P)
	{
		int width, height;
		auto V = make_shared<MatrixStack>();

		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);

		width = width / 2;
		height = height / 2;

		float x = (2.0f * testClickX) / width - 1.0f;
		float y = 1.0f - (2.0f * testClickY) / height;
		//float z = 1.0f;

		//vec3 ray_nds = vec3(x, y, z);

		vec4 ray_clip = vec4(x, y, -1.0, 1.0);

		vec4 ray_eye = inverse(P->topMatrix()) * ray_clip;

		ray_eye = vec4(ray_eye.x, ray_eye.y, ray_eye.z, 0.0);

		V->pushMatrix();
		V->loadIdentity();
		V->lookAt(eye, lookAtPoint, vec3(0, 1, 0));

		vec4 ray_tmp = (inverse(V->topMatrix()) * ray_eye);
		vec3 ray_wor = vec3(ray_tmp.x, ray_tmp.y, ray_tmp.z);
		ray_wor = normalize(ray_wor);

		return ray_wor;
	}

	/**** geometry set up for a quad *****/
	void initQuad()
	{
		//now set up a simple quad for rendering FBO
		glGenVertexArrays(1, &quad_VertexArrayID);
		glBindVertexArray(quad_VertexArrayID);

		static const GLfloat g_quad_vertex_buffer_data[] =
		{
			-1.0f, -1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,
		};

		static const GLfloat g_quad_text_buffer_data[] =
		{
			1.0, 0.0,
			0.0, 1.0,
			0.0, 0.0,
			0.0, 1.0,
			0.0, 1.0,
			1.0, 1.0
		};


		glGenBuffers(1, &quad_vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

		glGenBuffers(1, &texBufID);
		glBindBuffer(GL_ARRAY_BUFFER, texBufID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_text_buffer_data), g_quad_text_buffer_data, GL_STATIC_DRAW);

	}

	/* Helper function to create the framebuffer object and
		associated texture to write to */
	void createFBO(GLuint& fb, GLuint& tex)
	{
		//initialize FBO
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);

		//set up framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, fb);
		//set up texture
		glBindTexture(GL_TEXTURE_2D, tex);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			cout << "Error setting up frame buffer - exiting" << endl;
			exit(0);
		}
	}

	// To complete image processing on the specificed texture
	// Right now just draws large quad to the screen that is texture mapped
	// with the prior scene image - next lab we will process
	void ProcessImage(GLuint inTex)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, inTex);

		// example applying of 'drawing' the FBO texture - change shaders
		texProg->bind();
		glUniform1i(texProg->getUniform("texBuf"), 0);
		glUniform2f(texProg->getUniform("dir"), -1, 0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(0);
		texProg->unbind();
	}

	void drawFloor(shared_ptr<MatrixStack> P)
	{
		auto M = make_shared<MatrixStack>();
		auto V = make_shared<MatrixStack>();

		floorProg->bind();
		glUniformMatrix4fv(floorProg->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
		M->loadIdentity();
		glUniformMatrix4fv(floorProg->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));

		updateLookAt();

		V->pushMatrix();
		V->loadIdentity();
		V->lookAt(eye, lookAtPoint, vec3(0, 1, 0));
		glUniformMatrix4fv(floorProg->getUniform("V"), 1, GL_FALSE, value_ptr(V->topMatrix()));
		V->popMatrix();

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, floor_VertexBufferID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		floorProg->unbind();
	}

	void drawForest(shared_ptr<MatrixStack> M)
	{
		SetMaterial(0);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()) );
		hex->draw(prog);

		M->pushMatrix();
		M->scale(vec3(.5, 1.0, .5));
		SetMaterial(4);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()) );
		pineForest->draw(prog);
		M->popMatrix();
	}

	void drawHills(shared_ptr<MatrixStack> M)
	{
		SetMaterial(0);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		hex->draw(prog);

		M->pushMatrix();
		M->scale(vec3(.5, 1.0, .5));
		SetMaterial(2);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		hills->draw(prog);
		M->popMatrix();
	}

	void drawMountains(shared_ptr<MatrixStack> M)
	{
		SetMaterial(0);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		hex->draw(prog);

		M->pushMatrix();
		M->scale(vec3(.5, 1.0, .5));
		SetMaterial(0);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		mountain->draw(prog);
		M->popMatrix();
	}

	void drawField(shared_ptr<MatrixStack> M)
	{
		SetMaterial(0);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		hex->draw(prog);

		M->pushMatrix();
		M->scale(vec3(.5, 1.0, .5));
		SetMaterial(2);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		farm->draw(prog);
		M->popMatrix();
	}

	void drawPasture(shared_ptr<MatrixStack> M)
	{
		SetMaterial(0);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		hex->draw(prog);

		M->pushMatrix();
		M->scale(vec3(.5, 1.0, .5));
		SetMaterial(2);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		pasture->draw(prog);
		M->popMatrix();
	}

	void drawDesert(shared_ptr<MatrixStack> M)
	{
		SetMaterial(0);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		hex->draw(prog);

		M->pushMatrix();
		M->scale(vec3(.5, 1.0, .5));
		SetMaterial(2);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		pyramid->draw(prog);
		M->popMatrix();
	}

	void drawWater(shared_ptr<MatrixStack> M)
	{
		SetMaterial(1);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		water->draw(prog);
	}

	void drawPort(shared_ptr<MatrixStack> M)
	{
		SetMaterial(1);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		water->draw(prog);
		
		SetMaterial(1);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		ship->draw(prog);
	}

	void initPlayers()
	{
		for (int i = 0; i < numPlayers; i++)
		{
			playerList[i] = Player(static_cast<MyEnum::Color>(i));
		}

		curPlayer = &playerList[curIndex];
	}

	int doubleBackOrder(int index)
	{
		if (index < numPlayers)
		{
			return index;
		}
		else
		{
			return numPlayers - (index % numPlayers) - 1;
		}
	}

	void firstResources()
	{
		Player *tempPlayer;
		std::vector<MyEnum::Resource> firstRoundResources;

		//get resources from your first settlements
		for (int i = 0; i < numPlayers; i++)
		{
			tempPlayer = &playerList[i];
			firstRoundResources = board->distributeFirstRoundResources(tempPlayer->getColor());
			tempPlayer->gainResourceCards(firstRoundResources);
		}
	}

	int drawPossibleFirstSettlements(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog, int index)
	{
		vec3 ray;

		passViewMatrix(prog);

		prog->bind();
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));

		for (int i = 0; i < board->getVertices().size(); i++)
		{
			if (board->validFirstSettlement(i))
			{
				int hexIndex = board->vertToHex(i);
				if (hexIndex != -1)
				{
					vec3 coords = getBuildingCoordinates(i, hexCoordinates[hexIndex]);
				
					M->pushMatrix();
					M->translate(coords);
					M->scale(vec3(SETTLEMENT_SIZE, SETTLEMENT_SIZE, SETTLEMENT_SIZE));
					SetMaterial(7);
					glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
					barn->draw(prog);
					if (testClick)
					{
						ray = computeRay(P);
						if (barn->wasHit(ray, eye, M->topMatrix()))
						{
							board->buildFirstSettlement(i, playerList[index].getColor());
							testClick = false;
							prog->unbind();
							return i;
						}
					}
					M->popMatrix();
				}
			}
		}

		prog->unbind();

		return -1;
	}

	bool drawRoad(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog, vec4 coords)
	{
		M->pushMatrix();
			M->translate(vec3(coords));
			M->rotate(coords[3], vec3(0, 1, 0));
			M->scale(vec3(ROAD_SIZE, ROAD_SIZE, ROAD_SIZE));
			SetMaterial(7);
			glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
			road1->draw(prog);
			if (testClick)
			{
				vec3 ray = computeRay(P);
				if (road1->wasHit(ray, eye, M->topMatrix()))
				{
					M->popMatrix();
					testClick = false;
					return true;
				}
			}
		M->popMatrix();
		return false;
	}

	bool drawRoadsOffASettlement(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog, int index, int settlementVert)
	{
		std::vector<int> branchRoads = board->roadsOffABuilding(settlementVert, playerList[index].getColor());

		passViewMatrix(prog);

		prog->bind();
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));

		for (int i = 0; i < branchRoads.size(); i++)
		{
			int hexIndex = board->edgeToHex(branchRoads[i]);
			if (hexIndex != -1)
			{
				vec4 coords = getRoadCoordinates(branchRoads[i], hexCoordinates[hexIndex]);

				if (drawRoad(P, M, prog, coords))
				{

					board->buildARoad(branchRoads[i], playerList[index].getColor());
					prog->unbind();
					return true;
				}
			}
		}

		prog->unbind();

		return false;
	}

	void drawFirstSettlementsAndRoad(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog)
	{
		static int index = 0;
		static bool drawSettlements = true;
		static int settlementJustBuilt = 0;
		int i = 0;
		

		if (index >= numPlayers * 2)
		{
			firstResources();
			gameState = MyEnum::GAME;
		}
		else
		{
			i = doubleBackOrder(index);
		}

		if (drawSettlements && (settlementJustBuilt = drawPossibleFirstSettlements(P, M, prog, i)) != -1)
		{
			drawSettlements = !drawSettlements;
		}

		if (!drawSettlements && drawRoadsOffASettlement(P, M, prog, i, settlementJustBuilt))
		{
			index++;
			drawSettlements = !drawSettlements;
		}
	}

	void drawPossibleBuildings(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog, MyEnum::BuildingType type)
	{
		auto V = make_shared<MatrixStack>();
		std::vector<int> possibleVerts;
		vec3 ray;

		if (type == MyEnum::settlement)
		{
			possibleVerts = board->possibleSettlementPositions(curPlayer->getColor());

			if (possibleVerts.size() == 0)
			{
				MyEnum::print("No valid posistions for a settlement");
				gameState = MyEnum::GAME;
			}
			else
			{
				passViewMatrix(prog);

				prog->bind();
				glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));

				for (int i = 0; i < possibleVerts.size(); i++)
				{
					//std::cout << "Possible spot at: " + std::to_string(possibleVerts[i]) << std::endl;
					//if () should check that the position is empty
					int hexIndex = board->vertToHex(possibleVerts[i]);
					if (hexIndex != -1)
					{
						vec3 coords = getBuildingCoordinates(possibleVerts[i], hexCoordinates[hexIndex]);
						M->pushMatrix();
						M->translate(coords);
						M->scale(vec3(SETTLEMENT_SIZE, SETTLEMENT_SIZE, SETTLEMENT_SIZE));
						SetMaterial(7);
						glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
						barn->draw(prog);
						if (testClick)
						{
							ray = computeRay(P);
							if (barn->wasHit(ray, eye, M->topMatrix()))
							{
								std::cout << "Build Settlement at " + std::to_string(possibleVerts[i]) << std::endl;
								if (!board->buildASettlement(possibleVerts[i], curPlayer->getColor()))
								{
									MyEnum::print("Something wrong with buildASettlement(GameBoard) at vert: " + std::to_string(possibleVerts[i]) + " and color: " + MyEnum::colorToString(curPlayer->getColor()));
								}

								if (!curPlayer->buildSettlement())
								{
									MyEnum::print("Something wrong with buildSettlement(Player) at vert: " + std::to_string(possibleVerts[i]) + " and color: " + MyEnum::colorToString(curPlayer->getColor()));
								}
								gameState = MyEnum::GAME;
								testClick = false;
							}
						}
						M->popMatrix();
					}
				}

				//cancel
				M->pushMatrix();
				M->translate(vec3(4, 0, -6));
				if (drawCancel(P, M, prog))
				{
					gameState = MyEnum::GAME;
				}
				M->popMatrix();

				prog->unbind();
			}
		}
	}

	void drawBuildings(shared_ptr<MatrixStack> M)
	{
		std::vector<Building> buildings = board->getVertices();

		for (int i = 0; i < buildings.size(); i++)
		{
			if (buildings[i].getColor() != MyEnum::empty)
			{
				int hexIndex = board->vertToHex(i);
				if (hexIndex != -1)
				{
					vec3 coords = getBuildingCoordinates(i, hexCoordinates[hexIndex]);
					M->pushMatrix();
					M->translate(coords);
					M->scale(vec3(SETTLEMENT_SIZE, SETTLEMENT_SIZE, SETTLEMENT_SIZE));
					setPlayerColor(buildings[i].getColor());
					glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
					barn->draw(prog);
					M->popMatrix();
				}
			}
		}
	}

	void drawPossibleRoads(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog, MyEnum::STATE nextState)
	{
		auto V = make_shared<MatrixStack>();
		std::vector<int> possibleEdges;
		
		possibleEdges = board->possibleRoadPositions(curPlayer->getColor());

		if (possibleEdges.size() == 0)
		{
			MyEnum::print("No valid posistions for a road");
			gameState = MyEnum::GAME;
		}
		else
		{
			passViewMatrix(prog);

			prog->bind();
			glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));

			for (int i = 0; i < possibleEdges.size(); i++)
			{
				//if () should check that the position is empty
				int hexIndex = board->edgeToHex(possibleEdges[i]);
				if (hexIndex != -1)
				{
					vec4 coords = getRoadCoordinates(possibleEdges[i], hexCoordinates[hexIndex]);

					if (drawRoad(P, M, prog, coords))
					{
						std::cout << "Build Road at " + std::to_string(possibleEdges[i]) << std::endl;
						if (!board->buildARoad(possibleEdges[i], curPlayer->getColor()))
						{
							MyEnum::print("Something wrong with buildARoad(GameBoard) at edge: " + std::to_string(possibleEdges[i]) + " and color: " + MyEnum::colorToString(curPlayer->getColor()));
						}

						if (gameState != MyEnum::ROAD_BUILDING_1 && gameState != MyEnum::ROAD_BUILDING_2)
						{
							if (!curPlayer->buildRoad())
							{
								MyEnum::print("Something wrong with buildRoad(Player) at edge: " + std::to_string(possibleEdges[i]) + " and color: " + MyEnum::colorToString(curPlayer->getColor()));
							}
						}
						gameState = nextState;

					}
				}
			}

			//cancel
			M->pushMatrix();
			M->translate(vec3(4, 0, -6));
			if (drawCancel(P, M, prog))
			{
				gameState = MyEnum::GAME;
			}
			M->popMatrix();

			prog->unbind();
		}
	}

	void drawRoads(shared_ptr<MatrixStack> M)
	{
		std::vector<Road> roads = board->getEdges();

		for (int i = 0; i < roads.size(); i++)
		{
			if (roads[i].getColor() != MyEnum::empty)
			{
				int hexIndex = board->edgeToHex(i);
				if (hexIndex != -1)
				{
					vec4 coords = getRoadCoordinates(i, hexCoordinates[hexIndex]);
					M->pushMatrix();
					M->translate(vec3(coords));
					M->rotate(coords[3], vec3(0, 1, 0));
					M->scale(vec3(ROAD_SIZE, ROAD_SIZE, ROAD_SIZE));
					setPlayerColor(roads[i].getColor());
					glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
					road1->draw(prog);
					M->popMatrix();

				}
			}
		}
	}

	vec3 getBuildingCoordinates(int index, vec3 hexCoordinates)
	{
		//top left
		if (index == 0 || index == 7 || index == 16 || index == 28 || index == 39)
		{
			return vec3(hexCoordinates[0] + -.866, hexCoordinates[1], hexCoordinates[2] + -.5);
		}
		//top
		else if (index == 1 || index == 3 || index == 5 || index == 8 || index == 10
			|| index == 12 || index == 14 || index == 17 || index == 19 || index == 21
			|| index == 23 || index == 25 || index == 29 || index == 31 || index == 33
			|| index == 35 || index == 40 || index == 42 || index == 44)
		{
			return vec3(hexCoordinates[0] + 0, hexCoordinates[1], hexCoordinates[2] + -1);
		}
		//top right
		else if (index == 2 || index == 4 || index == 6 || index == 9 || index == 11
			|| index == 13 || index == 15 || index == 18 || index == 20 || index == 22
			|| index == 24 || index == 26 || index == 30 || index == 32 || index == 34
			|| index == 36 || index == 41 || index == 43 || index == 45)
		{
			return vec3(hexCoordinates[0] + .866, hexCoordinates[1], hexCoordinates[2] + -.5);
		}
		//bot left
		else if (index == 27 || index == 38 || index == 47)
		{
			return vec3(hexCoordinates[0] + -.866, hexCoordinates[1], hexCoordinates[2] + .5);
		}
		//bot
		else if (index == 48 || index == 50 || index == 52)
		{
			return vec3(hexCoordinates[0] + 0, hexCoordinates[1], hexCoordinates[2] + 1);
		}
		//bot right
		else if (index == 37 || index == 46 || index == 49 || index == 51 || index == 53)
		{
			return vec3(hexCoordinates[0] + .866, hexCoordinates[1], hexCoordinates[2] + .5);
		}

		MyEnum::print("Missed an index: " + std::to_string(index));
		return vec3 (0, 0, 0);
	}

	vec4 getRoadCoordinates(int index, vec3 hexCoordinates)
	{
		//top left
		if (index == 0 || index == 2 || index == 4 || index == 10 || index == 12 
			|| index == 14 || index == 16 || index == 23 || index == 25 || index == 27
			|| index == 29 || index == 31 || index == 40 || index == 42 || index == 44
			|| index == 46 || index == 55 || index == 57 || index == 59)
		{
			return vec4(hexCoordinates[0] + -.433, hexCoordinates[1], hexCoordinates[2] + -.75, 1.5708);
		}
		//top right
		else if (index == 1 || index == 3 || index == 5 || index == 11 || index == 13
			|| index == 15 || index == 17 || index == 24 || index == 26 || index == 28
			|| index == 30 || index == 32 || index == 41 || index == 43 || index == 45
			|| index == 47 || index == 56 || index == 58 || index == 60)
		{
			return vec4(hexCoordinates[0] + .433, hexCoordinates[1], hexCoordinates[2] + -.75, .523599);
		}
		//right
		else if (index == 7 || index == 8 || index == 9 || index == 19 || index == 20
			|| index == 21 || index == 22 || index == 34 || index == 35 || index == 36
			|| index == 37 || index == 38 || index == 50 || index == 51 || index == 52 
			|| index == 53 || index == 63 || index == 64 || index == 65)
		{
			return vec4(hexCoordinates[0] + .866, hexCoordinates[1], hexCoordinates[2] + 0, -.523599);
		}
		//bot right
		else if (index == 48 || index == 61 || index == 67 || index == 69 || index == 71)
		{
			return vec4(hexCoordinates[0] + .433, hexCoordinates[1], hexCoordinates[2] + .75, 1.5708);
		}
		//bot left 
		else if (index == 39 || index == 54 || index == 66 || index == 68 || index == 70)
		{
			return vec4(hexCoordinates[0] + -.433, hexCoordinates[1], hexCoordinates[2] + .75, .523599);
		}
		//left
		else if (index == 6 || index == 18 || index == 33 || index == 49 || index == 62)
		{
			return vec4(hexCoordinates[0] + -.866, hexCoordinates[1], hexCoordinates[2] + 0, -.523599);
		}

		MyEnum::print("Missed an index: " + std::to_string(index));
		return vec4 (0, 0, 0, 0);
	} 

	void drawScene(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog)
	{
		auto V = make_shared<MatrixStack>();

		//Draw our scene
		prog->bind();
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));

		M->pushMatrix();
		M->scale(vec3(1.0, 1.0, 1.0));

		//LAYER 1
		//WATER
			M->pushMatrix();
			M->translate(hexCoordinates[0]);
			drawPort(M);
			M->popMatrix();
		//WATER
			M->pushMatrix();
			M->translate(hexCoordinates[1]);
			drawWater(M);
			M->popMatrix();
		//WATER
			M->pushMatrix();
			M->translate(hexCoordinates[2]);
			drawPort(M);
			M->popMatrix();
		//WATER
			M->pushMatrix();
			M->translate(hexCoordinates[3]);
			drawWater(M);
			M->popMatrix();

		//LAYER 2
		//WATER
			M->pushMatrix();
			M->translate(hexCoordinates[4]);
			drawWater(M);
			M->popMatrix();
		//ORE
			M->pushMatrix();
			M->translate(hexCoordinates[5]);
			drawMountains(M);
			M->popMatrix();
		//WOOL
			M->pushMatrix();
			M->translate(hexCoordinates[6]);
			drawPasture(M);
			M->popMatrix();
		//WOOD
			M->pushMatrix();
			M->translate(hexCoordinates[7]);
			drawForest(M);
			M->popMatrix();
		//WATER
			M->pushMatrix();
			M->translate(hexCoordinates[8]);
			drawPort(M);
			M->popMatrix();

		//LAYER 3
		//WATER
			M->pushMatrix();
			M->translate(hexCoordinates[9]);
			drawPort(M);
			M->popMatrix();
		//WHEAT
			M->pushMatrix();
			M->translate(hexCoordinates[10]);
			drawField(M);
			M->popMatrix();
		//BRICK
			M->pushMatrix();
			M->translate(hexCoordinates[11]);
			drawHills(M);
			M->popMatrix();
		//WOOL
			M->pushMatrix();
			M->translate(hexCoordinates[12]);
			drawPasture(M);
			M->popMatrix();
		//BRICK
			M->pushMatrix();
			M->translate(hexCoordinates[13]);
			drawHills(M);
			M->popMatrix();
		//WATER
			M->pushMatrix();
			M->translate(hexCoordinates[14]);
			drawWater(M);
			M->popMatrix();

		//LAYER 4
		//WATER
			M->pushMatrix();
			M->translate(hexCoordinates[15]);
			drawWater(M);
			M->popMatrix();
		//WHEAT
			M->pushMatrix();
			M->translate(hexCoordinates[16]);
			drawField(M);
			M->popMatrix();
		//WOOD
			M->pushMatrix();
			M->translate(hexCoordinates[17]);
			drawForest(M);
			M->popMatrix();
		//DESERT
			M->pushMatrix();
			M->translate(hexCoordinates[18]);
			drawDesert(M);
			M->popMatrix();
		//WOOD
			M->pushMatrix();
			M->translate(hexCoordinates[19]);
			drawForest(M);
			M->popMatrix();
		//ORE
			M->pushMatrix();
			M->translate(hexCoordinates[20]);
			drawMountains(M);
			M->popMatrix();
		//WATER
			M->pushMatrix();
			M->translate(hexCoordinates[21]);
			drawPort(M);
			M->popMatrix();

		//LAYER 5
		//WATER
			M->pushMatrix();
			M->translate(hexCoordinates[22]);
			drawPort(M);
			M->popMatrix();
		//WOOD
			M->pushMatrix();
			M->translate(hexCoordinates[23]);
			drawForest(M);
			M->popMatrix();
		//ORE
			M->pushMatrix();
			M->translate(hexCoordinates[24]);
			drawMountains(M);
			M->popMatrix();
		//WHEAT
			M->pushMatrix();
			M->translate(hexCoordinates[25]);
			drawField(M);
			M->popMatrix();
		//WOOL
			M->pushMatrix();
			M->translate(hexCoordinates[26]);
			drawPasture(M);
			M->popMatrix();
		//WATER
			M->pushMatrix();
			M->translate(hexCoordinates[27]);
			drawWater(M);
			M->popMatrix();

		//LAYER 6
		//WATER
			M->pushMatrix();
			M->translate(hexCoordinates[28]);
			drawWater(M);
			M->popMatrix();
		//BRICK
			M->pushMatrix();
			M->translate(hexCoordinates[29]);
			drawHills(M);
			M->popMatrix();
		//WHEAT
			M->pushMatrix();
			M->translate(hexCoordinates[30]);
			drawField(M);
			M->popMatrix();
		//WOOL
			M->pushMatrix();
			M->translate(hexCoordinates[31]);
			drawPasture(M);
			M->popMatrix();
		//WATER
			M->pushMatrix();
			M->translate(hexCoordinates[32]);
			drawPort(M);
			M->popMatrix();

		//LAYER 7
		//WATER
			M->pushMatrix();
			M->translate(hexCoordinates[33]);
			drawPort(M);
			M->popMatrix();
		//WATER
			M->pushMatrix();
			M->translate(hexCoordinates[34]);
			drawWater(M);
			M->popMatrix();
		//WATER
			M->pushMatrix();
			M->translate(hexCoordinates[35]);
			drawPort(M);
			M->popMatrix();
		//WATER
			M->pushMatrix();
			M->translate(hexCoordinates[36]);
			drawWater(M);
			M->popMatrix();


			drawBuildings(M);
			drawRoads(M);

		M->popMatrix();
		

		updateLookAt();

		V->pushMatrix();
		V->loadIdentity();
		V->lookAt(eye, lookAtPoint, vec3(0, 1, 0));
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(V->topMatrix()));
		V->popMatrix();

		glUniform3f(prog->getUniform("LightColor"), 1, 1, 1);
		glUniform3f(prog->getUniform("Lpos"), lightPos[0], lightPos[1], lightPos[2]);

		prog->unbind();

		drawFloor(P);
	}

	void drawHUD(shared_ptr<MatrixStack> P, shared_ptr<Program> prog)
	{
		auto MV = make_shared<MatrixStack>();
		auto M = make_shared<MatrixStack>();
		auto V = make_shared<MatrixStack>();
		auto P2 = make_shared<MatrixStack>();
		vec3 ray;
		//int width, height;
		//int width_2 = 0, height_2 = 0;

		//float aspect = width/(float)height;


		//glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);

		//width = width / 2;
		//height = height / 2;

		//width_2 = width / 2;
		//height = height / 2;

		//float x = (2.0f * testClickX) / width - 1.0f;
		//float y = 1.0f - (2.0f * testClickY) / height;

		//std::cout << "width: " + std::to_string(width) + " height: " + std::to_string(height) << std::endl;
		if (testClick)
		{
			std::cout << "X: " + std::to_string(testClickX) + " Y: " + std::to_string(testClickY) << std::endl;
			testClick = false;
		}

		prog->bind();
		P2->pushMatrix();
		P2->loadIdentity();
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P2->topMatrix()));

			MV->pushMatrix();
			MV->translate(vec3(0, 0, -.5));
			MV->rotate(PI / 2, vec3(1, 0, 0));
			MV->scale(vec3(1, 1, 1));
			glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, value_ptr(MV->topMatrix()));
			texture0->bind(prog->getUniform("Texture0"));
			square->draw(prog);
			if (testClick)
			{
				mat4 matrix = MV->topMatrix();

				ray = computeRay(P);
				if (square->wasHit(ray, eye, matrix))
				{
					std::cout << "Hit" << std::endl;
				}
				testClick = false;
			}
			MV->popMatrix();
			texture0->unbind();

		prog->unbind();
	}

	void drawPossibleThief(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog)
	{
		auto V = make_shared<MatrixStack>();
		vec2 hexCoords;
		vec3 ray;
		int x, y;

		prog->bind();
			glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));

			V->pushMatrix();
			V->loadIdentity();
			V->lookAt(eye, lookAtPoint, vec3(0, 1, 0));
			glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(V->topMatrix()));
			V->popMatrix();

			for (int i = 0; i < 37; i++)
			{
				hexCoords = board->intToHex(i);
				x = hexCoords.x;
				y = hexCoords.y;

				if (board->notWater(y + 3, x + 3) && board->getThiefPos() != board->getHex(x,y))
				{
					M->pushMatrix();
					M->translate(vec3(hexCoordinates[i].x, hexCoordinates[i].y + 1, hexCoordinates[i].z));
					M->scale(vec3(THIEF_SIZE, THIEF_SIZE, THIEF_SIZE));
					glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
					cap->draw(prog);
					if (testClick)
					{
						ray = computeRay(P);
						if (cap->wasHit(ray, eye, M->topMatrix()))
						{
							std::cout << "Thief placed at " + std::to_string(i) << std::endl;
							board->placeThief(x, y);
							gameState = MyEnum::STEAL_FROM_PLAYERS;
							testClick = false;
						}
					}
					M->popMatrix();
				}
			}
		prog->unbind();
	}



	void drawKnightCard(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog)
	{
		if (drawKnight(P, M, prog))
		{
			if (curPlayer->canPlayKnight())
			{
				MyEnum::print(MyEnum::colorToString(curPlayer->getColor()) + " has played a Knight Card");
				curPlayer->playedAKnight();
				gameState = MyEnum::PLACE_THIEF;
			}
			else
			{
				MyEnum::print("No Knight Card to play");
			}
		}
	}

	bool drawKnight(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog)
	{
		M->pushMatrix();
		M->scale(.5);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		MyEnum::print(knight->shapes.size());
		knight->draw(prog);
		if (testClick)
		{
			vec3 ray = computeRay(P);
			if (knight->wasHit(ray, eye, M->topMatrix()))
			{
				MyEnum::print("Knight");
				testClick = false;
				M->popMatrix();
				return true;
			}
		}
		M->popMatrix();

		return false;
	}

	void drawRoadBuilding(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog)
	{
		vec3 ray;

		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		roadBuilding->draw(prog);
		if (testClick)
		{
			ray = computeRay(P);
			if (roadBuilding->wasHit(ray, eye, M->topMatrix()))
			{
				std::cout << "Road Building" << std::endl;
				testClick = false;
				gameState = MyEnum::ROAD_BUILDING_1;
			}
		}
	}

	void drawYearPlenty(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog)
	{
		vec3 ray;

		M->pushMatrix();
		M->scale(.2);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		yearPlenty->draw(prog);
		if (testClick)
		{
			ray = computeRay(P);
			if (yearPlenty->wasHit(ray, eye, M->topMatrix()))
			{
				std::cout << "Year Plenty" << std::endl;
				testClick = false;
				gameState = MyEnum::YEAR_PLENTY;
			}
		}
		M->popMatrix();
	}

	void drawMonopoly(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog)
	{
		vec3 ray;

		M->pushMatrix();
		M->scale(.2);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		monopoly->draw(prog);
		if (testClick)
		{
			ray = computeRay(P);
			if (monopoly->wasHit(ray, eye, M->topMatrix()))
			{
				std::cout << "Monopoly" << std::endl;
				testClick = false;
				gameState = MyEnum::MONOPOLY;
			}
		}
		M->popMatrix();
	}

	void drawVictoryPoint(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog)
	{
		vec3 ray;

		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		victoryPoint->draw(prog);
		if (testClick)
		{
			ray = computeRay(P);
			if (victoryPoint->wasHit(ray, eye, M->topMatrix()))
			{
				std::cout << "Victory Point" << std::endl;
				testClick = false;
				//gameState = MyEnum::GAME;
			}
		}
	}

	void drawPossibleDevCards(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog)
	{
		std::vector<int> devCards;

		passViewMatrix(prog);

		prog->bind();
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));

		devCards = curPlayer->getHand().getDevCards();

		M->pushMatrix();
		M->translate(vec3(2, 0, -6));
			
			if (devCards[MyEnum::knight] > 0)
			{
				drawKnightCard(P, M, prog);
			}

			if (devCards[MyEnum::roadBuilding] > 0)
			{
				M->pushMatrix();
				M->translate(vec3(2, 0, 0));
				drawRoadBuilding(P, M, prog);
				M->popMatrix();
			}

			if (devCards[MyEnum::yearPlenty] > 0)
			{
				M->pushMatrix();
				M->translate(vec3(4, 0, 0));
				drawYearPlenty(P, M, prog);
				M->popMatrix();
			}

			if (devCards[MyEnum::monopoly] > 0)
			{
				M->pushMatrix();
				M->translate(vec3(6, 0, 0));
				drawMonopoly(P, M, prog);
				M->popMatrix();
			}

			for (int i = 0; i < devCards[MyEnum::victoryPoint]; i++)
			{
				M->pushMatrix();
				M->translate(vec3(8 + 2 * i, 0, 0));
				drawVictoryPoint(P, M, prog);
				M->popMatrix();
			}

			//cancel
			M->pushMatrix();
			M->translate(vec3(0, 0, -6));
			M->scale(.5);
			if (drawCancel(P, M, prog))
			{
				gameState = MyEnum::GAME;
			}
			M->popMatrix();

		M->popMatrix();
		prog->unbind();
	}



	void roll()
	{
		int roll = (rand() % 6) + 1 + (rand() % 6) + 1;

		MyEnum::print("A " + std::to_string(roll) + " was rolled");

		if (roll == 7)
		{
			//for (int i = 0; i < numPlayers; i++)
			//{
			//	playerList[i].rolledASeven();
			//}
			//GameLogic::placeThief(playerList, board, numPlayers, curIndex);
			gameState = MyEnum::PLACE_THIEF;
		}
		else
		{
			board->diceRoll(playerList, roll);
		}

		GameLogic::printPlayers(playerList, 2);
	}

	void drawTempHUD(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog)
	{
		auto V = make_shared<MatrixStack>();
		vec3 ray;

		passViewMatrix(prog);

		prog->bind();
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));

		//building sphere
			M->pushMatrix();
			M->translate(vec3(7, 0, 0));
			M->scale(vec3(1, 1, 1));
			glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
			texture0->bind(prog->getUniform("Texture0"));
			sphere->draw(prog);
			if (testClick)
			{
				ray = computeRay(P);
				if (sphere->wasHit(ray, eye, M->topMatrix()))
				{
					std::cout << "Build" << std::endl;
					gameState = MyEnum::BUILDING;
					testClick = false;
				}
			}
			M->popMatrix();
			texture0->unbind();

		//roll sphere
			M->pushMatrix();
			M->translate(vec3(-9, 0, 0));
			M->scale(vec3(1, 1, 1));
			glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
			texture1->bind(prog->getUniform("Texture0"));
			sphere->draw(prog);
			if (testClick)
			{
				ray = computeRay(P);
				if (sphere->wasHit(ray, eye, M->topMatrix()))
				{
					std::cout << "Roll" << std::endl;
					roll();
					testClick = false;
				}
			}
			M->popMatrix();
			texture1->unbind();
		//dev card sphere
			M->pushMatrix();
			M->translate(vec3(7, 0, 3));
			M->scale(vec3(1, 1, 1));
			glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
			texture3->bind(prog->getUniform("Texture0"));
			sphere->draw(prog);
			if (testClick)
			{
				ray = computeRay(P);
				if (sphere->wasHit(ray, eye, M->topMatrix()))
				{
					std::cout << "Dev Card" << std::endl;
					gameState = MyEnum::DEV_CARD;
					testClick = false;
				}
			}
			M->popMatrix();
			texture3->unbind();
		//end turn sphere
			M->pushMatrix();
			M->translate(vec3(-9, 0, 8));
			M->scale(vec3(1, 1, 1));
			glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
			texture2->bind(prog->getUniform("Texture0"));
			sphere->draw(prog);
			if (testClick)
			{
				ray = computeRay(P);
				if (sphere->wasHit(ray, eye, M->topMatrix()))
				{
					std::cout << "End Turn" << std::endl;
					curIndex = (curIndex + 1) % numPlayers;
					curPlayer = &playerList[curIndex];
					MyEnum::print(curPlayer->getColorString()+ "'s Turn\n");
					MyEnum::print(curPlayer->getHand().print());
					MyEnum::print(curPlayer->getHand().printDevelopmentCards());
					testClick = false;
				}
			}
			M->popMatrix();
			texture2->unbind();
		prog->unbind();
	}

	void drawCurrentPlayer(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog)
	{
		passViewMatrix(prog);

		prog->bind();
			M->pushMatrix();
			M->translate(vec3(-8, 0, -7));
			M->scale(2);
			setPlayerColor(curPlayer->getColor());
			glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
			sphere->draw(prog);
			M->popMatrix();
		prog->unbind();
	}

	bool drawWood(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog)
	{
		M->pushMatrix();
		M->scale(.1);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		wood->draw(prog);
		if (testClick)
		{
			vec3 ray = computeRay(P);
			if (wood->wasHit(ray, eye, M->topMatrix()))
			{
				MyEnum::print("Wood");
				testClick = false;
				M->popMatrix();
				return true;
			}
		}
		M->popMatrix();

		return false;
	}

	bool drawBrick(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog)
	{
		M->pushMatrix();
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		brick->draw(prog);
		if (testClick)
		{
			vec3 ray = computeRay(P);
			if (brick->wasHit(ray, eye, M->topMatrix()))
			{
				MyEnum::print("Brick");
				testClick = false;
				M->popMatrix();
				return true;
			}
		}
		M->popMatrix();

		return false;
	}

	bool drawWheat(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog)
	{
		M->pushMatrix();
		M->scale(.5);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		wheat->draw(prog);
		if (testClick)
		{
			vec3 ray = computeRay(P);
			if (wheat->wasHit(ray, eye, M->topMatrix()))
			{
				MyEnum::print("Wheat");
				testClick = false;
				M->popMatrix();
				return true;
			}
		}
		M->popMatrix();

		return false;
	}

	bool drawWool(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog)
	{
		M->pushMatrix();
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		wool->draw(prog);
		if (testClick)
		{
			vec3 ray = computeRay(P);
			if (wool->wasHit(ray, eye, M->topMatrix()))
			{
				MyEnum::print("Wool");
				testClick = false;
				M->popMatrix();
				return true;
			}
		}
		M->popMatrix();

		return false;
	}

	bool drawOre(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog)
	{
		M->pushMatrix();
		M->scale(.5);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		ore->draw(prog);
		if (testClick)
		{
			vec3 ray = computeRay(P);
			if (ore->wasHit(ray, eye, M->topMatrix()))
			{
				MyEnum::print("Ore");
				testClick = false;
				M->popMatrix();
				return true;
			}
		}
		M->popMatrix();

		return false;
	}

	void drawOneResource(MyEnum::Resource res, shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog)
	{
		switch(res)
		{
			case MyEnum::wood:
				drawWood(P, M, prog);
				break;
			case MyEnum::brick:
				drawBrick(P, M, prog);
				break;
			case MyEnum::wheat:
				drawWheat(P, M, prog);
				break;
			case MyEnum::wool:
				drawWool(P, M, prog);
				break;
			case MyEnum::ore:
				drawOre(P, M, prog);
				break;
			case MyEnum::water:
				break;
			case MyEnum::desert:
				break;
		}
	}

	void chooseResource_2(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog, MyEnum::Resource resource)
	{
		std::vector<MyEnum::Resource> resourceCards = std::vector<MyEnum::Resource>(2);

		if (resource == MyEnum::water)
		{
			MyEnum::print("Got water in chooseResource_2");
			gameState = MyEnum::GAME;
		}

		passViewMatrix(prog);

		prog->bind();
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));

		M->pushMatrix();
		M->translate(vec3(7, 0, -3));
		drawOneResource(resource, P, M, prog);
		M->popMatrix();

		M->pushMatrix();
		M->translate(vec3(7, 0, -1));

			if (drawWood(P, M, prog))
			{
				if (gameState == MyEnum::YEAR_PLENTY_2)
				{
					gameState = MyEnum::GAME;
					resourceCards[0] = resource;
					resourceCards[1] = MyEnum::wood;
					curPlayer->playedAYearPlenty(resourceCards);
				}
				else
				{
					MyEnum::print("Shouldn't be in chooseResource_2 wood");
					gameState = MyEnum::GAME;
				}
			}

			M->pushMatrix();
			M->translate(vec3(2, 0, 0));
			if (drawBrick(P, M, prog))
			{
				if (gameState == MyEnum::YEAR_PLENTY_2)
				{
					gameState = MyEnum::GAME;
					resourceCards[0] = resource;
					resourceCards[1] = MyEnum::brick;
					curPlayer->playedAYearPlenty(resourceCards);
				}
				else
				{
					MyEnum::print("Shouldn't be in chooseResource_2 brick");
					gameState = MyEnum::GAME;
				}
			}
			M->popMatrix();

			M->pushMatrix();
			M->translate(vec3(4, 0, 0));
			if (drawWheat(P, M, prog))
			{
				if (gameState == MyEnum::YEAR_PLENTY_2)
				{
					gameState = MyEnum::GAME;
					resourceCards[0] = resource;
					resourceCards[1] = MyEnum::wheat;
					curPlayer->playedAYearPlenty(resourceCards);
				}
				else
				{
					MyEnum::print("Shouldn't be in chooseResource_2 wheat");
					gameState = MyEnum::GAME;
				}
			}
			M->popMatrix();

			M->pushMatrix();
			M->translate(vec3(6, 0, 0));
			if (drawWool(P, M, prog))
			{
				if (gameState == MyEnum::YEAR_PLENTY_2)
				{
					gameState = MyEnum::GAME;
					resourceCards[0] = resource;
					resourceCards[1] = MyEnum::wool;
					curPlayer->playedAYearPlenty(resourceCards);
				}
				else
				{
					MyEnum::print("Shouldn't be in chooseResource_2 wool");
					gameState = MyEnum::GAME;
				}
			}
			M->popMatrix();

			M->pushMatrix();
			M->translate(vec3(8, 0, 0));
			if (drawOre(P, M, prog))
			{
				if (gameState == MyEnum::YEAR_PLENTY_2)
				{
					gameState = MyEnum::GAME;
					resourceCards[0] = resource;
					resourceCards[1] = MyEnum::ore;
					curPlayer->playedAYearPlenty(resourceCards);
				}
				else
				{
					MyEnum::print("Shouldn't be in chooseResource_2 ore");
					gameState = MyEnum::GAME;
				}
			}
			M->popMatrix();
	}

	MyEnum::Resource chooseResource(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog)
	{
		passViewMatrix(prog);

		prog->bind();
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));

		M->pushMatrix();
		M->translate(vec3(7, 0, -3));

			if (drawWood(P, M, prog))
			{
				M->popMatrix();
				if (gameState == MyEnum::YEAR_PLENTY)
				{
					gameState = MyEnum::YEAR_PLENTY_2;
					return MyEnum::wood;
				}
				else if (gameState == MyEnum::MONOPOLY)
				{
					GameLogic::playAMonopoly(playerList, numPlayers, curIndex, MyEnum::wood);
					gameState = MyEnum::GAME;
					return MyEnum::water;
				}
				else
				{
					MyEnum::print("Shouldn't be in chooseResource wood");
					return MyEnum::water;
				}
			}

			M->pushMatrix();
			M->translate(vec3(2, 0, 0));
			if (drawBrick(P, M, prog))
			{
				M->popMatrix();
				M->popMatrix();
				if (gameState == MyEnum::YEAR_PLENTY)
				{
					gameState = MyEnum::YEAR_PLENTY_2;
					return MyEnum::brick;
				}
				else if (gameState == MyEnum::MONOPOLY)
				{
					GameLogic::playAMonopoly(playerList, numPlayers, curIndex, MyEnum::brick);
					gameState = MyEnum::GAME;
					return MyEnum::water;
				}
				else
				{
					MyEnum::print("Shouldn't be in chooseResource brick");
					return MyEnum::water;
				}
			}
			M->popMatrix();

			M->pushMatrix();
			M->translate(vec3(4, 0, 0));
			if (drawWheat(P, M, prog))
			{
				M->popMatrix();
				M->popMatrix();
				if (gameState == MyEnum::YEAR_PLENTY)
				{
					gameState = MyEnum::YEAR_PLENTY_2;
					return MyEnum::wheat;
				}
				else if (gameState == MyEnum::MONOPOLY)
				{
					GameLogic::playAMonopoly(playerList, numPlayers, curIndex, MyEnum::wheat);
					gameState = MyEnum::GAME;
					return MyEnum::water;
				}
				else 
				{
					MyEnum::print("Shouldn't be in chooseResource wheat");
					return MyEnum::water;
				}
			}
			M->popMatrix();

			M->pushMatrix();
			M->translate(vec3(6, 0, 0));
			if (drawWool(P, M, prog))
			{
				M->popMatrix();
				M->popMatrix();
				if (gameState == MyEnum::YEAR_PLENTY)
				{
					gameState = MyEnum::YEAR_PLENTY_2;
					return MyEnum::wool;
				}
				else if (gameState == MyEnum::MONOPOLY)
				{
					GameLogic::playAMonopoly(playerList, numPlayers, curIndex, MyEnum::wool);
					gameState = MyEnum::GAME;
					return MyEnum::water;
				}
				else
				{
					MyEnum::print("Shouldn't be in chooseResource wool");
					return MyEnum::water;
				}
			}
			M->popMatrix();

			M->pushMatrix();
			M->translate(vec3(8, 0, 0));
			if (drawOre(P, M, prog))
			{
				M->popMatrix();
				M->popMatrix();
				if (gameState == MyEnum::YEAR_PLENTY)
				{
					gameState = MyEnum::YEAR_PLENTY_2;
					return MyEnum::ore;
				}
				else if (gameState == MyEnum::MONOPOLY)
				{
					GameLogic::playAMonopoly(playerList, numPlayers, curIndex, MyEnum::ore);
					gameState = MyEnum::GAME;
					return MyEnum::water;
				}
				else 
				{
					MyEnum::print("Shouldn't be in chooseResource ore");
					return MyEnum::water;
				}
			}
			M->popMatrix();

		M->popMatrix();
		prog->unbind();

		return MyEnum::water;
	}

	void drawResources(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog)
	{
		std::vector<int> resources = curPlayer->getHand().getResourceCards();
		passViewMatrix(prog);

		prog->bind();
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));

		M->pushMatrix();
		M->translate(vec3(6, 0, 7));
			//draw wood
			for (int i = 0; i < resources[MyEnum::wood]; i++)
			{
				M->pushMatrix();
				M->translate(vec3(2 * i, 0, 0));
				drawWood(P, M, prog);
				M->popMatrix();
			}
			//draw brick
			for (int i = 0; i < resources[MyEnum::brick]; i++)
			{
				M->pushMatrix();
				M->translate(vec3(2 * i, 0, 2));
				drawBrick(P, M, prog);
				M->popMatrix();
			}
			//draw wheat
			for (int i = 0; i < resources[MyEnum::wheat]; i++)
			{
				M->pushMatrix();
				M->translate(vec3(2 * i, 0, 4));
				drawWheat(P, M, prog);
				M->popMatrix();
			}
			//draw wool
			for (int i = 0; i < resources[MyEnum::wool]; i++)
			{
				M->pushMatrix();
				M->translate(vec3(2 * i, 0, 6));
				drawWool(P, M, prog);
				M->popMatrix();
			}
			//draw ore
			for (int i = 0; i < resources[MyEnum::ore]; i++)
			{
				M->pushMatrix();
				M->translate(vec3(2 * i, 0, 8));
				drawOre(P, M, prog);
				M->popMatrix();
			}
		M->popMatrix();

		prog->unbind();
	}

	void drawBuildMenu(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog)
	{
		auto V = make_shared<MatrixStack>();
		vec3 ray;

		passViewMatrix(prog);

		prog->bind();
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));

		//settlement
		if (curPlayer->canBuildSettlement())
		{
			M->pushMatrix();
			M->translate(vec3(-4, 0, -6));
			glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
			barn->draw(prog);
			if (testClick)
			{
				ray = computeRay(P);
				if (barn->wasHit(ray, eye, M->topMatrix()))
				{
					std::cout << "Settlement" << std::endl;
					gameState = MyEnum::BUILD_SETTLEMENT;
					testClick = false;
				}
			}
			M->popMatrix();
		}
		//road
		if (curPlayer->canBuildRoad())
		{
			M->pushMatrix();
			M->translate(vec3(-2, 0, -6));
			glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
			road1->draw(prog);
			if (testClick)
			{
				ray = computeRay(P);
				if (road1->wasHit(ray, eye, M->topMatrix()))
				{
					std::cout << "Road" << std::endl;
					gameState = MyEnum::BUILD_ROAD;
					testClick = false;
				}
			}
			M->popMatrix();
		}
		//city
		if (curPlayer->canBuildCity())
		{
			M->pushMatrix();
			M->translate(vec3(0, 0, -6));
			glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
			city->draw(prog);
			if (testClick)
			{
				ray = computeRay(P);
				if (city->wasHit(ray, eye, M->topMatrix()))
				{
					std::cout << "City" << std::endl;
					//build city
					testClick = false;
				}
			}
			M->popMatrix();
		}
		//dev card
		if (curPlayer->canBuildDevelopmentCard())
		{
			M->pushMatrix();
			M->translate(vec3(2, 0, -6));
			glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
			square->draw(prog);
			if (testClick)
			{
				ray = computeRay(P);
				if (square->wasHit(ray, eye, M->topMatrix()))
				{
					//if (curPlayer->buildDevelopmentCard())
					//{
						std::cout << "Dev Card" << std::endl;
						int random = rand() % developmentCards.size();
						MyEnum::print(MyEnum::colorToString(curPlayer->getColor()) + " got a " + MyEnum::devcardToString(developmentCards[random]));
						curPlayer->gainDevelopmentCard(developmentCards[random]);
						developmentCards.erase(developmentCards.begin() + random);

					//}
					//else
					//{
					//	MyEnum::print("Player can't build dev card");
					//}
					gameState = MyEnum::GAME;
					testClick = false;
				}
			}
			M->popMatrix();
		}
		//cancel
		M->pushMatrix();
		M->translate(vec3(4, 0, -6));
		if (drawCancel(P, M, prog))
		{
			gameState = MyEnum::GAME;
		}
		M->popMatrix();
	}

	bool drawCancel(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog)
	{
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		sphere->draw(prog);
		if (testClick)
		{
			vec3 ray = computeRay(P);
			if (sphere->wasHit(ray, eye, M->topMatrix()))
			{
				MyEnum::print("Cancel");
				testClickY = false;
				return true;
			}
		}

		return false;
	}

	bool wasHit(vec3 ray, vec3 eye, std::vector<mat4> matrices, std::vector<shared_ptr<Shape>> shapes)
	{
		if (matrices.size() != shapes.size())
		{
			MyEnum::print("incorrect call of wasHit in Application");
			gameState = MyEnum::GAME;
		}

		for (int i = 0; i < shapes.size(); i++)
		{
			if (shapes[i]->wasHit(ray, eye, matrices[i]))
			{
				return true;
			}
		}

		return false;
	}

	void passViewMatrix(shared_ptr<Program> prog)
	{
		auto V = make_shared<MatrixStack>();

		prog->bind();

		V->pushMatrix();
		V->loadIdentity();
		V->lookAt(eye, lookAtPoint, vec3(0, 1, 0));
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(V->topMatrix()));
		V->popMatrix();

		prog->unbind();
	}

	void pickNumberOfPlayers(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog)
	{
		auto V = make_shared<MatrixStack>();
		vec3 ray;
		std::vector<shared_ptr<Shape>> shapes;
		std::vector<mat4> matrices;

		passViewMatrix(prog);

		prog->bind();
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
		SetMaterial(2);

		shapes = std::vector<shared_ptr<Shape>>();
		matrices = std::vector<mat4>();
		//2 players
		M->pushMatrix();
		M->translate(vec3(-8.5, 0, -4));
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		shield->draw(prog);
		shapes.push_back(shield);
		matrices.push_back(M->topMatrix());
		M->popMatrix();

		M->pushMatrix();
		M->translate(vec3(-7, 0, -4));
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		shield->draw(prog);
		shapes.push_back(shield);
		matrices.push_back(M->topMatrix());
		M->popMatrix();
		if (testClick && wasHit(computeRay(P), eye, matrices, shapes))
		{
			MyEnum::print("2");
			numPlayers = 2;
			initPlayers();
			gameState = MyEnum::SETUP;
			testClick = false;
		}


		shapes = std::vector<shared_ptr<Shape>>();
		matrices = std::vector<mat4>();
		//3 players
		M->pushMatrix();
		M->translate(vec3(-4, 0, -4));
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		shield->draw(prog);
		shapes.push_back(shield);
		matrices.push_back(M->topMatrix());
		M->popMatrix();

		M->pushMatrix();
		M->translate(vec3(-2.5, 0, -4));
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		shield->draw(prog);
		shapes.push_back(shield);
		matrices.push_back(M->topMatrix());
		M->popMatrix();

		M->pushMatrix();
		M->translate(vec3(-1, 0, -4));
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		shield->draw(prog);
		shapes.push_back(shield);
		matrices.push_back(M->topMatrix());
		M->popMatrix();
		if (testClick && wasHit(computeRay(P), eye, matrices, shapes))
		{
			MyEnum::print("3");
			numPlayers = 3;
			initPlayers();
			gameState = MyEnum::SETUP;
			testClick = false;
		}


		shapes = std::vector<shared_ptr<Shape>>();
		matrices = std::vector<mat4>();
		//player 4
		M->pushMatrix();
		M->translate(vec3(2, 0, -4));
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		shield->draw(prog);
		shapes.push_back(shield);
		matrices.push_back(M->topMatrix());
		M->popMatrix();

		M->pushMatrix();
		M->translate(vec3(3.5, 0, -4));
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		shield->draw(prog);
		shapes.push_back(shield);
		matrices.push_back(M->topMatrix());
		M->popMatrix();

		M->pushMatrix();
		M->translate(vec3(5, 0, -4));
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		shield->draw(prog);
		shapes.push_back(shield);
		matrices.push_back(M->topMatrix());
		M->popMatrix();

		M->pushMatrix();
		M->translate(vec3(6.5, 0, -4));
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
		shield->draw(prog);
		shapes.push_back(shield);
		matrices.push_back(M->topMatrix());
		M->popMatrix();

		if (testClick && wasHit(computeRay(P), eye, matrices, shapes))
		{
			MyEnum::print("4");
			numPlayers = 4;
			initPlayers();
			gameState = MyEnum::SETUP;
			testClick = false;
		}

		prog->unbind();

	}

	void drawLargestArmy(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog)
	{
		passViewMatrix(prog);

		prog->bind();
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));

		M->pushMatrix();
		M->translate(vec3(-6, 0, 10));
			drawKnight(P, M, prog);

			M->pushMatrix();
			M->translate(vec3(1, 0, 0));
			drawKnight(P, M, prog);
			M->popMatrix();

			M->pushMatrix();
			M->translate(vec3(.5, 0, 1));
			drawKnight(P, M, prog);
			M->popMatrix();

		M->popMatrix();
	}

	void drawLongestRoad(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> M, shared_ptr<Program> prog)
	{
		passViewMatrix(prog);

		prog->bind();
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));

		M->pushMatrix();
		M->translate(vec3(-6, 0, 12));
			glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
			road1->draw(prog);

			M->pushMatrix();
			M->translate(vec3(1, 0, 0));
			glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
			road1->draw(prog);
			M->popMatrix();

			M->pushMatrix();
			M->translate(vec3(.5, 0, 1));
			glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(M->topMatrix()));
			road1->draw(prog);
			M->popMatrix();

		M->popMatrix();
	}

	void render()
	{
		//std::vector<MyEnum::Color> playersToStealFrom = std::vector<MyEnum::Color>();
		static MyEnum::Resource resource1 = MyEnum::water;

		// Get current frame buffer size.
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		glViewport(0, 0, width, height);

		/* Leave this code to just draw the meshes alone */
		float aspect = width/(float)height;

		// Create the matrix stacks
		auto P = make_shared<MatrixStack>();
		auto M = make_shared<MatrixStack>();

		auto MV = make_shared<MatrixStack>();
		
		
		// Apply perspective projection.
		P->pushMatrix();
		P->perspective(45.0f, aspect, 0.01f, 100.0f);

		//redraw for the blur or screen
		if (Moving && freecamera)
		{
			//set up to render to buffer
			glBindFramebuffer(GL_FRAMEBUFFER, frameBuf[0]);
		}
		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		if (gameState == MyEnum::PICK_NUM_PLAYERS)
		{
			pickNumberOfPlayers(P, M, prog);
		}
		else
		{
			drawCurrentPlayer(P, M, prog);
			drawResources(P, M, prog);
			drawScene(P, M, prog);
		}

		if (gameState == MyEnum::SETUP)
		{
			drawFirstSettlementsAndRoad(P, M, prog);
		}
		if (gameState == MyEnum::GAME)
		{
			drawTempHUD(P, M, hudProg1);
			//drawHUD(P, hudProg0);
			//drawHUD(P, prog);
		}
		else if (gameState == MyEnum::BUILDING)
		{
			drawBuildMenu(P, M, prog);
		}
		else if (gameState == MyEnum::BUILD_SETTLEMENT)
		{
			drawPossibleBuildings(P, M, prog, MyEnum::settlement);
		}
		else if (gameState == MyEnum::BUILD_ROAD)
		{
			drawPossibleRoads(P, M, prog, MyEnum::GAME);
		}
		else if (gameState == MyEnum::PLACE_THIEF)
		{
			drawPossibleThief(P, M, prog);
		}
		else if (gameState == MyEnum::STEAL_FROM_PLAYERS)
		{
			gameState = MyEnum::GAME;
		}
		else if (gameState == MyEnum::DEV_CARD)
		{
			drawPossibleDevCards(P, M, prog);
		}
		else if (gameState == MyEnum::ROAD_BUILDING_1)
		{
			drawPossibleRoads(P, M, prog, MyEnum::ROAD_BUILDING_2);
		}
		else if (gameState == MyEnum::ROAD_BUILDING_2)
		{
			drawPossibleRoads(P, M, prog, MyEnum::GAME);
		}
		else if (gameState == MyEnum::YEAR_PLENTY)
		{
			MyEnum::Resource res = chooseResource(P, M, prog);
			if (res != MyEnum::water)
			{
				resource1 = res;
			}
		}
		else if (gameState == MyEnum::YEAR_PLENTY_2)
		{
			chooseResource_2(P, M, prog, resource1);
		}
		else if (gameState == MyEnum::MONOPOLY)
		{
			chooseResource(P, M, prog);
		}

		GameLogic::checkLargestArmy(playerList, numPlayers, &largestArmy);
		int temp = GameLogic::checkLongestRoad(playerList, board, numPlayers, &longestRoad, longestRoadCount);

		if (temp > longestRoadCount)
		{
			longestRoadCount = temp;
		}
		
		if (largestArmy && largestArmy->getColor() == curPlayer->getColor())
		{
			drawLargestArmy(P, M, prog);
		}

		if (longestRoad && longestRoad->getColor() == curPlayer->getColor())
		{
			drawLongestRoad(P, M, prog);
		}

		if (GameLogic::checkWinner(playerList, board, numPlayers, largestArmy, longestRoad))
		{

		}

		P->popMatrix(); 
		
		if (Moving && freecamera)
		{
			for (int i = 0; i < 3; i ++)
			{
				//set up framebuffer
				glBindFramebuffer(GL_FRAMEBUFFER, frameBuf[(i+1)%2]);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				//set up texture
				ProcessImage(texBuf[i%2]);
			}

			//now draw the actual output
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			ProcessImage(texBuf[1]);

			//code to write out the FBO (texture) just once -an example
			if (FirstTime)
			{
				
				FirstTime = false;
			}
		}
	}

	void initHexCoordinates(std::vector<vec3> &hexCoordinates)
	{
		hexCoordinates[0] = vec3(GAMEBOARD_MID_X + -2.598, GAMEBOARD_Y, GAMEBOARD_MID_Z + -4.5);
		hexCoordinates[1] = vec3(GAMEBOARD_MID_X + -.866, GAMEBOARD_Y, GAMEBOARD_MID_Z + -4.5);
		hexCoordinates[2] = vec3(GAMEBOARD_MID_X + .866, GAMEBOARD_Y, GAMEBOARD_MID_Z + -4.5);
		hexCoordinates[3] = vec3(GAMEBOARD_MID_X + 2.598, GAMEBOARD_Y, GAMEBOARD_MID_Z + -4.5);
		hexCoordinates[4] = vec3(GAMEBOARD_MID_X + -3.464, GAMEBOARD_Y, GAMEBOARD_MID_Z + -3);
		hexCoordinates[5] = vec3(GAMEBOARD_MID_X + -1.732, GAMEBOARD_Y, GAMEBOARD_MID_Z + -3);
		hexCoordinates[6] = vec3(GAMEBOARD_MID_X + 0, GAMEBOARD_Y, GAMEBOARD_MID_Z + -3);
		hexCoordinates[7] = vec3(GAMEBOARD_MID_X + 1.732, GAMEBOARD_Y, GAMEBOARD_MID_Z + -3);
		hexCoordinates[8] = vec3(GAMEBOARD_MID_X + 3.464, GAMEBOARD_Y, GAMEBOARD_MID_Z + -3);
		hexCoordinates[9] = vec3(GAMEBOARD_MID_X + -4.33, GAMEBOARD_Y, GAMEBOARD_MID_Z + -1.5);
		hexCoordinates[10] = vec3(GAMEBOARD_MID_X + -2.598, GAMEBOARD_Y, GAMEBOARD_MID_Z + -1.5);
		hexCoordinates[11] = vec3(GAMEBOARD_MID_X + -.866, GAMEBOARD_Y, GAMEBOARD_MID_Z + -1.5);
		hexCoordinates[12] = vec3(GAMEBOARD_MID_X + .866, GAMEBOARD_Y, GAMEBOARD_MID_Z + -1.5);
		hexCoordinates[13] = vec3(GAMEBOARD_MID_X + 2.598, GAMEBOARD_Y, GAMEBOARD_MID_Z + -1.5);
		hexCoordinates[14] = vec3(GAMEBOARD_MID_X + 4.33, GAMEBOARD_Y, GAMEBOARD_MID_Z + -1.5);
		hexCoordinates[15] = vec3(GAMEBOARD_MID_X + -5.196, GAMEBOARD_Y, GAMEBOARD_MID_Z + 0);
		hexCoordinates[16] = vec3(GAMEBOARD_MID_X + -3.464, GAMEBOARD_Y, GAMEBOARD_MID_Z + 0);
		hexCoordinates[17] = vec3(GAMEBOARD_MID_X + -1.732, GAMEBOARD_Y, GAMEBOARD_MID_Z + 0);
		hexCoordinates[18] = vec3(GAMEBOARD_MID_X + 0, GAMEBOARD_Y, GAMEBOARD_MID_Z + 0);
		hexCoordinates[19] = vec3(GAMEBOARD_MID_X + 1.732, GAMEBOARD_Y, GAMEBOARD_MID_Z + 0);
		hexCoordinates[20] = vec3(GAMEBOARD_MID_X + 3.464, GAMEBOARD_Y, GAMEBOARD_MID_Z + 0);
		hexCoordinates[21] = vec3(GAMEBOARD_MID_X + 5.196, GAMEBOARD_Y, GAMEBOARD_MID_Z + 0);
		hexCoordinates[22] = vec3(GAMEBOARD_MID_X + -4.33, GAMEBOARD_Y, GAMEBOARD_MID_Z + 1.5);
		hexCoordinates[23] = vec3(GAMEBOARD_MID_X + -2.598, GAMEBOARD_Y, GAMEBOARD_MID_Z + 1.5);
		hexCoordinates[24] = vec3(GAMEBOARD_MID_X + -.866, GAMEBOARD_Y, GAMEBOARD_MID_Z + 1.5);
		hexCoordinates[25] = vec3(GAMEBOARD_MID_X + .866, GAMEBOARD_Y, GAMEBOARD_MID_Z + 1.5);
		hexCoordinates[26] = vec3(GAMEBOARD_MID_X + 2.598, GAMEBOARD_Y, GAMEBOARD_MID_Z + 1.5);
		hexCoordinates[27] = vec3(GAMEBOARD_MID_X + 4.33, GAMEBOARD_Y, GAMEBOARD_MID_Z + 1.5);
		hexCoordinates[28] = vec3(GAMEBOARD_MID_X + -3.464, GAMEBOARD_Y, GAMEBOARD_MID_Z + 3);
		hexCoordinates[29] = vec3(GAMEBOARD_MID_X + -1.732, GAMEBOARD_Y, GAMEBOARD_MID_Z + 3);
		hexCoordinates[30] = vec3(GAMEBOARD_MID_X + 0, GAMEBOARD_Y, GAMEBOARD_MID_Z + 3);
		hexCoordinates[31] = vec3(GAMEBOARD_MID_X + 1.732, GAMEBOARD_Y, GAMEBOARD_MID_Z + 3);
		hexCoordinates[32] = vec3(GAMEBOARD_MID_X + 3.464, GAMEBOARD_Y, GAMEBOARD_MID_Z + 3);
		hexCoordinates[33] = vec3(GAMEBOARD_MID_X + -2.598, GAMEBOARD_Y, GAMEBOARD_MID_Z + 4.5);
		hexCoordinates[34] = vec3(GAMEBOARD_MID_X + -.866, GAMEBOARD_Y, GAMEBOARD_MID_Z + 4.5);
		hexCoordinates[35] = vec3(GAMEBOARD_MID_X + .866, GAMEBOARD_Y, GAMEBOARD_MID_Z + 4.5);
		hexCoordinates[36] = vec3(GAMEBOARD_MID_X + 2.598, GAMEBOARD_Y, GAMEBOARD_MID_Z + 4.5);
	}

	// helper function to set materials for shading
	void SetMaterial(int i)
	{
		switch (i)
		{
			case 0: //Obsidian
				glUniform3f(prog->getUniform("MatAmb"), 0.05375f, 0.05f, 0.06625f);
				glUniform3f(prog->getUniform("MatDif"), 0.18275f, 0.17f, 0.22525f);
				glUniform3f(prog->getUniform("MatSpec"), 0.332741f, 0.328634f, 0.346435f);
				glUniform1f(prog->getUniform("S"), 38.4);
				break;
			case 1: //turquoise
				glUniform3f(prog->getUniform("MatAmb"), 0.1, 0.18725f, 0.1745f);
				glUniform3f(prog->getUniform("MatDif"), 0.396f, 0.74151f, 0.69102f);
				glUniform3f(prog->getUniform("MatSpec"), 0.297254f, 0.30829f, 0.306678f);
				glUniform1f(prog->getUniform("S"), 12.8);
				break;
			case 2: //silver
				glUniform3f(prog->getUniform("MatAmb"), 0.19225f, 0.19225f, 0.19225f);
				glUniform3f(prog->getUniform("MatDif"), 0.50754f, 0.50754f, 0.50754f);
				glUniform3f(prog->getUniform("MatSpec"), 0.508273, 0.508273, 0.508273);
				glUniform1f(prog->getUniform("S"), 51.2);
				break;
			 case 3: //ruby
				glUniform3f(prog->getUniform("MatAmb"), 0.1745f, 0.01175f, 0.01175f);
				glUniform3f(prog->getUniform("MatDif"), 0.061424f, 0.04136f, 0.04136f);
				glUniform3f(prog->getUniform("MatSpec"), 0.727811, 0.626959, 0.626959);
				glUniform1f(prog->getUniform("S"), 76.8);
				break;
			case 4: // yellow plastic
				glUniform3f(prog->getUniform("MatAmb"), 0.0, 0.0, 0.0);
				glUniform3f(prog->getUniform("MatDif"), 0.5, 0.5, 0.0);
				glUniform3f(prog->getUniform("MatSpec"), 0.6, 0.6, 0.5);
				glUniform1f(prog->getUniform("S"), 32);
				break;
			case 5:
				glUniform3f(prog->getUniform("MatAmb"), 0.0, 0.0, 0.0);
				glUniform3f(prog->getUniform("MatDif"), 0.55, 0.55, 0.55);
				glUniform3f(prog->getUniform("MatSpec"), 0.7, 0.7, 0.7);
				glUniform1f(prog->getUniform("S"), 32);
				break;
			case 6:
				glUniform3f(prog->getUniform("MatAmb"), 0.0, 0.0, 0.0);
				glUniform3f(prog->getUniform("MatDif"), 0.5, 0.0, 0.0);
				glUniform3f(prog->getUniform("MatSpec"), 0.7, 0.6, 0.6);
				glUniform1f(prog->getUniform("S"), 32);
				break;
			case 7:
				glUniform3f(prog->getUniform("MatAmb"), 0.0, 0.1, 0.06);
				glUniform3f(prog->getUniform("MatDif"), 0.0, 0.50980392, 0.50980392);
				glUniform3f(prog->getUniform("MatSpec"), 0.50196078, 0.50196078, 0.50196078);
				glUniform1f(prog->getUniform("S"), 32);
				break;
		}
	}

	void setPlayerColor(MyEnum::Color color)
	{
		switch(color)
		{
			case MyEnum::red:
				glUniform3f(prog->getUniform("MatAmb"), 0.05, 0.0, 0.0);
				glUniform3f(prog->getUniform("MatDif"), 0.5, 0.4, 0.4);
				glUniform3f(prog->getUniform("MatSpec"), 0.7, 0.04, 0.04);
				glUniform1f(prog->getUniform("S"), 10);
				break;
			case MyEnum::blue:
				glUniform3f(prog->getUniform("MatAmb"), 0.0, 0.05, 0.05);
				glUniform3f(prog->getUniform("MatDif"), 0.4, 0.5, 0.5);
				glUniform3f(prog->getUniform("MatSpec"), 0.04, 0.7, 0.7);
				glUniform1f(prog->getUniform("S"), 10);
				break;
			case MyEnum::green:
				glUniform3f(prog->getUniform("MatAmb"), 0.0, 0.05, 0.0);
				glUniform3f(prog->getUniform("MatDif"), 0.4, 0.5, 0.4);
				glUniform3f(prog->getUniform("MatSpec"), 0.04, 0.7, 0.04);
				glUniform1f(prog->getUniform("S"), 10);
				break;
			case MyEnum::white:
				glUniform3f(prog->getUniform("MatAmb"), 0.05, 0.05, 0.05);
				glUniform3f(prog->getUniform("MatDif"), 0.5, 0.5, 0.5);
				glUniform3f(prog->getUniform("MatSpec"), 0.7, 0.7, 0.7);
				glUniform1f(prog->getUniform("S"), 10);
				break;
			case MyEnum::empty:
				MyEnum::print("Passed Empty to setPlayerColor");
				break;
		}
	}

};

int main(int argc, char **argv)
{
	// Where the resources are loaded from
	std::string resourceDir = "../resources";

	if (argc >= 2)
	{
			resourceDir = argv[1];
	}

	Application *application = new Application();

	// Your main will always include a similar set up to establish your window
	// and GL context, etc.

	WindowManager *windowManager = new WindowManager();
	windowManager->init(512, 512);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	// This is the code that will likely change program to program as you
	// may need to initialize or set up different data and state

	application->init(resourceDir);
	application->initGeom(resourceDir);
	application->initGame();

	// Loop until the user closes the window.
	while (! glfwWindowShouldClose(windowManager->getHandle()))
	{
			// Render scene.
			application->render();

			// Swap front and back buffers.
			glfwSwapBuffers(windowManager->getHandle());
			// Poll for and process events.
			glfwPollEvents();
	}

	// Quit program.
	windowManager->shutdown();
	return 0;
}
