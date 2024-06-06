#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"
#include "Headers/ThirdPersonCamera.h"

// Font rendering include
#include "Headers/FontTypeRendering.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"

// Include Terrain
#include "Headers/Terrain.h"

#include "Headers/AnimationUtils.h"

// Include Colision headers functions
#include "Headers/Colisiones.h"

// OpenAL include
#include <AL/alut.h>

//modelo dinamico d euna caja para las sombras
#include "Headers/ShadowBox.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

GLFWwindow* window;

Shader shader;
//Shader con skybox
Shader shaderSkybox;
//Shader con multiples luces
Shader shaderMulLighting;
//Shader para el terreno
//Shader shaderTerrain;
// Shader para dibujar un objeto con solo textura
Shader shaderTexture;
// Shader para el render del buffer de profunidad
Shader shaderDepth;
// Shader para visualizar el buffer de profundidad
Shader shaderViewDepth;

std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());
float distanceFromTarget = 7.0;

Sphere skyboxSphere(20, 20);
Box boxCalle;
Box boxBanqueta;
Box boxEdificio;

Box boxCesped;
Box boxWalls;
Box boxHighway;
Box boxLandingPad;
Sphere esfera1(10, 10);
Box boxCollider;
Sphere sphereCollider(10, 10);
//Cylinder rayModel(10, 10, 1.0, 1.0, 1.0);

Box boxIntro;
Box boxViewDepth;

// Models complex instances
Model modelChoppingTable;
Model modelDishesTable;
Model modelBurnersTable;
Model modelIngredientsTable;


Model modelFoodTruck;

// Lamps
Model modelLamp1;
Model modelLamp2;
Model modelLampPost2;

// Modelos animados
// Chef
Model chefModelAnimate;
// Cliente
Model clienteModelAnimate;
// Terrain model instance
//Terrain terrain(-1, -1, 200, 8, "../Textures/heightmap.png");

//sahdow box 
ShadowBox* shadowBox;

GLuint textureCespedID, textureWallID, textureWindowID, textureHighwayID, textureLandingPadID;
//GLuint textureTerrainRID, textureTerrainGID, textureTerrainBID, textureTerrainBlendMapID;
GLuint textureCalleID, textureBanquetaID, textureEdificioID;
GLuint skyboxTextureID;
GLuint textureInit1ID, textureInit2ID, textureActivaID, textureScreenID;

bool iniciaPartida = false, presionarOpcion = false;

// Modelo para el render del texto
FontTypeRendering::FontTypeRendering* modelText;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

std::string fileNames[6] = { "../Textures/mp_bloodvalley/nochefront.png",
		"../Textures/mp_bloodvalley/nochefront.png",
		"../Textures/mp_bloodvalley/nocheback.png",
		"../Textures/mp_bloodvalley/nosheback.png",
		"../Textures/mp_bloodvalley/nocheleft.png",
		"../Textures/mp_bloodvalley/nocheleft.png" };


bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

// Model matrix definitions
glm::mat4 modelMatrixChoppingTable = glm::mat4(1.0f);
glm::mat4 modelMatrixDishesTable = glm::mat4(1.0f);
glm::mat4 modelMatrixBurnersTable = glm::mat4(1.0f);
glm::mat4 modelMatrixIngredientsTable = glm::mat4(1.0f);

glm::mat4 modelMatrixFoodTruck = glm::mat4(1.0f);
glm::mat4 modelMatrixChef = glm::mat4(1.0f);
glm::mat4 modelMatrixCliente = glm::mat4(1.0f);

//int animationMayowIndex = 1;
int animationChefIndex = 1;
int animationClienteIndex = 1;
float rotDartHead = 0.0, rotDartLeftArm = 0.0, rotDartLeftHand = 0.0, rotDartRightArm = 0.0, rotDartRightHand = 0.0, rotDartLeftLeg = 0.0, rotDartRightLeg = 0.0;
float rotBuzzHead = 0.0, rotBuzzLeftarm = 0.0, rotBuzzLeftForeArm = 0.0, rotBuzzLeftHand = 0.0;
int modelSelected = 0;
bool enableCountSelected = true;

// Variables to animations keyframes
bool saveFrame = false, availableSave = true;
std::ofstream myfile;
std::string fileName = "";
bool record = false;

// Joints interpolations Dart Lego
std::vector<std::vector<float>> keyFramesDartJoints;
std::vector<std::vector<glm::mat4>> keyFramesDart;
int indexFrameDartJoints = 0;
int indexFrameDartJointsNext = 1;
float interpolationDartJoints = 0.0;
int maxNumPasosDartJoints = 20;
int numPasosDartJoints = 0;
int indexFrameDart = 0;
int indexFrameDartNext = 1;
float interpolationDart = 0.0;
int maxNumPasosDart = 200;
int numPasosDart = 0;

// Joints interpolations Buzz
std::vector<std::vector<float>> keyFramesBuzzJoints;
std::vector<std::vector<glm::mat4>> keyFramesBuzz;
int indexFrameBuzzJoints = 0;
int indexFrameBuzzJointsNext = 1;
float interpolationBuzzJoints = 0.0;
int maxNumPasosBuzzJoints = 20;
int numPasosBuzzJoints = 0;
int indexFrameBuzz = 0;
int indexFrameBuzzNext = 1;
float interpolationBuzz = 0.0;
int maxNumPasosBuzz = 100;
int numPasosBuzz = 0;

// Var animate helicopter
float rotHelHelY = 0.0;
float rotHelHelBack = 0.0;

// Var animate lambo dor
int stateDoor = 0;
float dorRotCount = 0.0;

// Lamps position
//15.0f, 1.0f, 0.1f
std::vector<glm::vec3> lamp1Position = {
	glm::vec3(6.0, 0.0, 15.0),
	glm::vec3(6.0, 0.0, -12.0),
	glm::vec3(-10.15, 0, -54.1)
};
std::vector<float> lamp1Orientation = {
	0.0,0.0,0.0
};
//std::vector<glm::vec3> lamp2Position = {
//	glm::vec3(-36.52, 0, -23.24),
//	glm::vec3(-52.73, 0, -3.90)
//};
//std::vector<float> lamp2Orientation = {
//	21.37 + 90, -65.0 + 90
//};

// Blending model unsorted
std::map<std::string, glm::vec3> blendingUnsorted = {
		{"aircraft", glm::vec3(10.0, 0.0, -17.5)},
		{"lambo", glm::vec3(23.0, 0.0, 0.0)},
		{"heli", glm::vec3(5.0, 10.0, -5.0)}
};

double deltaTime;
double currTime, lastTime;


// Jump variables
bool isJump = false;
float GRAVITY = 1.81;
double tmv = 0;
double startTimeJump = 0;

// Colliders
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB;
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > collidersSBB;

// Variables animacion maquina de estados eclipse
const float avance = 0.1;
const float giroEclipse = 0.5f;
float rotWheelsX = 0.0;
float rotWheelsY = 0.0;

// OpenAL Defines
#define NUM_BUFFERS 9
#define NUM_SOURCES 9
#define NUM_ENVIRONMENTS 1
// Listener
ALfloat listenerPos[] = { 0.0, 0.0, 4.0 };
ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
ALfloat listenerOri[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 0.0 };
// Source 0
ALfloat source0Pos[] = { -2.0, 0.0, 0.0 };//chopping
ALfloat source0Vel[] = { 0.0, 0.0, 0.0 };
// Source 1
ALfloat source1Pos[] = { 2.0, 0.0, 0.0 };//burner
ALfloat source1Vel[] = { 0.0, 0.0, 0.0 };
// Source 2
ALfloat source2Pos[] = { 2.0, 0.0, 0.0 };//people
ALfloat source2Vel[] = { 0.0, 0.0, 0.0 };
// Source 3
ALfloat source3Pos[] = { 2.0, 0.0, 0.0 };//victory
ALfloat source3Vel[] = { 0.0, 0.0, 0.0 };
// Source 4
ALfloat source4Pos[] = { 2.0, 0.0, 0.0 };//death
ALfloat source4Vel[] = { 0.0, 0.0, 0.0 };
// Source 5
ALfloat source5Pos[] = { 2.0, 0.0, 0.0 };//click
ALfloat source5Vel[] = { 0.0, 0.0, 0.0 };
// Source 6
ALfloat source6Pos[] = { 2.0, 0.0, 0.0 };//footsteps
ALfloat source6Vel[] = { 0.0, 0.0, 0.0 };
// Source 7
ALfloat source7Pos[] = { 2.0, 0.0, 0.0 };//victory final
ALfloat source7Vel[] = { 0.0, 0.0, 0.0 };
// Source 8
ALfloat source8Pos[] = { 2.0, 0.0, 0.0 };//grab item
ALfloat source8Vel[] = { 0.0, 0.0, 0.0 };
// Buffers
ALuint buffer[NUM_BUFFERS];
ALuint source[NUM_SOURCES];
ALuint environment[NUM_ENVIRONMENTS];
// Configs
ALsizei size, freq;
ALenum format;
ALvoid* data;
int ch;
ALboolean loop;
std::vector<bool> sourcesPlay = { true, true, true };

