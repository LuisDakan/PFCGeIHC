
/*
Pr�ctica 7: Iluminaci�n 1 
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <map>
#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>
#include "animations.cpp"
#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"
#include "TorchSystem.h"
#include <algorithm>

//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

const float toRadians = 3.14159265f / 180.0f;

// Declaraciones globales para comunicación de eventos
bool g_addModelRequest = false;
glm::vec3 g_pendingModelPos = glm::vec3(0.0f, 0.0f, 0.0f);
bool g_removeModelRequest = false;
glm::vec3 g_removeModelPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::mat4 projection = glm::mat4(1.0f);

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
Model piso;
Model ak;
Model currentModel;
Model barco;
Model tori;
Model torchModel;
Model palmera_doble;
// Variables globales para comunicación de eventos
// (Eliminadas duplicadas)
Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


std::vector<glm::vec3> coords={
	{7.64,0.00,-9.30},
	{9.07,0.00,-10.94},
	{10.73,0.00,-12.89},
	{8.37,0.00,-8.88},
	{9.73,0.00,-10.55},
	{11.22,0.00,-12.35},
	    {1.52,0.00,3.64},
    {-1.53,0.00,3.66},
    {-3.57,0.00,1.52},
    {-3.59,0.00,-1.67},
    {3.84,0.00,1.62},
    {3.80,0.00,-1.73},
    {1.58,0.00,-3.88},
    {-1.58,0.00,-3.99},
    {0.00,0.00,0.00},
    {5.94,0.00,-3.86},
    {4.00,0.00,-6.57},
    {5.67,0.00,-9.62},
    {8.76,0.00,-6.62},
    {-6.10,-0.00,3.47},
    {-3.52,0.00,6.65},
    {-6.39,0.00,9.21},
    {-8.47,0.00,6.76},
    {-6.03,0.00,-3.93},
    {-8.23,0.00,-7.06},
    {-6.00,0.00,-9.88},
    {-3.48,0.00,-6.86},
    {-8.30,0.00,4.81},
    {-9.62,0.00,3.12},
    {-10.88,0.00,1.43},
    {-7.57,0.00,4.00},
    {-8.85,0.00,2.33},
    {-10.24,0.00,0.78},
    {10.49,0.00,12.89},
    {9.38,0.00,11.31},
    {8.00,0.00,9.69},
    {8.79,-0.00,9.13},
    {10.05,0.00,10.57},
    {11.33,0.00,12.19},
	    {-4.35,0.00,9.46},
    {-3.12,0.00,10.83},
    {-1.77,0.00,12.37},
    {-1.09,0.00,11.85},
    {-2.44,-0.00,10.20},
    {-3.72,0.00,8.57},
	    {-0.38,0.00,4.64},
    {0.42,0.00,4.62},
    {-0.39,-0.00,7.03},
    {0.42,0.00,7.14},
    {-0.34,0.00,9.05},
    {0.43,0.00,9.18},
    {-0.33,0.00,11.05},
    {0.40,0.00,11.04},
    {0.89,0.00,12.22},
    {1.43,0.00,12.76},
    {2.22,0.00,11.97},
    {1.68,0.00,11.27},
    {3.19,0.00,10.74},
    {2.68,0.00,10.12},
    {4.21,0.00,9.48},
    {3.69,0.00,8.88},
    {-3.96,-0.00,3.46},
    {-3.23,0.00,4.10},
    {-10.25,0.00,-0.66},
    {-7.77,0.00,0.70},
    {-4.96,0.00,0.74},
    {-7.85,0.00,-0.79},
    {-5.01,0.00,-0.84},
    {-8.73,0.00,-2.55},
    {-7.26,0.00,-4.28},
    {-8.17,0.00,-4.97},
    {-9.52,0.00,-3.29},
    {-11.06,0.00,-1.39},
    {-8.31,0.00,-8.75},
    {-10.31,0.00,-11.14},
    {-7.45,0.00,-9.32},
    {-9.56,0.00,-11.84},
    {-3.61,0.00,-8.75},
    {-4.36,0.00,-9.51},
    {-3.48,0.00,-10.47},
    {-2.56,0.00,-9.69},
    {-0.82,0.00,-11.96},
    {-1.83,0.00,-12.53},
    {0.52,0.00,-11.83},
    {-0.39,0.00,-9.26},
    {0.44,0.00,-9.15},
    {-0.34,0.00,-6.20},
    {0.45,0.00,-6.02},
    {1.67,0.00,-11.27},
    {2.28,0.00,-11.79},
    {3.10,-0.00,-9.61},
    {3.71,0.00,-10.03},
    {7.77,0.00,-3.95},
    {8.41,0.00,-4.38},
    {8.96,0.00,-2.52},
    {9.55,0.00,-3.07},
    {10.91,0.00,-1.38},
    {10.38,0.00,-0.86},
    {9.35,0.00,-0.66},
    {9.35,0.00,0.65},
    {7.26,0.00,-0.77},
    {7.21,0.00,0.55},
    {4.03,0.00,-0.65},
    {4.13,0.00,0.54},
    {9.83,0.00,1.53},
    {10.43,0.00,2.15},
    {8.26,0.00,3.34},
    {8.80,0.00,3.97},
    {3.43,0.00,2.78},
    {4.51,0.00,4.04},
    {2.58,0.00,3.33},
    {3.75,0.00,4.67},
    {-7.75,0.00,9.60},
    {-8.41,0.00,8.89},
    {-9.11,0.00,10.89},
    {-9.72,0.00,10.42},
    {-10.57,0.00,12.84},
    {-11.20,-0.00,12.20},
};

//matriz con posiciones y escalas de palmeras (x,y,z,s1,s2,s3)
//cada vector interno representa una palmera
std::vector<std::vector<GLfloat>> coordsPalm= {
	{981.38,0.00,-53.59,20.00,20.00,20.00},
	{1125.47,0.00,-306.81,20.00,20.00,20.00},
	{495.39,0.00,-154.64,20.00,20.00,20.00},
	{801.01,0.00,-666.71,23.10,23.10,23.10},
	{218.78,0.00,-523.34,17.60,17.60,17.60},
	{- 415.46,0.00,739.16,19.20,19.20,19.20 },
	{-677.91,0.00,-238.19,22.70,22.70,22.70}

};


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

std::vector<std::string> spots(MAX_SPOT_LIGHTS),points(MAX_POINT_LIGHTS);

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//funci�n de calculo de normales por promedio de v�rtices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}

Mesh* crearDado8() {
	GLfloat octaedro_vertices[] = {
		 0.0f,  1.0f,  0.0f,	0.125f,	0.99f,	0.0f,	0.0f,	0.0f,
		 0.0f,  0.0f,  1.0f,	0.01f,	0.76f,	0.0f,	0.0f,	0.0f,
		 1.0f,  0.0f,  0.0f,	0.24f,	0.76f,	0.0f,	0.0f,	0.0f,

		 0.0f,  1.0f,  0.0f,	0.375f,	0.99f,	0.0f,	0.0f,	0.0f,
		-1.0f,  0.0f,  0.0f,	0.26f,	0.76f,	0.0f,	0.0f,	0.0f,
		 0.0f,  0.0f,  1.0f,	0.49f,	0.76f,	0.0f,	0.0f,	0.0f,

		 0.0f,  1.0f,  0.0f,	0.625f,	0.99f,	0.0f,	0.0f,	0.0f,
		 0.0f,  0.0f, -1.0f,	0.51f,	0.76f,	0.0f,	0.0f,	0.0f,
		-1.0f,  0.0f,  0.0f,	0.74f,	0.76f,	0.0f,	0.0f,	0.0f,

		 0.0f,  1.0f,  0.0f,	0.875f,	0.99f,	0.0f,	0.0f,	0.0f,
		 1.0f,  0.0f,  0.0f,	0.76f,	0.76f,	0.0f,	0.0f,	0.0f,
		 0.0f,  0.0f, -1.0f,	0.99f,	0.76f,	0.0f,	0.0f,	0.0f,

		 0.0f, -1.0f,  0.0f,	0.125f,	0.51f,	0.0f,	0.0f,	0.0f,
		 1.0f,  0.0f,  0.0f,	0.24f,	0.74f,	0.0f,	0.0f,	0.0f,
		 0.0f,  0.0f,  1.0f,	0.01f,	0.74f,	0.0f,	0.0f,	0.0f,

		 0.0f, -1.0f,  0.0f,	0.125f,	0.26f,	0.0f,	0.0f,	0.0f,
		 0.0f,  0.0f,  1.0f,	0.24f,	0.49f,	0.0f,	0.0f,	0.0f,
		-1.0f,  0.0f,  0.0f,	0.01f,	0.49f,	0.0f,	0.0f,	0.0f,

		 0.0f, -1.0f,  0.0f,	0.125f,	0.01f,	0.0f,	0.0f,	0.0f,
		-1.0f,  0.0f,  0.0f,	0.24f,	0.24f,	0.0f,	0.0f,	0.0f,
		 0.0f,  0.0f, -1.0f,	0.01f,	0.24f,	0.0f,	0.0f,	0.0f,

		 0.0f, -1.0f,  0.0f,	0.375f,	0.01f,	0.0f,	0.0f,	0.0f,
		 0.0f,  0.0f, -1.0f,	0.49f,	0.24f,	0.0f,	0.0f,	0.0f,
		 1.0f,  0.0f,  0.0f,	0.26f,	0.24f,	0.0f,	0.0f,	0.0f
	};
	
	unsigned int octaedro_indices[] = {
		2, 1, 0,
		5, 4, 3,
		8, 7, 6,
		11, 10, 9,
		12, 14, 13,
		15, 17, 16,
		18, 20, 19,
		21, 23, 22
	};
	
	calcAverageNormals(octaedro_indices, 24, octaedro_vertices, 192, 8, 5);
	
	Mesh* dado8 = new Mesh();
	dado8->CreateMesh(octaedro_vertices, octaedro_indices, 192, 24);
	return dado8;
}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int searchPoint(std::string id) {
	for (int i = 0;i < MAX_POINT_LIGHTS;i++) {
		if (points[i] == id) return i;
	}
	printf("NO SE ENCONTRO LA LUZ PointLight\n");
	return -1;
}

void turnOffPoint(std::string id, unsigned int& pointLightCount)
{
	if (pointLightCount <= 0) {
		return;
	}
	int idx=-1;
	for(int i=0;i<pointLightCount;i++){
		if(points[i]==id){
			idx=i;
			break;
		}
	}
	if (idx == -1) return;
	std::swap(pointLights[idx], pointLights[--pointLightCount]);
	std::swap(points[idx], points[pointLightCount]);

}

void turnOnPoint(std::string id, unsigned int& pointLightCount) {
	if (pointLightCount >= MAX_POINT_LIGHTS) {
		return;
	}
	int idx =-1;
	for(int i=pointLightCount;i<MAX_POINT_LIGHTS;i++){
		if(points[i]==id){
			idx=i;
			break;
		}
	}
	if (idx == -1) return;
	std::swap(pointLights[idx], pointLights[pointLightCount]);
	std::swap(points[idx], points[pointLightCount]);
	pointLightCount++;
}

int searchSpot(std::string id) {
	for (int i = 0;i < MAX_SPOT_LIGHTS;i++) {
		if (spots[i] == id) return i;
	}
	printf("NO SE ENCONTRO LA LUZ SpotLight\n");
	return -1;
}

void turnOffSpot(std::string id, unsigned int& spotLightCount) {
    if (spotLightCount <= 0) {
		return;
	}
	int idx=-1;
	for(int i=0;i<spotLightCount;i++){
		if(spots[i]==id){
			idx=i;
			break;
		}
	}
	if (idx == -1) return;
	std::swap(spotLights[idx], spotLights[--spotLightCount]);
	std::swap(spots[idx], spots[spotLightCount]);
}

void turnOnSpot(std::string id, unsigned int& spotLightCount) {
    if (spotLightCount >= MAX_SPOT_LIGHTS) {
		return;
	}
	int idx =-1;
	for(int i=spotLightCount;i<MAX_SPOT_LIGHTS;i++){
		if(spots[i]==id){
			idx=i;
			break;
		}
	}
	if (idx == -1) return;
	std::swap(spotLights[idx], spotLights[spotLightCount]);
	std::swap(spots[idx], spots[spotLightCount]);
	spotLightCount++;
}


int main()
	
{
	
	// No lambdas: usaremos Window::ManejaMouseClick y Window::ManejaTeclado
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	// Configurar modelo actual a colocar
	SetCurrentModel("Models/Tori.obj");
	currentModel = Model();
	currentModel.LoadModel(currentModelPath.c_str());
	barco = Model();
	barco.LoadModel("Models/Barco.obj");
	piso=Model();
	piso.LoadModel("Models/piso.obj");
	tori = Model();
	tori.LoadModel("Models/Tori.obj");
	torchModel = Model();
	torchModel.LoadModel("Models/Antorcha_Ace_Attorney.obj");
	palmera_doble = Model();
	palmera_doble.LoadModel("Models/PalmeraDoble.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/dia_despejado.jpg");
	skyboxFaces.push_back("Textures/Skybox/dia_despejado.jpg");
	skyboxFaces.push_back("Textures/Skybox/dia_despejado.jpg");
	skyboxFaces.push_back("Textures/Skybox/dia_despejado.jpg");
	skyboxFaces.push_back("Textures/Skybox/dia_despejado.jpg");
	skyboxFaces.push_back("Textures/Skybox/dia_despejado.jpg");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Lampara
	pointLights[0] = PointLight(1.0f, 1.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.1f, 0.01f, 0.01f);
	points[pointLightCount] = "antorcha";
	pointLightCount++;
	//Laser
	pointLights[1] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.3f, 0.02f, 0.02f
	);
	points[pointLightCount] = "laser";
	//pointLightCount++;
	
	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spots[spotLightCount]="linterna";
	spotLightCount++;
	
	//luz cofre
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.05f, 0.05f,
		15.0f);
	spots[spotLightCount] = "linternaCofre";
	spotLightCount++;

	//faro delantero
	spotLights[2] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		0.4f, 0.01f, 0.01f,
		15.0f);
	spots[spotLightCount] = "faroDelantero";
	spotLightCount++;
	

	//faro trasero
	spotLights[3] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.4f, 0.01f, 0.01f,
		15.0f);
	spots[spotLightCount] = "faroTrasero";
	spotLightCount++;


	int idx;
	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::vec3 p;
	glm::vec4 rot;
		projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose()){
		// Procesar solicitudes de agregar/quitar modelos
		if (g_addModelRequest) {
			AddModelInstance(g_pendingModelPos);
			g_addModelRequest = false;
		}
		if (g_removeModelRequest) {
			RemoveModelInstance(g_removeModelPos);
			g_removeModelRequest = false;
		}

		
	
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glm::mat4 view = camera.calculateViewMatrix();
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la c�mara de tipo flash
		//sirve para que en tiempo de ejecuci�n (dentro del while) se cambien propiedades de la luz
			glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//informaci�n al shader de fuentes de iluminaci�n
		shaderList[0].SetDirectionalLight(&mainLight);

		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		//Piso 

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		piso.RenderModel();

		for (std::vector <GLfloat> v : coordsPalm) {
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(v[0], v[1]+7.0, v[2]));
			model = glm::scale(model, glm::vec3(v[3], v[4], v[5]));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			//palmera_doble.RenderModel();

		}

		for (const auto& coor : coords) 
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(coor[0]*2,coor[1],coor[2]*1.5)*1.5f);
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			//torchModel.RenderModel();
		}

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-694.59f,0.00,-418.33f));
		model = AnimationShip(model);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		barco.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1142.11, 0.00, 1.83));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tori.RenderModel();

		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	// Exportar coordenadas de modelos a archivo txt
	ExportModelPositions("model_positions.txt");

	return 0;
}
