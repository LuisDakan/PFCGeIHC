
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

// Variable para contador de rounds (0 a 14)
int roundCounter = 0,firstDigit,secondDigit;

// Variable para rotación de texturas de máscaras (0-3)
int maskRotation = 0;

Texture numeros,mascaras;
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
Model palmera_tres;
Model arbol_seis;
Model arbusto_grande;
Model arbusto_largo;
Model arbol_tronco;
Model torchAce, torchCrash, torchSonic;
Model ring;
// Variables globales para comunicación de eventos
// (Eliminadas duplicadas)
Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;

//posiciones de antorchas
std::vector<std::vector<float>> coordTorch = {
	{110.53, 0.00, 189.66},
	{-170.60, 0.00, 186.27},
	{-403.51, 0.00, 80.53},
	{-425.57, 0.00, 161.16},
	{-390.37, 0.00, 218.64},
	{-682.14, 0.00, 190.84},
	{-463.45, 0.00, 322.64},
	{-679.58, -0.00, 497.43},
	{-982.96, 0.00, 318.62},
	{-1021.20, 0.00, 482.89},
	{-998.45, 0.00, 536.70},
	{-940.59, 0.00, 227.71},
	{-912.70, 0.00, 173.24},
	{-1028.78, 0.00, 117.73},
	{-1056.01, 0.00, 164.21},
	{-1171.08, 0.00, 27.96},
	{-1171.70, 0.00, -31.83},
	{-1018.60, 0.00, 36.61},
	{-417.33, 0.00, -87.05},
	{-440.10, 0.00, -163.30},
	{-397.27, 0.00, -230.64},
	{-1040.92, 0.00, -485.72},
	{-999.45, 0.00, -540.02},
	{-929.10, 0.00, -349.26},
	{-714.32, 0.00, -461.43},
	{-713.58, 0.00, -195.47},
	{-418.54, 0.00, -353.95},
	{-396.25, 0.00, -454.34},
	{-420.96, 0.00, -512.78},
	{-210.54, 0.00, -551.62},
	{-240.86, 0.00, -613.61},
	{-158.38, 0.00, -194.14},
	{161.06, 0.00, -200.07},
	{413.01, 0.00, -336.59},
	{426.64, 0.00, -433.54},
	{465.18, 0.00, -482.65},
	{211.61, 0.00, -549.73},
	{240.17, 0.00, -599.78},
	{668.71, 0.00, -474.49},
	{927.73, -0.00, -334.00},
	{542.86, 0.00, -200.93},
	{711.57, 0.00, -179.81},
	{897.09, 0.00, -488.01},
	{941.59, 0.00, -444.43},
	{1175.34, 0.00, -570.88},
	{1135.66, 0.00, -615.76},
	{907.55, 0.00, -239.31},
	{879.97, 0.00, -194.78},
	{987.86, 0.00, -138.41},
	{1010.80, 0.00, -182.92},
	{1170.75, 0.00, 33.67},
	{1175.82, 0.00, -32.86},
	{888.69, 0.00, -33.35},
	{902.99, 0.00, 30.06},
	{719.20, 0.00, 29.94},
	{715.81, 0.00, -28.15},
	{573.43, 0.00, -28.31},
	{568.87, 0.00, 28.76},
	{396.10, 0.00, -78.83},
	{385.64, 0.00, 85.20},
	{416.51, 0.00, 172.02},
	{386.60, 0.00, 216.31},
	{-42.49, 0.00, 272.90},
	{41.21, 0.00, 270.99},
	{-45.36, 0.00, 556.17},
	{30.97, 0.00, 555.43},
	{-483.80, 0.00, 471.92},
	{-447.79, 0.00, 427.35},
	{-339.00, 0.00, 548.55},
	{-300.99, 0.00, 503.00},
	{261.81, 0.00, 593.04},
	{224.91, 0.00, 538.23},
	{448.41, -0.00, 489.54},
	{424.85, -0.00, 431.41},
	{954.87, 0.00, 518.85},
	{996.53, 0.00, 485.79},
	{1008.87, 0.00, 181.70},
	{988.65, 0.00, 137.30},
	{678.07, 0.00, 198.28},
	{409.99, 0.00, 334.87},
	{621.50, 0.00, 492.90},
	{822.05, 0.00, 450.69},
	{880.02, 0.00, 420.03},
	{395.58, 0.00, -220.54},
	{415.25, 0.00, -169.53},
	{39.69, 0.00, -218.90},
	{-45.83, 0.00, -230.14},
	{35.73, 0.00, -416.47},
	{-44.57, 0.00, -411.97},
	{-1012.90, 0.00, -194.52},
	{-999.22, -0.00, -141.55},
	{-897.80, 0.00, -27.26},
	{-905.90, 0.00, 32.49},
	{-619.28, 0.00, 37.07},
	{-618.94, 0.00, -29.59},
	{-452.40, 0.00, 26.76},
	{-451.58, 0.00, -27.17}
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

//matriz con posiciones de palmeras de tres troncos (x,y,z)
std::vector<std::vector<GLfloat>> coordsPalmtres = {
	{ -559.54,0.00,-653.03},
	{ -928.46,0.00,-435.60},
	{ -1040.85,0.00,-85.37},
	{1092.84,0.00,470.01},
	{494.46,0.00,748.56},
	{127.89,0.00,616.22},
	{533.59,0.00,192.92},
	{-197.24,0.00,482.37},
	{-1012.27,0.00,-309.12},
	{-118.02,0.00,-700.87},
	{-198.34,0.00,-326.08}

};


std::vector<std::vector<GLfloat>> coordsTreesix = {
	{964.97,0.00,-485.81},
	{444.38,0.00,-758.24},
	{ -952.37,0.00,99.83},
	{ -471.97,0.00,119.55},
	{ -1089.71,0.00,609.70},
	{ -846.79,0.00,672.76},
	{ -388.39,0.00,494.02},
	{252.41,0.00,-248.32},
	{433.80,0.00,-332.91},
	{761.71,0.00,-216.86},
	{ 99.81,0.00,-726.70 },
	{-60.19,0.00,-510.63},
	{-703.17,0.00,244.07}

};
std::vector<std::vector<GLfloat>> coordsBushlarge= {
	{ -1078.04,0.00,-785.81,1.00,1.00,1.00},
{-343.41,0.00,-764.77,1.00,1.00,1.00},
{-702.20,0.00,-637.29,1.00,1.00,1.00},
{-1081.57,0.00,-639.51,1.00,1.00,1.00},
{-1109.23,0.00,-300.75,1.00,1.00,1.00},
{-816.10,0.00,97.18,1.00,1.00,1.00},
{-1075.79,-0.00,309.58,1.00,1.00,1.00},
{ -1162.39,0.00,544.71,1.00,1.00,1.00 },
{ -573.94,0.00,775.06,1.00,1.00,1.00 },
{ -905.24,0.00,776.70,1.00,1.00,1.00 },
{ -182.01,0.00,284.04,1.00,1.00,1.00 },
{ -82.91,0.00,768.35,1.00,1.00,1.00 },
{ -560.38,0.00,224.83,1.00,1.00,1.00 },
{ 1182.31,0.00,161.66,1.00,1.00,1.00 },
{ 977.09,0.00,328.17,1.00,1.00,1.00 },
{ 1105.20,0.00,703.26,1.00,1.00,1.00 },
{ 1101.46,0.00,-590.96,1.00,1.00,1.00 },
{ 925.29,0.00,-141.23,1.00,1.00,1.00 },
{ 594.86,0.00,-82.81,1.00,1.00,1.00 },
{ -73.37,0.00,-323.11,1.00,1.00,1.00 },
{ 305.41,0.00,-636.02,1.00,1.00,1.00 },
{ 114.49,0.00,317.36,1.00,1.00,1.00 },

};

std::vector<std::vector<GLfloat>> coordsBushlong= {
	{ -317.86,0.00,-427.51},
{-218.40,0.00,-573.61},
{-262.53,0.00,-235.30},
{323.68,-0.00,403.69},
{111.71,0.00,492.16},
{269.91,0.00,664.26},
{693.59,0.00,797.74},
{840.47,0.00,723.05},
{1065.82,0.00,557.96},
{1172.86,0.00,350.22},
{1097.65,0.00,241.89},
{705.75,0.00,256.33},
{874.20,0.00,119.12,1.00,1.00,1.00},
{1005.71,0.00,-280.25,1.00,1.00,1.00},
{1139.65,0.00,-160.09,1.00,1.00,1.00},
{939.78,0.00,-406.51,1.00,1.00,1.00},
{694.75,0.00,-649.27,1.00,1.00,1.00},
{57.93,0.00,-626.86,1.00,1.00,1.00},
{-417.69,0.00,-92.67,1.00,1.00,1.00},
{-602.80,0.00,-56.39,1.00,1.00,1.00},
{-802.82,0.00,-126.97,1.00,1.00,1.00},
{-925.67,0.00,-68.77,1.00,1.00,1.00},
{-658.70,0.00,136.44,1.00,1.00,1.00},
{-861.86,0.00,226.13,1.00,1.00,1.00},
{-462.07,0.00,68.47,1.00,1.00,1.00},

};

std::vector<std::vector<GLfloat>> coordsTreetrunk = {
	{451.74,0.00,133.87},
{ 117.15,0.00,717.37 },
{ -715.52,0.00,680.17},
{ -1011.89,0.00,796.28 },
{ -1118.37,0.00,423.22},
{ -252.82,0.00,-510.33 },
{ 214.29,0.00,-391.25 },
{ 631.03,0.00,-182.21 },
{ 1066.55,0.00,-417.04 },
{ 651.85,-0.00,-754.06 },
{ 317.42,0.00,555.41 },
{ 221.68,0.00,447.60}, 
{ -233.05, 0.00, 626.71 },
{ -101.99, 0.00, 427.04}

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
glm::vec2 getUVNumber(int num);
void CreateMeshNumber();
void CreateRingWalls();
glm::vec2 getMaskUVOffset(int wallIndex);
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
	CreateMeshNumber();
	CreateRingWalls();
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	
	barco = Model();
	barco.LoadModel("Models/Barco.obj");
	piso=Model();
	piso.LoadModel("Models/piso.obj");
	tori = Model();
	tori.LoadModel("Models/Tori.obj");
	palmera_doble = Model();
	palmera_doble.LoadModel("Models/PalmeraDoble.obj");
	torchModel = Model();
	torchModel.LoadModel("Models/Antorcha_Ace_Attorney.obj");
	palmera_tres = Model();
	palmera_tres.LoadModel("Models/Palmera3.obj");
	arbol_seis = Model();
	arbol_seis.LoadModel("Models/Arbol6.obj");
	arbusto_grande = Model();
	arbusto_grande.LoadModel("Models/Arbusto_grande.obj");
	arbusto_largo = Model();
	arbusto_largo.LoadModel("Models/Arbusto_largo.obj");
	arbol_tronco = Model();
	arbol_tronco.LoadModel("Models/Arbol12.obj");
	torchAce = Model();
	torchAce.LoadModel("Models/Antorcha_Ace_Attorney.obj");
	torchCrash = Model();
	torchCrash.LoadModel("Models/antorcha_crash.obj");
	torchSonic = Model();
	torchSonic.LoadModel("Models/Antorcha_Sonic.obj");
	ring = Model();
	ring.LoadModel("Models/Boxing Ring.obj");

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
	mascaras = Texture("Textures/Masks.png"); mascaras.LoadTextureA();
	numeros= Texture("Textures/Numeros.png"); numeros.LoadTextureA();
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

	glm::vec2 offset;
	int idx,aux;
	//se crean mas luces puntuales y spotlight 
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::vec3 p;
	glm::vec4 rot;
		projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose()){

	
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
		uniformTextureOffset = shaderList[0].getOffsetLocation();
		
		//información en el shader de intensidad especular y brillo
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
		
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		//Piso 

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		piso.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-694.59f,0.00,-418.33f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		barco.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1142.11, 0.00, 1.83));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tori.RenderModel();

		//ciclo for para palmeras dobles
		for (std::vector <GLfloat> v : coordsPalm) {
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(v[0], v[1]+7.0, v[2]));
			model = glm::scale(model, glm::vec3(v[3], v[4], v[5]));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			palmera_doble.RenderModel();

		}

		//ciclo for para palmeratres
		for (std::vector <GLfloat> v : coordsPalmtres) {
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(v[0], v[1] + 7.0, v[2]));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			palmera_tres.RenderModel();

		}
		//ciclo for para arboles seis
		for (std::vector <GLfloat> v : coordsTreesix) {
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(v[0], v[1], v[2]));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			arbol_seis.RenderModel();

		}
		//ciclo for para arbustos grandes
		for (std::vector <GLfloat> v : coordsBushlarge) {
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(v[0], v[1], v[2]));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			arbusto_grande.RenderModel();
		
		}
		//ciclo for para arbustos largos
		for (std::vector <GLfloat> v : coordsBushlong) {
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(v[0], v[1], v[2]));
			//model = glm::scale(model, glm::vec3(v[3], v[4], v[5]));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			arbusto_largo.RenderModel();
		}
		
		//cliclo for para los troncos de los arboles
		for (std::vector <GLfloat> v : coordsTreetrunk) {
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(v[0], v[1], v[2]));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			arbol_tronco.RenderModel();
		}
		aux = 0;
		for (const auto& coor : coordTorch) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(coor[0], coor[1], coor[2]*2.0f/1.5f));
			//model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			switch (aux) {
			case 0:
				break;
			case 1:
				model = glm::translate(model, glm::vec3(0.0f, 5.0f, 0.0f));
				break;
			case 2:
				break;
			}
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			switch (aux) {
			case 0:
				torchAce.RenderModel();
				break;
			case 1:
				torchCrash.RenderModel();
				break;
			case 2:
				torchSonic.RenderModel();
				break;
			}
			aux = (aux + 1) % 3;

		}


		glDisable(GL_BLEND);







		


















































































		
		// Renderizar contador de rounds - DÍGITO DE DECENAS
		firstDigit = roundCounter / 10;
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.7f, 10.0f, 0.0f)); // Ajusta Y para visibilidad
		modelaux = model;
		model = glm::scale(model, glm::vec3(2.0f, 4.0f, 1.0f));
		offset = getUVNumber(firstDigit);
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(offset));
		numeros.UseTexture();
		meshList[4]->RenderMesh();

		// Renderizar contador de rounds - DÍGITO DE UNIDADES
		secondDigit = roundCounter % 10;
		model = modelaux;
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f)); // Separación entre dígitos
		model = glm::scale(model, glm::vec3(2.0f, 4.0f, 1.0f));
		offset = getUVNumber(secondDigit);
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(offset));
		numeros.UseTexture();
		meshList[4]->RenderMesh();

		// Renderizar paredes del ring con texturas de máscaras
		mascaras.UseTexture();
		
		// Pared Norte (meshList[5])
		model = glm::mat4(1.0);
		offset = getMaskUVOffset(0);  // Pared 0
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(offset));
		meshList[5]->RenderMesh();
		
		// Pared Este (meshList[6])
		model = glm::mat4(1.0);
		offset = getMaskUVOffset(1);  // Pared 1
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(offset));

		meshList[6]->RenderMesh();
		
		// Pared Sur (meshList[7])
		model = glm::mat4(1.0);
		offset = getMaskUVOffset(2);  // Pared 2
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(offset));
		meshList[7]->RenderMesh();
		
		// Pared Oeste (meshList[8])
		model = glm::mat4(1.0);
		offset = getMaskUVOffset(3);  // Pared 3
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(offset));
		meshList[8]->RenderMesh();

		offset = glm::vec2(0.0f,0.0f);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(offset));
		
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