// Framesbuffers
GLuint depthMap, depthMapFBO;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action,
	int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
			glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
			nullptr);

	if (window == nullptr) {
		std::cerr
			<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
			<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Inicialización de los shaders
	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox_fog.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation_shadow.vs", "../Shaders/multipleLights_shadow.fs");
	//shaderTerrain.initialize("../Shaders/terrain_shadow.vs", "../Shaders/terrain_shadow.fs");
	shaderTexture.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado.fs");
	shaderDepth.initialize("../Shaders/shadow_mapping_depth.vs",
		"../Shaders/shadow_mapping_depth.fs");
	shaderViewDepth.initialize("../Shaders/texturizado.vs",
		"../Shaders/texturizado_depth_view.fs");

	// Inicializacion de los objetos.
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));

	boxCollider.init();
	boxCollider.setShader(&shader);
	boxCollider.setColor(glm::vec4(1.0,1.0,1.0,0.0));

	


	sphereCollider.init();
	sphereCollider.setShader(&shader);
	sphereCollider.setColor(glm::vec4(0.0, 1.0, 1.0, 1.0));
	

	/*rayModel.init();
	rayModel.setShader(&shader);
	rayModel.setColor(glm::vec4(1.0));*/

	boxCesped.init();
	boxCesped.setShader(&shaderMulLighting);

	boxWalls.init();
	boxWalls.setShader(&shaderMulLighting);

	boxHighway.init();
	boxHighway.setShader(&shaderMulLighting);

	boxLandingPad.init();
	boxLandingPad.setShader(&shaderMulLighting);

	esfera1.init();
	esfera1.setShader(&shaderMulLighting);

	boxCalle.init();
	boxCalle.setShader(&shaderMulLighting);

	boxBanqueta.init();
	boxBanqueta.setShader(&shaderMulLighting);

	boxEdificio.init();
	boxEdificio.setShader(&shaderMulLighting);

	boxIntro.init();
	boxIntro.setShader(&shaderTexture);
	boxIntro.setScale(glm::vec3(2.0, 2.0, 1.0));

	boxViewDepth.init();
	boxViewDepth.setShader(&shaderViewDepth);

	/*modelRock.loadModel("../models/rock/rock.obj");
	modelRock.setShader(&shaderMulLighting);*/

	modelFoodTruck.loadModel("../models/foodTruck/foodTruckinside_cage.obj");
	modelFoodTruck.setShader(&shaderMulLighting);

	modelBurnersTable.loadModel("../models/foodTruck/foodtruckinside_burnerstable.obj");
	modelBurnersTable.setShader(&shaderMulLighting);

	modelChoppingTable.loadModel("../models/foodTruck/foodTruckinside_choppingtable.obj");
	modelChoppingTable.setShader(&shaderMulLighting);

	modelDishesTable.loadModel("../models/foodTruck/foodTruckinside_dishestable.obj");
	modelDishesTable.setShader(&shaderMulLighting);

	modelIngredientsTable.loadModel("../models/foodTruck/foodtruckinside_ingredientstable.obj");
	modelIngredientsTable.setShader(&shaderMulLighting);


	//Lamps models
	modelLamp1.loadModel("../models/Street-Lamp-Black/objLamp.obj");
	modelLamp1.setShader(&shaderMulLighting);
	modelLamp2.loadModel("../models/Street_Light/Lamp.obj");
	modelLamp2.setShader(&shaderMulLighting);
	modelLampPost2.loadModel("../models/Street_Light/LampPost.obj");
	modelLampPost2.setShader(&shaderMulLighting);


	//Chef
	chefModelAnimate.loadModel("../models/chef/chefidlerunning.fbx");
	chefModelAnimate.setShader(&shaderMulLighting);

	//Cliente
	clienteModelAnimate.loadModel("../models/cliente/cliente.fbx");
	clienteModelAnimate.setShader(&shaderMulLighting);



	// Terreno
	/*terrain.init();
	terrain.setShader(&shaderTerrain);*/

	// Se inicializa el model de render text
	modelText = new FontTypeRendering::FontTypeRendering(screenWidth, screenHeight);
	modelText->Initialize();

	/*camera->setPosition(glm::vec3(0.0, 3.0, 4.0));
	camera->setDistanceFromTarget(distanceFromTarget);
	camera->setSensitivity(1.0);*/

	camera->setPosition(glm::vec3(-14.0, 11.0, 1.0));

	// Carga de texturas para el skybox
	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	// Tipo de textura CUBE MAP
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		skyboxTexture = Texture(fileNames[i]);
		skyboxTexture.loadImage(true);
		if (skyboxTexture.getData()) {
			glTexImage2D(types[i], 0, skyboxTexture.getChannels() == 3 ? GL_RGB : GL_RGBA, skyboxTexture.getWidth(), skyboxTexture.getHeight(), 0,
				skyboxTexture.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, skyboxTexture.getData());
		}
		else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage();
	}

	Texture textureCalle("../Textures/calle.jpg");
	textureCalle.loadImage();
	glGenTextures(1, &textureCalleID);
	glBindTexture(GL_TEXTURE_2D, textureCalleID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (textureCalle.getData()) {
		glTexImage2D(GL_TEXTURE_2D, 0, textureCalle.getChannels() == 3 ? GL_RGB : GL_RGBA, textureCalle.getWidth(), textureCalle.getHeight(), 0,
			textureCalle.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureCalle.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureCalle.freeImage();

	Texture textureBanqueta("../Textures/sidewalk.jpg");
	textureBanqueta.loadImage();
	glGenTextures(1, &textureBanquetaID);
	glBindTexture(GL_TEXTURE_2D, textureBanquetaID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (textureBanqueta.getData()) {
		glTexImage2D(GL_TEXTURE_2D, 0, textureBanqueta.getChannels() == 3 ? GL_RGB : GL_RGBA, textureBanqueta.getWidth(), textureBanqueta.getHeight(), 0,
			textureBanqueta.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureBanqueta.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureBanqueta.freeImage();

	Texture textureEdificio("../Textures/city.jpg");
	textureEdificio.loadImage();
	glGenTextures(1, &textureEdificioID);
	glBindTexture(GL_TEXTURE_2D, textureEdificioID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (textureEdificio.getData()) {
		glTexImage2D(GL_TEXTURE_2D, 0, textureEdificio.getChannels() == 3 ? GL_RGB : GL_RGBA, textureEdificio.getWidth(), textureEdificio.getHeight(), 0,
			textureEdificio.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureEdificio.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureEdificio.freeImage();

	// Definiendo la textura a utilizar
	Texture textureCesped("../Textures/grassy2.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	textureCesped.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureCespedID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureCespedID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (textureCesped.getData()) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		std::cout << "Numero de canales :=> " << textureCesped.getChannels() << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, textureCesped.getChannels() == 3 ? GL_RGB : GL_RGBA, textureCesped.getWidth(), textureCesped.getHeight(), 0,
			textureCesped.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureCesped.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureCesped.freeImage();

	// Definiendo la textura a utilizar
	Texture textureWall("../Textures/whiteWall.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	textureWall.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureWallID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureWallID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (textureWall.getData()) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, textureWall.getChannels() == 3 ? GL_RGB : GL_RGBA, textureWall.getWidth(), textureWall.getHeight(), 0,
			textureWall.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureWall.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureWall.freeImage();

	// Definiendo la textura a utilizar
	Texture textureWindow("../Textures/ventana.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	textureWindow.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureWindowID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureWindowID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (textureWindow.getData()) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, textureWindow.getChannels() == 3 ? GL_RGB : GL_RGBA, textureWindow.getWidth(), textureWindow.getHeight(), 0,
			textureWindow.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureWindow.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureWindow.freeImage();

	// Definiendo la textura a utilizar
	Texture textureHighway("../Textures/highway.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	textureHighway.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureHighwayID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureHighwayID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (textureHighway.getData()) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, textureHighway.getChannels() == 3 ? GL_RGB : GL_RGBA, textureHighway.getWidth(), textureHighway.getHeight(), 0,
			textureHighway.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureHighway.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureHighway.freeImage();

	// Definiendo la textura
	Texture textureLandingPad("../Textures/landingPad.jpg");
	textureLandingPad.loadImage(); // Cargar la textura
	glGenTextures(1, &textureLandingPadID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureLandingPadID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if (textureLandingPad.getData()) {
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureLandingPad.getChannels() == 3 ? GL_RGB : GL_RGBA, textureLandingPad.getWidth(), textureLandingPad.getHeight(), 0,
			textureLandingPad.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureLandingPad.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Fallo la carga de textura" << std::endl;
	textureLandingPad.freeImage(); // Liberamos memoria


	// Definiendo la textura
	Texture textureIntro1("../Textures/Intro1.png");
	textureIntro1.loadImage(); // Cargar la textura
	glGenTextures(1, &textureInit1ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureInit1ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if (textureIntro1.getData()) {
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureIntro1.getChannels() == 3 ? GL_RGB : GL_RGBA, textureIntro1.getWidth(), textureIntro1.getHeight(), 0,
			textureIntro1.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureIntro1.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Fallo la carga de textura" << std::endl;
	textureIntro1.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureIntro2("../Textures/Intro2.png");
	textureIntro2.loadImage(); // Cargar la textura
	glGenTextures(1, &textureInit2ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureInit2ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if (textureIntro2.getData()) {
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureIntro2.getChannels() == 3 ? GL_RGB : GL_RGBA, textureIntro2.getWidth(), textureIntro2.getHeight(), 0,
			textureIntro2.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureIntro2.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Fallo la carga de textura" << std::endl;
	textureIntro2.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureScreen("../Textures/Screen.png");
	textureScreen.loadImage(); // Cargar la textura
	glGenTextures(1, &textureScreenID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureScreenID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if (textureScreen.getData()) {
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureScreen.getChannels() == 3 ? GL_RGB : GL_RGBA, textureScreen.getWidth(), textureScreen.getHeight(), 0,
			textureScreen.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureScreen.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Fallo la carga de textura" << std::endl;
	textureScreen.freeImage(); // Liberamos memoria

	/*******************************************
	 * OpenAL init
	 *******************************************/
	alutInit(0, nullptr);
	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);
	alGetError(); // clear any error messages
	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating buffers !!\n");
		exit(1);
	}
	else {
		printf("init() - No errors yet.");
	}
	// Generate buffers, or else no sound will happen!
	alGenBuffers(NUM_BUFFERS, buffer);
	buffer[0] = alutCreateBufferFromFile("../sounds/chopping.wav");
	buffer[1] = alutCreateBufferFromFile("../sounds/fire.wav");
	buffer[2] = alutCreateBufferFromFile("../sounds/ambience.wav");
	buffer[3] = alutCreateBufferFromFile("../sounds/victory.wav");
	buffer[4] = alutCreateBufferFromFile("../sounds/death.wav");
	buffer[5] = alutCreateBufferFromFile("../sounds/click.wav");
	buffer[6] = alutCreateBufferFromFile("../sounds/footsteps.wav");
	buffer[7] = alutCreateBufferFromFile("../sounds/victoryfinal.wav");
	buffer[8] = alutCreateBufferFromFile("../sounds/grab-item.wav");

	int errorAlut = alutGetError();
	if (errorAlut != ALUT_ERROR_NO_ERROR) {
		printf("- Error open files with alut %d !!\n", errorAlut);
		exit(2);
	}

	alGetError(); /* clear error */
	alGenSources(NUM_SOURCES, source);

	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating sources !!\n");
		exit(2);
	}
	else {
		printf("init - no errors after alGenSources\n");
	}
	alSourcef(source[0], AL_PITCH, 1.0f);
	alSourcef(source[0], AL_GAIN, 3.0f);
	alSourcefv(source[0], AL_POSITION, source0Pos);
	alSourcefv(source[0], AL_VELOCITY, source0Vel);
	alSourcei(source[0], AL_BUFFER, buffer[0]);
	alSourcei(source[0], AL_LOOPING, AL_TRUE);
	alSourcef(source[0], AL_MAX_DISTANCE, 2000);

	alSourcef(source[1], AL_PITCH, 1.0f);
	alSourcef(source[1], AL_GAIN, 0.5f);
	alSourcefv(source[1], AL_POSITION, source1Pos);
	alSourcefv(source[1], AL_VELOCITY, source1Vel);
	alSourcei(source[1], AL_BUFFER, buffer[1]);
	alSourcei(source[1], AL_LOOPING, AL_TRUE);
	alSourcef(source[1], AL_MAX_DISTANCE, 1000);

	alSourcef(source[2], AL_PITCH, 1.0f);
	alSourcef(source[2], AL_GAIN, 0.3f);
	alSourcefv(source[2], AL_POSITION, source2Pos);
	alSourcefv(source[2], AL_VELOCITY, source2Vel);
	alSourcei(source[2], AL_BUFFER, buffer[2]);
	alSourcei(source[2], AL_LOOPING, AL_TRUE);
	alSourcef(source[2], AL_MAX_DISTANCE, 2000);

	alSourcef(source[3], AL_PITCH, 1.0f);
	alSourcef(source[3], AL_GAIN, 0.3f);
	alSourcefv(source[3], AL_POSITION, source3Pos);
	alSourcefv(source[3], AL_VELOCITY, source3Vel);
	alSourcei(source[3], AL_BUFFER, buffer[3]);
	alSourcei(source[3], AL_LOOPING, AL_TRUE);
	alSourcef(source[3], AL_MAX_DISTANCE, 2000);

	alSourcef(source[4], AL_PITCH, 1.0f);
	alSourcef(source[4], AL_GAIN, 0.3f);
	alSourcefv(source[4], AL_POSITION, source4Pos);
	alSourcefv(source[4], AL_VELOCITY, source4Vel);
	alSourcei(source[4], AL_BUFFER, buffer[4]);
	alSourcei(source[4], AL_LOOPING, AL_TRUE);
	alSourcef(source[4], AL_MAX_DISTANCE, 2000);

	alSourcef(source[5], AL_PITCH, 1.0f);
	alSourcef(source[5], AL_GAIN, 0.3f);
	alSourcefv(source[5], AL_POSITION, source5Pos);
	alSourcefv(source[5], AL_VELOCITY, source5Vel);
	alSourcei(source[5], AL_BUFFER, buffer[5]);
	alSourcei(source[5], AL_LOOPING, AL_TRUE);
	alSourcef(source[5], AL_MAX_DISTANCE, 2000);

	alSourcef(source[6], AL_PITCH, 1.0f);
	alSourcef(source[6], AL_GAIN, 0.3f);
	alSourcefv(source[6], AL_POSITION, source6Pos);
	alSourcefv(source[6], AL_VELOCITY, source6Vel);
	alSourcei(source[6], AL_BUFFER, buffer[6]);
	alSourcei(source[6], AL_LOOPING, AL_TRUE);
	alSourcef(source[6], AL_MAX_DISTANCE, 2000);

	alSourcef(source[7], AL_PITCH, 1.0f);
	alSourcef(source[7], AL_GAIN, 0.3f);
	alSourcefv(source[7], AL_POSITION, source7Pos);
	alSourcefv(source[7], AL_VELOCITY, source7Vel);
	alSourcei(source[7], AL_BUFFER, buffer[7]);
	alSourcei(source[7], AL_LOOPING, AL_TRUE);
	alSourcef(source[7], AL_MAX_DISTANCE, 2000);

	alSourcef(source[8], AL_PITCH, 1.0f);
	alSourcef(source[8], AL_GAIN, 0.3f);
	alSourcefv(source[8], AL_POSITION, source8Pos);
	alSourcefv(source[8], AL_VELOCITY, source8Vel);
	alSourcei(source[8], AL_BUFFER, buffer[8]);
	alSourcei(source[8], AL_LOOPING, AL_TRUE);
	alSourcef(source[8], AL_MAX_DISTANCE, 2000);

	// Inicializar el frame buffer para almacenar la profundidad del render desde el 
	//punto de vista de la luz
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Shaders Delete
	shader.destroy();
	shaderMulLighting.destroy();
	shaderSkybox.destroy();
	//shaderTerrain.destroy();
	shaderDepth.destroy();
	shaderViewDepth.destroy();

	// Basic objects Delete
	skyboxSphere.destroy();
	boxCesped.destroy();
	boxWalls.destroy();
	boxHighway.destroy();
	boxLandingPad.destroy();
	esfera1.destroy();
	boxCollider.destroy();
	sphereCollider.destroy();
	//rayModel.destroy();
	boxIntro.destroy();
	boxViewDepth.destroy();
	boxCalle.destroy();
	boxBanqueta.destroy();
	boxEdificio.destroy();

	// Custom objects Delete
	
	modelBurnersTable.destroy();
	modelChoppingTable.destroy();
	modelDishesTable.destroy();
	modelIngredientsTable.destroy();

	modelLamp1.destroy();
	modelLamp2.destroy();
	modelLampPost2.destroy();

	modelFoodTruck.destroy();

	chefModelAnimate.destroy();
	clienteModelAnimate.destroy();

	// Terrains objects Delete
	//terrain.destroy();

	// Textures Delete
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureCespedID);
	glDeleteTextures(1, &textureWallID);
	glDeleteTextures(1, &textureWindowID);
	glDeleteTextures(1, &textureHighwayID);
	glDeleteTextures(1, &textureLandingPadID);

	glDeleteTextures(1, &textureInit1ID);
	glDeleteTextures(1, &textureInit2ID);
	glDeleteTextures(1, &textureScreenID);
	glDeleteTextures(1, &textureCalleID);
	glDeleteTextures(1, &textureBanquetaID);
	glDeleteTextures(1, &textureEdificioID);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &depthMapFBO);

	// Cube Maps Delete
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDeleteTextures(1, &skyboxTextureID);
}

void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	distanceFromTarget -= yoffset;
	camera->setDistanceFromTarget(distanceFromTarget);
}

void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}

	if (!iniciaPartida) {
		bool presionarEnter = glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS;
		if (textureActivaID == textureInit1ID && presionarEnter) {
			iniciaPartida = true;
			textureActivaID = textureScreenID;
		}
		else if (!presionarOpcion && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
			presionarOpcion = true;
			if (textureActivaID == textureInit1ID)
				textureActivaID = textureInit2ID;
			else if (textureActivaID == textureInit2ID)
				textureActivaID = textureInit1ID;
		}
		else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
			presionarOpcion = false;
	}

	if (glfwJoystickPresent(GLFW_JOYSTICK_1) == GL_TRUE) {
		std::cout << "Esta presente el joystick" << std::endl;
		int axesCount, buttonCount;
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
		std::cout << "Número de ejes disponibles :=>" << axesCount << std::endl;
		std::cout << "Left Stick X axis: " << axes[0] << std::endl;
		std::cout << "Left Stick Y axis: " << axes[1] << std::endl;
		std::cout << "Left Trigger/L2: " << axes[2] << std::endl;
		std::cout << "Right Stick X axis: " << axes[3] << std::endl;
		std::cout << "Right Stick Y axis: " << axes[4] << std::endl;
		std::cout << "Right Trigger/R2: " << axes[5] << std::endl;

		

		const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
		std::cout << "Número de botones disponibles :=>" << buttonCount << std::endl;
		if (buttons[0] == GLFW_PRESS)
			std::cout << "Se presiona x" << std::endl;

		if (!isJump && buttons[0] == GLFW_PRESS) {
			isJump = true;
			startTimeJump = currTime;
			tmv = 0;
		}
	}


	offsetX = 0;
	offsetY = 0;

	// Seleccionar modelo
	if (enableCountSelected && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
		enableCountSelected = false;
		modelSelected++;
		if (modelSelected > 1)
			modelSelected = 0;
	
		std::cout << "modelSelected:" << modelSelected << std::endl;
	}
	else if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
		enableCountSelected = true;

	// Guardar key frames
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		record = true;
		if (myfile.is_open())
			myfile.close();
		myfile.open(fileName);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE
		&& glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		record = false;
		myfile.close();
		if (modelSelected == 1)
			keyFramesDartJoints = getKeyRotFrames(fileName);
		if (modelSelected == 2)
			keyFramesDart = getKeyFrames(fileName);
		if (modelSelected == 3)
			keyFramesBuzzJoints = getKeyRotFrames(fileName);
		if (modelSelected == 4)
			keyFramesBuzz = getKeyFrames(fileName);
	}
	if (availableSave && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		saveFrame = true;
		availableSave = false;
	}if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
		availableSave = true;

	

	// Controles de Chef
	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		modelMatrixChef = glm::rotate(modelMatrixChef, 0.02f, glm::vec3(0, 1, 0));
		animationChefIndex = 2;
	}
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		modelMatrixChef = glm::rotate(modelMatrixChef, -0.02f, glm::vec3(0, 1, 0));
		animationChefIndex = 2;
	}
	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		modelMatrixChef = glm::translate(modelMatrixChef, glm::vec3(0.05, 0.0, 0.0));
		animationChefIndex = 2;
	}
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		modelMatrixChef = glm::translate(modelMatrixChef, glm::vec3(-0.05, 0.0, 0.0));
		animationChefIndex = 2;
	}

	bool keySpaceStatus = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
	if (!isJump && keySpaceStatus) {
		isJump = true;
		startTimeJump = currTime;
		tmv = 0;
	}

	glfwPollEvents();
	return continueApplication;
}

void prepareScene() {

	/*terrain.setShader(&shaderTerrain);*/

	//modelRock.setShader(&shaderMulLighting);
	modelFoodTruck.setShader(&shaderMulLighting);
	modelBurnersTable.setShader(&shaderMulLighting);
	modelChoppingTable.setShader(&shaderMulLighting);
	modelDishesTable.setShader(&shaderMulLighting);
	modelIngredientsTable.setShader(&shaderMulLighting);

	

	//Lamp models
	modelLamp1.setShader(&shaderMulLighting);
	modelLamp2.setShader(&shaderMulLighting);
	modelLampPost2.setShader(&shaderMulLighting);

	

	//Chef
	chefModelAnimate.setShader(&shaderMulLighting);

	//Cliente
	clienteModelAnimate.setShader(&shaderMulLighting);

	
}

void prepareDepthScene() {

	//terrain.setShader(&shaderDepth);

	//modelRock.setShader(&shaderDepth);
	modelFoodTruck.setShader(&shaderDepth);
	modelBurnersTable.setShader(&shaderDepth);
	modelChoppingTable.setShader(&shaderDepth);
	modelDishesTable.setShader(&shaderDepth);
	modelIngredientsTable.setShader(&shaderDepth);

	

	//Lamp models
	modelLamp1.setShader(&shaderDepth);
	modelLamp2.setShader(&shaderDepth);
	modelLampPost2.setShader(&shaderDepth);

	

	// Chef
	chefModelAnimate.setShader(&shaderDepth);
	
	// Cliente
	clienteModelAnimate.setShader(&shaderDepth);

	
}

void renderSolidScene() {
	

	/*******************************************
	 * Custom objects obj
	 *******************************************/


	// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
	glActiveTexture(GL_TEXTURE0);

	modelFoodTruck.render(modelMatrixFoodTruck);
	modelBurnersTable.render(modelMatrixBurnersTable);
	modelChoppingTable.render(modelMatrixChoppingTable);
	modelDishesTable.render(modelMatrixDishesTable);
	modelIngredientsTable.render(modelMatrixIngredientsTable);


	// Render lamp
	for (int i = 0; i < lamp1Position.size(); i++) {
		//lamp1Position[i].y = terrain.getHeightTerrain(lamp1Position[i].x, lamp1Position[i].z);
		modelLamp1.setPosition(lamp1Position[i]);
		modelLamp1.setScale(glm::vec3(0.7));
		modelLamp1.setOrientation(glm::vec3(0, lamp1Orientation[i], 0));
		modelLamp1.render();
	}
	//for (int i = 0; i < lamp2Position.size(); i++) {
	//	//lamp2Position[i].y = terrain.getHeightTerrain(lamp2Position[i].x, lamp2Position[i].z);
	//	modelLamp2.setPosition(lamp2Position[i]);
	//	modelLamp2.setScale(glm::vec3(0.5));
	//	modelLamp2.setOrientation(glm::vec3(0, lamp2Orientation[i], 0));
	//	modelLamp2.render();
	//	modelLampPost2.setPosition(lamp2Position[i]);
	//	modelLampPost2.setScale(glm::vec3(0.5));
	//	modelLampPost2.setOrientation(glm::vec3(0, lamp2Orientation[i], 0));
	//	modelLampPost2.render();
	//}

	

	//Chef
	glm::mat4 modelMatrixChefBody = glm::mat4(modelMatrixChef);
	modelMatrixChefBody = glm::scale(modelMatrixChefBody, glm::vec3(0.007f));
	chefModelAnimate.setAnimationIndex(animationChefIndex);
	chefModelAnimate.render(modelMatrixChefBody);
	//animationChefIndex = 1;

	// Cliente
	glm::mat4 modelMatrixClienteBody = glm::mat4(modelMatrixCliente);
	modelMatrixClienteBody = glm::scale(modelMatrixClienteBody, glm::vec3(0.007f));
	clienteModelAnimate.setAnimationIndex(animationClienteIndex);
	clienteModelAnimate.render(modelMatrixClienteBody);


	/*******************************************
	* Calle
	*******************************************/
	boxCalle.setScale(glm::vec3(70.0f, 0.05f, 10.0f));
	boxCalle.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	boxCalle.setOrientation(glm::vec3(0.0f, 90.0f, 0.0f));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureCalleID);
	shaderMulLighting.setInt("texture1", 0);
	//shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 2.0)));
	boxCalle.render();
	//shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(1.0, 1.0)));
	glBindTexture(GL_TEXTURE_2D, 0);

	boxCalle.setScale(glm::vec3(60.0f, 0.05f, 10.0f));
	boxCalle.setPosition(glm::vec3(-10.0f, 0.0f, 0.0f));
	boxCalle.setOrientation(glm::vec3(0.0f, 90.0f, 0.0f));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureCalleID);
	shaderMulLighting.setInt("texture1", 0);
	//shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 2.0)));
	boxCalle.render();
	//shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(1.0, 1.0)));
	glBindTexture(GL_TEXTURE_2D, 0);

	boxCalle.setScale(glm::vec3(60.0f, 0.05f, 10.0f));
	boxCalle.setPosition(glm::vec3(-20.0f, 0.0f, 0.0f));
	boxCalle.setOrientation(glm::vec3(0.0f, 90.0f, 0.0f));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureCalleID);
	shaderMulLighting.setInt("texture1", 0);
	//shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 2.0)));
	boxCalle.render();
	//shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(1.0, 1.0)));
	glBindTexture(GL_TEXTURE_2D, 0);

	/*******************************************
	 * Banqueta
	 *******************************************/
	boxBanqueta.setScale(glm::vec3(70.0f, 0.5f, 20.0f));
	boxBanqueta.setPosition(glm::vec3(15.0f, 0.0f, 0.0f));
	boxBanqueta.setOrientation(glm::vec3(0.0f, 90.0f, 0.0f));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureBanquetaID);
	shaderMulLighting.setInt("texture1", 0);
	//shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 2.0)));
	boxBanqueta.render();
	//shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(1.0, 1.0)));
	glBindTexture(GL_TEXTURE_2D, 0);

	/*******************************************
	 * Edificio
	 *******************************************/
	boxEdificio.setScale(glm::vec3(70.0f, 0.3f, 20.0f));
	boxEdificio.setPosition(glm::vec3(15.0f, 6.0f, 0.0f));
	boxEdificio.setOrientation(glm::vec3(90.0f, 90.0f, 0.0f));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureEdificioID);
	shaderMulLighting.setInt("texture1", 0);
	//shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 2.0)));
	boxEdificio.render();
	//shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(1.0, 1.0)));
	glBindTexture(GL_TEXTURE_2D, 0);


	/*******************************************
	 * Skybox
	 *******************************************/
	GLint oldCullFaceMode;
	GLint oldDepthFuncMode;
	// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
	glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
	glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
	shaderSkybox.setFloat("skybox", 0);
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);
	glActiveTexture(GL_TEXTURE0);
	skyboxSphere.render();
	glCullFace(oldCullFaceMode);
	glDepthFunc(oldDepthFuncMode);
}

void renderAlphaScene(bool render = true) {
	/**********Render de transparencias***************/
	/**********
	 * Update the position with alpha objects
	 */
	 // Update the aircraft
	//blendingUnsorted.find("aircraft")->second = glm::vec3(modelMatrixAircraft[3]);
	//// Update the lambo
	//blendingUnsorted.find("lambo")->second = glm::vec3(modelMatrixLambo[3]);
	//// Update the helicopter
	//blendingUnsorted.find("heli")->second = glm::vec3(modelMatrixHeli[3]);

	/**********
	 * Sorter with alpha objects
	 */
	std::map<float, std::pair<std::string, glm::vec3>> blendingSorted;
	std::map<std::string, glm::vec3>::iterator itblend;
	for (itblend = blendingUnsorted.begin(); itblend != blendingUnsorted.end(); itblend++) {
		float distanceFromView = glm::length(camera->getPosition() - itblend->second);
		blendingSorted[distanceFromView] = std::make_pair(itblend->first, itblend->second);
	}

	/**********
	 * Render de las transparencias
	 */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	for (std::map<float, std::pair<std::string, glm::vec3> >::reverse_iterator it = blendingSorted.rbegin(); it != blendingSorted.rend(); it++) {
		if (it->second.first.compare("aircraft") == 0) {
			
		}
		else if (it->second.first.compare("lambo") == 0) {
			
		}
		else if (it->second.first.compare("heli") == 0) {
			
		}
	}
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	if (render) {
		/************Render de imagen de frente**************/
		shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
		shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureActivaID);
		shaderTexture.setInt("outTexture", 0);
		glEnable(GL_BLEND);
		boxIntro.render();
		glDisable(GL_BLEND);

		modelText->render("Texto en OpenGL", -1, 0, 1, 0, 0, 24);
	}
}

void renderScene() {
	renderSolidScene();
	renderAlphaScene(false);
}

void applicationLoop() {
	bool psi = true;

	glm::vec3 axis;
	glm::vec3 target;
	float angleTarget;

	//modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(27.5, 0, 30.0));
	//modelMatrixEclipse = glm::rotate(modelMatrixEclipse, glm::radians(180.0f), glm::vec3(0, 1, 0));
	int state = 0;
	float advanceCount = 0.0;
	float rotCount = 0.0;
	int numberAdvance = 0;
	int maxAdvance = 0.0;

	//matrixModelRock = glm::translate(matrixModelRock, glm::vec3(-3.0, 0.0, 2.0));

	modelMatrixFoodTruck = glm::translate(modelMatrixFoodTruck, glm::vec3(1.0, 0.0, 1.0));
	modelMatrixFoodTruck = glm::scale(modelMatrixFoodTruck, glm::vec3(0.5, 0.5, 0.5));

	modelMatrixBurnersTable = glm::translate(modelMatrixBurnersTable, glm::vec3(1.0, -0.3, 1.0));
	modelMatrixBurnersTable = glm::scale(modelMatrixBurnersTable, glm::vec3(0.5, 0.5, 0.5));

	modelMatrixChoppingTable = glm::translate(modelMatrixChoppingTable, glm::vec3(1.0, 0.1, 1.0));
	modelMatrixChoppingTable = glm::scale(modelMatrixChoppingTable, glm::vec3(0.5, 0.5, 0.5));

	modelMatrixDishesTable = glm::translate(modelMatrixDishesTable, glm::vec3(1.0, 0.0, 1.0));
	modelMatrixDishesTable = glm::scale(modelMatrixDishesTable, glm::vec3(0.5, 0.5, 0.5));

	modelMatrixIngredientsTable = glm::translate(modelMatrixIngredientsTable, glm::vec3(1.0, 0.0, 1.0));
	modelMatrixIngredientsTable = glm::scale(modelMatrixIngredientsTable, glm::vec3(0.5, 0.5, 0.5));


	modelMatrixChef = glm::translate(modelMatrixChef, glm::vec3(1.0f, 0.5f, 0.1f));

	modelMatrixCliente = glm::translate(modelMatrixCliente, glm::vec3(15.0f, 1.0f, 0.1f));

	// Variables to interpolation key frames
	fileName = "../animaciones/animation_dart_joints.txt";
	keyFramesDartJoints = getKeyRotFrames(fileName);
	keyFramesDart = getKeyFrames("../animaciones/animation_dart.txt");
	keyFramesBuzzJoints = getKeyRotFrames("../animaciones/animation_buzz_joints.txt");
	keyFramesBuzz = getKeyFrames("../animaciones/animation_buzz.txt");

	lastTime = TimeManager::Instance().GetTime();

	textureActivaID = textureInit1ID;

	glm::vec3 lightPos = glm::vec3(10.0, 10.0, -10.0);

	shadowBox = new ShadowBox(-lightPos, camera.get(), 15.0f, 0.1, 45.0f);

	while (psi) {
		currTime = TimeManager::Instance().GetTime();
		if (currTime - lastTime < 0.016666667) {
			glfwPollEvents();
			continue;
		}
		lastTime = currTime;
		TimeManager::Instance().CalculateFrameRate(true);
		deltaTime = TimeManager::Instance().DeltaTime;
		psi = processInput(true);

		std::map<std::string, bool> collisionDetection;

		// Variables donde se guardan las matrices de cada articulacion por 1 frame
		std::vector<float> matrixDartJoints;
		std::vector<glm::mat4> matrixDart;
		std::vector<float> matrixBuzzJoints;
		std::vector<glm::mat4> matrixBuzz;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)screenWidth / (float)screenHeight, 0.01f, 100.0f);

		/*if (modelSelected == 1) {
			axis = glm::axis(glm::quat_cast(modelMatrixDart));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixDart));
			target = modelMatrixDart[3];
		}
		else {*/
			/*axis = glm::axis(glm::quat_cast(modelMatrixMayow));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixMayow));
			target = modelMatrixMayow[3];*/
		//}

		//if (std::isnan(angleTarget))
		//	angleTarget = 0.0;
		//if (axis.y < 0)
		//	angleTarget = -angleTarget;
		//if (modelSelected == 1)
		//	angleTarget -= glm::radians(90.0f);
		//camera->setCameraTarget(target);
		//camera->setAngleTarget(angleTarget);
		//camera->updateCamera();
		glm::mat4 view = camera->getViewMatrix();

		shadowBox->update(screenWidth, screenHeight);
		glm::vec3 centerBox = shadowBox->getCenter();

		// Projection Light shadow mapping
		glm::mat4 lightProjection = glm::mat4(1.0f), lightView = glm::mat4(1.0f);
		glm::mat4 lightSpaceMatrix;
		float near_plane = 0.1, far_plane = 20.0f;
		//	lightProjection = glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, near_plane, far_plane);
		lightProjection[0][0] = 2.0 / shadowBox->getWidth();
		lightProjection[1][1] = 2.0 / shadowBox->getHeight();
		lightProjection[2][2] = -2.0 / shadowBox->getLength();
		lightProjection[3][3] = 1.0;
		//lightView = glm::lookAt(lightPos, glm::vec3(0.0), glm::vec3(0, 1, 0));
		lightView = glm::lookAt(centerBox, centerBox + glm::normalize(-lightPos), glm::vec3(0, 1, 0));
		lightSpaceMatrix = lightProjection * lightView;
		shaderDepth.setMatrix4("lightSpaceMatrix", 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

		// Settea la matriz de vista y projection al shader con solo color
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
			glm::value_ptr(glm::mat4(glm::mat3(view))));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false,
			glm::value_ptr(view));
		// Settea la matriz de vista y projection al shader con multiples luces
		/*shaderTerrain.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderTerrain.setMatrix4("view", 1, false,
			glm::value_ptr(view));*/

		/*******************************************
		 * Propiedades de neblina
		 *******************************************/
		shaderMulLighting.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.2, 0.3, 0.6))); //azul
		//shaderTerrain.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));
		shaderSkybox.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.2, 0.3, 0.6)));

		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.4, 0.4)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.7071, -0.7071, -0.7071)));

		/*shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderTerrain.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTerrain.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.4, 0.4, 0.4)));
		shaderTerrain.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.7071, -0.7071, -0.7071)));*/

		/*******************************************
		 * Propiedades SpotLights
		 *******************************************/
		//shaderMulLighting.setInt("spotLightCount", 1);
		////shaderTerrain.setInt("spotLightCount", 1);
		//glm::vec3 spotPosition = glm::vec3(modelMatrixHeli * glm::vec4(0.0, 0.2, 1.75, 1.0));
		//shaderMulLighting.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		//shaderMulLighting.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		//shaderMulLighting.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		//shaderMulLighting.setVectorFloat3("spotLights[0].position", glm::value_ptr(spotPosition));
		//shaderMulLighting.setVectorFloat3("spotLights[0].direction", glm::value_ptr(glm::vec3(0, -1, 0)));
		//shaderMulLighting.setFloat("spotLights[0].constant", 1.0);
		//shaderMulLighting.setFloat("spotLights[0].linear", 0.07);
		//shaderMulLighting.setFloat("spotLights[0].quadratic", 0.03);
		//shaderMulLighting.setFloat("spotLights[0].cutOff", cos(glm::radians(12.5f)));
		//shaderMulLighting.setFloat("spotLights[0].outerCutOff", cos(glm::radians(15.0f)));
		/*shaderTerrain.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
		shaderTerrain.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTerrain.setVectorFloat3("spotLights[0].position", glm::value_ptr(spotPosition));
		shaderTerrain.setVectorFloat3("spotLights[0].direction", glm::value_ptr(glm::vec3(0, -1, 0)));
		shaderTerrain.setFloat("spotLights[0].constant", 1.0);
		shaderTerrain.setFloat("spotLights[0].linear", 0.07);
		shaderTerrain.setFloat("spotLights[0].quadratic", 0.03);
		shaderTerrain.setFloat("spotLights[0].cutOff", cos(glm::radians(12.5f)));
		shaderTerrain.setFloat("spotLights[0].outerCutOff", cos(glm::radians(15.0f)));
		shaderTerrain.setMatrix4("lightSpaceMatrix", 1, false, glm::value_ptr(lightSpaceMatrix));*/
		shaderMulLighting.setMatrix4("lightSpaceMatrix", 1, false, glm::value_ptr(lightSpaceMatrix));

		/*******************************************
		 * Propiedades PointLights
		 *******************************************/
		shaderMulLighting.setInt("pointLightCount", lamp1Position.size() );
		//shaderTerrain.setInt("pointLightCount", lamp1Position.size() + lamp2Position.size());
		for (int i = 0; i < lamp1Position.size(); i++) {
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, lamp1Position[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lamp1Orientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(0.5));
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0.0, 10.35, 0));
			glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.48, 0.03)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].constant", 0.7);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.02);
			/*shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.02);*/
		}
		//for (int i = 0; i < lamp2Position.size(); i++) {
		//	glm::mat4 matrixAdjustLamp = glm::mat4(1.0);
		//	matrixAdjustLamp = glm::translate(matrixAdjustLamp, lamp2Position[i]);
		//	matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lamp2Orientation[i]), glm::vec3(0, 1, 0));
		//	matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(1.0));
		//	matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0.75, 5.0, 0));
		//	glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
		//	shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
		//	shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
		//	shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
		//	shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].position", glm::value_ptr(lampPosition));
		//	shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].constant", 1.0);
		//	shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].linear", 0.09);
		//	shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].quadratic", 0.02);
		//	/*shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
		//	shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
		//	shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
		//	shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].position", glm::value_ptr(lampPosition));
		//	shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].constant", 1.0);
		//	shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].linear", 0.09);
		//	shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].quadratic", 0.02);*/
		//}

		/************Render de imagen de frente**************/
		if (!iniciaPartida) {
			shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
			shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureActivaID);
			shaderTexture.setInt("outTexture", 0);
			boxIntro.render();
			glfwSwapBuffers(window);
			continue;
		}

		// Render del buffer de profundidad desde el punto de vista de la luz
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Render
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		prepareDepthScene();
		renderScene();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//Para debug
		//glViewport(0, 0, screenWidth, screenHeight);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//// Render solo debug
		//shaderViewDepth.setMatrix4("projection", 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
		//shaderViewDepth.setMatrix4("view", 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
		//shaderViewDepth.setFloat("near_plane", near_plane);
		//shaderViewDepth.setFloat("far_plane", far_plane);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, depthMap);
		//boxViewDepth.setScale(glm::vec3(2.0, 2.0, 1.0));
		//boxViewDepth.render();

		// render normal de los objetos
		glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		prepareScene();
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		shaderMulLighting.setInt("shadowMap", 10);
		//shaderTerrain.setInt("shadowMap", 10);
		renderSolidScene();

		/*******************************************
		 * Creacion de colliders
		 * IMPORTANT do this before interpolations
		 *******************************************/
		

		AbstractModel::OBB dishesTableCollider;
		glm::mat4 modelMatrixColliderDishesTable = glm::mat4(modelMatrixDishesTable);
		dishesTableCollider.u = glm::quat_cast(modelMatrixDishesTable);
		modelMatrixColliderDishesTable = glm::scale(modelMatrixColliderDishesTable, glm::vec3(0.5,0.02,0.47));
		modelMatrixColliderDishesTable = glm::translate(modelMatrixColliderDishesTable, glm::vec3(15.0, 140.0, -1.0));
		dishesTableCollider.c = glm::vec3(modelMatrixColliderDishesTable[3]);
		dishesTableCollider.e = modelDishesTable.getObb().e * glm::vec3(0.5,0.02,0.47);
		addOrUpdateColliders(collidersOBB, "dishes", dishesTableCollider, modelMatrixDishesTable);

		AbstractModel::OBB choppingTableCollider;
		glm::mat4 modelMatrixColliderChoppingTable = glm::mat4(modelMatrixChoppingTable);
		choppingTableCollider.u = glm::quat_cast(modelMatrixChoppingTable);
		modelMatrixColliderChoppingTable = glm::scale(modelMatrixColliderChoppingTable, glm::vec3(0.4, 0.02, 0.47));
		modelMatrixColliderChoppingTable = glm::translate(modelMatrixColliderChoppingTable, glm::vec3(0.0, 135.0, -30.0));
		choppingTableCollider.c = glm::vec3(modelMatrixColliderChoppingTable[3]);
		choppingTableCollider.e = modelChoppingTable.getObb().e * glm::vec3(0.4, 0.02, 0.47);
		addOrUpdateColliders(collidersOBB, "chopping", choppingTableCollider, modelMatrixChoppingTable);

		AbstractModel::OBB burnerTableCollider;
		glm::mat4 modelMatrixColliderBurnerTable = glm::mat4(modelMatrixBurnersTable);
		burnerTableCollider.u = glm::quat_cast(modelMatrixBurnersTable);
		modelMatrixColliderBurnerTable = glm::scale(modelMatrixColliderBurnerTable, glm::vec3(0.4, 0.02, 0.47));
		modelMatrixColliderBurnerTable = glm::translate(modelMatrixColliderBurnerTable, glm::vec3(0.0, 170.0, 28.0));
		burnerTableCollider.c = glm::vec3(modelMatrixColliderBurnerTable[3]);
		burnerTableCollider.e = modelBurnersTable.getObb().e * glm::vec3(0.4, 0.02, 0.47);
		addOrUpdateColliders(collidersOBB, "burner", burnerTableCollider, modelMatrixBurnersTable);

		AbstractModel::OBB ingredientsTableCollider;
		glm::mat4 modelMatrixColliderIngredientsTable = glm::mat4(modelMatrixIngredientsTable);
		ingredientsTableCollider.u = glm::quat_cast(modelMatrixIngredientsTable);
		modelMatrixColliderIngredientsTable = glm::scale(modelMatrixColliderIngredientsTable, glm::vec3(0.5, 0.02, 0.48));
		modelMatrixColliderIngredientsTable = glm::translate(modelMatrixColliderIngredientsTable, glm::vec3(-14.2, 142.0, -1.0));
		ingredientsTableCollider.c = glm::vec3(modelMatrixColliderIngredientsTable[3]);
		ingredientsTableCollider.e = modelIngredientsTable.getObb().e * glm::vec3(0.5, 0.02, 0.48);
		addOrUpdateColliders(collidersOBB, "ingredients", ingredientsTableCollider, modelMatrixIngredientsTable);


		// Lamps1 colliders
		//for (int i = 0; i < lamp1Position.size(); i++) {
		//	AbstractModel::OBB lampCollider;
		//	glm::mat4 modelMatrixColliderLamp = glm::mat4(1.0);
		//	modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, lamp1Position[i]);
		//	modelMatrixColliderLamp = glm::rotate(modelMatrixColliderLamp, glm::radians(lamp1Orientation[i]),
		//		glm::vec3(0, 1, 0));
		//	addOrUpdateColliders(collidersOBB, "lamp1-" + std::to_string(i), lampCollider, modelMatrixColliderLamp);
		//	// Set the orientation of collider before doing the scale
		//	lampCollider.u = glm::quat_cast(modelMatrixColliderLamp);
		//	modelMatrixColliderLamp = glm::scale(modelMatrixColliderLamp, glm::vec3(0.5, 0.5, 0.5));
		//	modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, modelLamp1.getObb().c);
		//	lampCollider.c = glm::vec3(modelMatrixColliderLamp[3]);
		//	lampCollider.e = modelLamp1.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		//	std::get<0>(collidersOBB.find("lamp1-" + std::to_string(i))->second) = lampCollider;
		//}

		//// Lamps2 colliders
		//for (int i = 0; i < lamp2Position.size(); i++) {
		//	AbstractModel::OBB lampCollider;
		//	glm::mat4 modelMatrixColliderLamp = glm::mat4(1.0);
		//	modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, lamp2Position[i]);
		//	modelMatrixColliderLamp = glm::rotate(modelMatrixColliderLamp, glm::radians(lamp2Orientation[i]),
		//		glm::vec3(0, 1, 0));
		//	addOrUpdateColliders(collidersOBB, "lamp2-" + std::to_string(i), lampCollider, modelMatrixColliderLamp);
		//	// Set the orientation of collider before doing the scale
		//	lampCollider.u = glm::quat_cast(modelMatrixColliderLamp);
		//	modelMatrixColliderLamp = glm::scale(modelMatrixColliderLamp, glm::vec3(1.0, 1.0, 1.0));
		//	modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, modelLampPost2.getObb().c);
		//	lampCollider.c = glm::vec3(modelMatrixColliderLamp[3]);
		//	lampCollider.e = modelLampPost2.getObb().e * glm::vec3(1.0, 1.0, 1.0);
		//	std::get<0>(collidersOBB.find("lamp2-" + std::to_string(i))->second) = lampCollider;
		//}


		// Collider de Chef
		AbstractModel::OBB chefCollider;
		glm::mat4 modelmatrixColliderChef = glm::mat4(modelMatrixChef);
		modelmatrixColliderChef = glm::rotate(modelmatrixColliderChef, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
			chefCollider.u = glm::quat_cast(modelmatrixColliderChef);
		modelmatrixColliderChef = glm::scale(modelmatrixColliderChef, glm::vec3(0.3, 0.1, 0.05));
		modelmatrixColliderChef = glm::translate(modelmatrixColliderChef, glm::vec3(0.0, 0.0, 19.0));
		chefCollider.e = chefModelAnimate.getObb().e * glm::vec3(0.3, 0.1, 0.05) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
		chefCollider.c = glm::vec3(modelmatrixColliderChef[3]);
		addOrUpdateColliders(collidersOBB, "chef", chefCollider, modelMatrixChef);

		/*******************************************
		 * Render de colliders
		 *******************************************/
		for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
			collidersOBB.begin(); it != collidersOBB.end(); it++) {
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
			matrixCollider = matrixCollider * glm::mat4(std::get<0>(it->second).u);
			matrixCollider = glm::scale(matrixCollider, std::get<0>(it->second).e * 2.0f);
			boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 0.0));
			boxCollider.enableWireMode();
			boxCollider.render(matrixCollider);
		}

		for (std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
			collidersSBB.begin(); it != collidersSBB.end(); it++) {
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
			matrixCollider = glm::scale(matrixCollider, glm::vec3(std::get<0>(it->second).ratio * 2.0f));
			sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 0.0));
			sphereCollider.enableWireMode();
			sphereCollider.render(matrixCollider);
		}

		/*********************Prueba de colisiones****************************/
		for (std::map<std::string,
			std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator it =
			collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator jt =
				collidersSBB.begin(); jt != collidersSBB.end(); jt++) {
				if (it != jt && testSphereSphereIntersection(
					std::get<0>(it->second), std::get<0>(jt->second))) {
					std::cout << "Hay collision entre " << it->first <<
						" y el modelo " << jt->first << std::endl;
					isCollision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		for (std::map<std::string,
			std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator it =
			collidersOBB.begin(); it != collidersOBB.end(); it++) {
			bool isColision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator jt =
				collidersOBB.begin(); jt != collidersOBB.end(); jt++) {
				if (it != jt &&
					testOBBOBB(std::get<0>(it->second), std::get<0>(jt->second))) {
					std::cout << "Hay colision entre " << it->first << " y el modelo" <<
						jt->first << std::endl;
					isColision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isColision);
		}

		for (std::map<std::string,
			std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator it =
			collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator jt =
				collidersOBB.begin(); jt != collidersOBB.end(); jt++) {
				if (testSphereOBox(std::get<0>(it->second), std::get<0>(jt->second))) {
					std::cout << "Hay colision del " << it->first << " y el modelo" <<
						jt->first << std::endl;
					isCollision = true;
					addOrUpdateCollisionDetection(collisionDetection, jt->first, true);
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		std::map<std::string, bool>::iterator itCollision;
		for (itCollision = collisionDetection.begin();
			itCollision != collisionDetection.end(); itCollision++) {
			std::map<std::string, std::tuple<AbstractModel::SBB,
				glm::mat4, glm::mat4>>::iterator sbbBuscado =
				collidersSBB.find(itCollision->first);
			std::map<std::string, std::tuple<AbstractModel::OBB,
				glm::mat4, glm::mat4>>::iterator obbBuscado =
				collidersOBB.find(itCollision->first);
			if (sbbBuscado != collidersSBB.end()) {
				if (!itCollision->second)
					addOrUpdateColliders(collidersSBB, itCollision->first);
			}
			if (obbBuscado != collidersOBB.end()) {
				if (!itCollision->second)
					addOrUpdateColliders(collidersOBB, itCollision->first);
				else {
					/*if (itCollision->first.compare("mayow") == 0)
						modelMatrixMayow = std::get<1>(obbBuscado->second);*/
					if (itCollision->first.compare("chef") == 0)
						modelMatrixChef = std::get<1>(obbBuscado->second);
					/*if (itCollision->first.compare("dart") == 0)
						modelMatrixDart = std::get<1>(obbBuscado->second);*/
				}
			}
		}

		

		// Esto es para ilustrar la transformacion inversa de los coliders
		/*glm::vec3 cinv = glm::inverse(mayowCollider.u) * glm::vec4(rockCollider.c, 1.0);
		glm::mat4 invColliderS = glm::mat4(1.0);
		invColliderS = glm::translate(invColliderS, cinv);
		invColliderS =  invColliderS * glm::mat4(mayowCollider.u);
		invColliderS = glm::scale(invColliderS, glm::vec3(rockCollider.ratio * 2.0, rockCollider.ratio * 2.0, rockCollider.ratio * 2.0));
		sphereCollider.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));
		sphereCollider.enableWireMode();
		sphereCollider.render(invColliderS);
		glm::vec3 cinv2 = glm::inverse(mayowCollider.u) * glm::vec4(mayowCollider.c, 1.0);
		glm::mat4 invColliderB = glm::mat4(1.0);
		invColliderB = glm::translate(invColliderB, cinv2);
		invColliderB = glm::scale(invColliderB, mayowCollider.e * 2.0f);
		boxCollider.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));
		boxCollider.enableWireMode();
		boxCollider.render(invColliderB);
		// Se regresa el color blanco
		sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
		boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));*/

		// Animaciones por keyframes dart Vader
		// Para salvar los keyframes
		if (record && modelSelected == 1) {
			matrixDartJoints.push_back(rotDartHead);
			matrixDartJoints.push_back(rotDartLeftArm);
			matrixDartJoints.push_back(rotDartLeftHand);
			matrixDartJoints.push_back(rotDartRightArm);
			matrixDartJoints.push_back(rotDartRightHand);
			matrixDartJoints.push_back(rotDartLeftLeg);
			matrixDartJoints.push_back(rotDartRightLeg);
			if (saveFrame) {
				saveFrame = false;
				appendFrame(myfile, matrixDartJoints);
			}
		}
		else if (keyFramesDartJoints.size() > 0) {
			// Para reproducir el frame
			interpolationDartJoints = numPasosDartJoints / (float)maxNumPasosDartJoints;
			numPasosDartJoints++;
			if (interpolationDartJoints > 1.0) {
				interpolationDartJoints = 0;
				numPasosDartJoints = 0;
				indexFrameDartJoints = indexFrameDartJointsNext;
				indexFrameDartJointsNext++;
			}
			if (indexFrameDartJointsNext > keyFramesDartJoints.size() - 1)
				indexFrameDartJointsNext = 0;
			rotDartHead = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 0, interpolationDartJoints);
			rotDartLeftArm = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 1, interpolationDartJoints);
			rotDartLeftHand = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 2, interpolationDartJoints);
			rotDartRightArm = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 3, interpolationDartJoints);
			rotDartRightHand = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 4, interpolationDartJoints);
			rotDartLeftLeg = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 5, interpolationDartJoints);
			rotDartRightLeg = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 6, interpolationDartJoints);
		}
		

		// Constantes de animaciones
		rotHelHelY += 0.5;
		rotHelHelBack += 0.5;
		//animationMayowIndex = 1;
		animationChefIndex = 1;
		animationClienteIndex = 1;

		glfwSwapBuffers(window);

		/****************************+
		 * Open AL sound data
		 */
		source0Pos[0] = modelMatrixChoppingTable[3].x;
		source0Pos[1] = modelMatrixChoppingTable[3].y;
		source0Pos[2] = modelMatrixChoppingTable[3].z;
		alSourcefv(source[0], AL_POSITION, source0Pos);

		source1Pos[0] = modelMatrixBurnersTable[3].x;
		source1Pos[1] = modelMatrixBurnersTable[3].y;
		source1Pos[2] = modelMatrixBurnersTable[3].z;
		alSourcefv(source[1], AL_POSITION, source1Pos);

		source2Pos[0] = modelMatrixCliente[3].x;
		source2Pos[1] = modelMatrixCliente[3].y;
		source2Pos[2] = modelMatrixCliente[3].z;
		alSourcefv(source[2], AL_POSITION, source2Pos);

		// Listener for the Thris person camera
		listenerPos[0] = modelMatrixChef[3].x;
		listenerPos[1] = modelMatrixChef[3].y;
		listenerPos[2] = modelMatrixChef[3].z;
		alListenerfv(AL_POSITION, listenerPos);

		glm::vec3 upModel = glm::normalize(modelMatrixChef[1]);
		glm::vec3 frontModel = glm::normalize(modelMatrixChef[2]);

		listenerOri[0] = frontModel.x;
		listenerOri[1] = frontModel.y;
		listenerOri[2] = frontModel.z;
		listenerOri[3] = upModel.x;
		listenerOri[4] = upModel.y;
		listenerOri[5] = upModel.z;

		// Listener for the First person camera
		// listenerPos[0] = camera->getPosition().x;
		// listenerPos[1] = camera->getPosition().y;
		// listenerPos[2] = camera->getPosition().z;
		// alListenerfv(AL_POSITION, listenerPos);
		// listenerOri[0] = camera->getFront().x;
		// listenerOri[1] = camera->getFront().y;
		// listenerOri[2] = camera->getFront().z;
		// listenerOri[3] = camera->getUp().x;
		// listenerOri[4] = camera->getUp().y;
		// listenerOri[5] = camera->getUp().z;
		alListenerfv(AL_ORIENTATION, listenerOri);

		for (unsigned int i = 0; i < sourcesPlay.size(); i++) {
			if (sourcesPlay[i]) {
				sourcesPlay[i] = false;
				alSourcePlay(source[i]);
			}
		}
	}
}

int main(int argc, char** argv) {
	init(1100, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