void CreateMeshNumber()
{
		// Mesh para dígitos del contador (plano rectangular)
	unsigned int digitIndices[] = {
		0, 1, 2,
		0, 2, 3
	};

	GLfloat digitVertices[] = {
		//	x      y      z		u	  v			nx	  ny    nz
		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,		0.2f, 0.0f,		0.0f, 0.0f, 1.0f,
		 1.0f,  1.0f, 0.0f,		0.2f, 0.5f,		0.0f, 0.0f, 1.0f,
		-1.0f,  1.0f, 0.0f,		0.0f, 0.5f,		0.0f, 0.0f, 1.0f
	};

	Mesh *digitMesh = new Mesh();
	digitMesh->CreateMesh(digitVertices, digitIndices, 32, 6);
	meshList.push_back(digitMesh);

}

void CreateRingWalls()
{
	// Crear 4 paredes alrededor del ring (Norte, Este, Sur, Oeste)
	// Cada pared es un rectángulo vertical con UVs que apuntan a una fila diferente de la textura
	// La textura tiene 4 filas (cada una 0.25 de altura)
	
	float wallWidth = 50.0f;   // Ancho de cada pared
	float wallHeight = 15.0f;  // Altura de cada pared
	float ringRadius = 40.0f;  // Distancia desde el centro del ring
	
	// Índices para un rectángulo (2 triángulos)
	unsigned int wallIndices[] = {
		0, 1, 2,
		0, 2, 3
	};
	
	// PARED NORTE (frente, mirando al +Z)
	GLfloat wallNorthVertices[] = {
		//	x					y				z				u		v				nx		ny		nz
		-wallWidth/2.0f,	0.0f,			ringRadius,		0.0f,	0.0f,			0.0f,	0.0f,	-1.0f,
		 wallWidth/2.0f,	0.0f,			ringRadius,		1.0f,	0.0f,			0.0f,	0.0f,	-1.0f,
		 wallWidth/2.0f,	wallHeight,		ringRadius,		1.0f,	0.25f,			0.0f,	0.0f,	-1.0f,
		-wallWidth/2.0f,	wallHeight,		ringRadius,		0.0f,	0.25f,			0.0f,	0.0f,	-1.0f
	};
	
	// PARED ESTE (derecha, mirando al +X)
	GLfloat wallEastVertices[] = {
		ringRadius,			0.0f,			wallWidth/2.0f,		0.0f,	0.25f,		-1.0f,	0.0f,	0.0f,
		ringRadius,			0.0f,			-wallWidth/2.0f,	1.0f,	0.25f,		-1.0f,	0.0f,	0.0f,
		ringRadius,			wallHeight,		-wallWidth/2.0f,	1.0f,	0.5f,		-1.0f,	0.0f,	0.0f,
		ringRadius,			wallHeight,		wallWidth/2.0f,		0.0f,	0.5f,		-1.0f,	0.0f,	0.0f
	};
	
	// PARED SUR (atrás, mirando al -Z)
	GLfloat wallSouthVertices[] = {
		wallWidth/2.0f,		0.0f,			-ringRadius,	0.0f,	0.5f,		0.0f,	0.0f,	1.0f,
		-wallWidth/2.0f,	0.0f,			-ringRadius,	1.0f,	0.5f,		0.0f,	0.0f,	1.0f,
		-wallWidth/2.0f,	wallHeight,		-ringRadius,	1.0f,	0.75f,		0.0f,	0.0f,	1.0f,
		wallWidth/2.0f,		wallHeight,		-ringRadius,	0.0f,	0.75f,		0.0f,	0.0f,	1.0f
	};
	
	// PARED OESTE (izquierda, mirando al -X)
	GLfloat wallWestVertices[] = {
		-ringRadius,		0.0f,			-wallWidth/2.0f,	0.0f,	0.75f,		1.0f,	0.0f,	0.0f,
		-ringRadius,		0.0f,			wallWidth/2.0f,		1.0f,	0.75f,		1.0f,	0.0f,	0.0f,
		-ringRadius,		wallHeight,		wallWidth/2.0f,		1.0f,	1.0f,		1.0f,	0.0f,	0.0f,
		-ringRadius,		wallHeight,		-wallWidth/2.0f,	0.0f,	1.0f,		1.0f,	0.0f,	0.0f
	};
	
	// Crear meshes y agregarlos a la lista
	Mesh *wallNorth = new Mesh();
	wallNorth->CreateMesh(wallNorthVertices, wallIndices, 32, 6);
	meshList.push_back(wallNorth);  // índice 5
	
	Mesh *wallEast = new Mesh();
	wallEast->CreateMesh(wallEastVertices, wallIndices, 32, 6);
	meshList.push_back(wallEast);   // índice 6
	
	Mesh *wallSouth = new Mesh();
	wallSouth->CreateMesh(wallSouthVertices, wallIndices, 32, 6);
	meshList.push_back(wallSouth);  // índice 7
	
	Mesh *wallWest = new Mesh();
	wallWest->CreateMesh(wallWestVertices, wallIndices, 32, 6);
	meshList.push_back(wallWest);   // índice 8
}

void CreateRingMesh(){

}

glm::vec2 getUVNumber(int num){
	
	// Layout de textura: Fila superior [1][2][3][4][5], Fila inferior [6][7][8][9][0]
	
	if(num == 0){
		return glm::vec2(0.8f, 0.0f); // Dígito 0 en fila inferior, última columna
	}
	
	int fila = (num - 1) / 5;  // 1-5 = fila 0 (superior), 6-9 = fila 1 (inferior)
	int columna = (num - 1) % 5;
	
	float offsetx = 0.2f * (float)columna;
	float offsety = 0.5f - (0.5f * (float)fila); // 0.5 para superior, 0.0 para inferior
	
	return glm::vec2(offsetx, offsety);
}

// Función para calcular el offset UV de las máscaras según la pared y la rotación
// wallIndex: 0=Norte, 1=Este, 2=Sur, 3=Oeste
// La textura tiene 4 filas (0.25 de altura cada una)
glm::vec2 getMaskUVOffset(int wallIndex)
{
	// Aplicar rotación: cada pared muestra una fila diferente
	// La rotación es cíclica (0->1->2->3->0)
	int textureRow = (wallIndex + maskRotation) % 4;
	
	// El offset Y determina qué fila de la textura usar
	// Fila 0: Y offset = 0.0 (parte superior de la imagen)
	// Fila 1: Y offset = 0.25
	// Fila 2: Y offset = 0.5
	// Fila 3: Y offset = 0.75
	float offsetY = 0.25f * textureRow;
	
	return glm::vec2(0.0f, offsetY);
}

