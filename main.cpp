
/*
Proyecto Final
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
#include <geometric.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>
#include "animations.h"
#include "KeyframeAnimation.h"
#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"
#include <algorithm>
#include "miniaudio.h"

//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;
#define VERIFY(x) if((x)!=MA_SUCCESS) return (x)
// Declaraciones globales para comunicación de eventos
bool g_addModelRequest = false;
glm::vec3 g_pendingModelPos = glm::vec3(0.0f, 0.0f, 0.0f);
bool g_removeModelRequest = false;
glm::vec3 g_removeModelPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::mat4 projection = glm::mat4(1.0f);

//ciclo 
bool day = true;
float lastGemRotationTime = 0.0f;
float gemRotationInterval = 3.0f;
// Variable para contador de rounds (0 a 14)
int roundCounter = 0,firstDigit,secondDigit;
//Rotación badajo
float badajo_rot = 0.0f;

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
Model miniReflector;
Model torchModel;
Model torchAce, torchCrash, torchSonic;
Model palmera_doble;
Model palmera_tres;
Model arbol_seis;
Model arbusto_grande;
Model arbusto_largo;
Model arbol_tronco;
Model anillo;
Model ring;
Model piramide;
Model cabeza_olmeca;
Model bote_basura;
Model bano;
Model fuente;
Model bancas;
Model reflector;
Model cajas_tnt;
Model cajas_nitro;
Model jumping;
Model columna_juzgado;
Model lugar_juzgado;
Model valla_juzgado;
Model silla_juzgado;
Model casa_aku_aku;
Model TNT,tapa;
Model opaopa;
Model bell;
Model badajo;
//Personajes
Model akuaku, ukauka, mayafey,  edgeworth, alexkid, dbjoe, crash;
Model gemaAzul, gemaRoja, gemaPurpura, gemaAmarilla;
Model ring_bell,palanca,soporte_bell;
Model Reloj_Minuto,Reloj_Hora,Reloj_Crash;
//Model clock;
std::map<std::string,Model> ace;
// Variables globales para comunicación de eventos
// (Eliminadas duplicadas)
Skybox skybox;
Texture explosion;
//materiales
Material Material_brillante;
Material Material_opaco;
std::vector<PointLight> lights;
//material del personaje
Material Material_personaje;

ma_sound s_explosion,s_soundtrack,s_box_bell,s_clock,s_bell,s_crash_bandicoot,s_boat,s_judge,s_box_ring,s_pyramid,s_gema;
ma_result result;
ma_engine eng;
ma_sound_group ambiental, effects;

// Banderas para control de sonidos
bool ringBellSoundPlayed = false;
bool clockSoundPlayed = false;
float lastClockRotation = 0.0f;
bool bellSoundPlayed = false;

// Variables para el personaje principal (Ace)
glm::vec3 characterPosition = glm::vec3(1300.0f, 19.5f, 0.0f);
GLfloat characterRotation = -90.0f; // Ace empieza mirando hacia -90 grados
GLfloat characterMoveSpeed = 2.0f; // Velocidad ajustada
bool characterIsMoving = false;

// Escala para audio 3D
const float AUDIO_SCALE = 0.01f;


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

//toris
std::vector<std::vector<GLfloat>> coordsToris = {
	{1142.11, 0.00, 1.83, 0.00},
	{ -1143.86,0.00,3.86,180.00},
	{ 0.85,0.00,790.04,90.00},
	{ 4.98,0.00,-776.96,90.00}


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
std::vector<std::vector<GLfloat>> coordsOlmechead = {

	{666.64,0.00,-48.79},
	{ 266.90,0.00,-684.57},
	{ 490.30,0.00,-618.25 },
	{ 223.25,0.00,-442.40 },
	{ 443.99,0.00,-46.41 }

};
std::vector<glm::vec3> gemPositions = {
	glm::vec3(620.13f, 0.00f, 261.66f),
	glm::vec3(417.91f, 0.00f, 465.00f),
	glm::vec3(691.92f, 0.00f, 660.75f),
	glm::vec3(915.13f, 0.00f, 455.44f)
};

std::vector<std::vector<GLfloat>> coordsTrash = {

	{-36.14,0.00,250.11},
{ -31.82,0.00,-247.16 },
{ -412.42,0.00,-23.92 },
{  410.04,0.00,-27.38 },
{ 882.42,0.00,389.26},
{ 437.37,0.00,466.69 },
{ 480.02,0.00,-542.16},
{ -1224.58,0.00,-20.41 },
{ -1208.12,0.00,31.77},
{ -939.06,0.00,429.18 },
{ -471.91,0.00,450.46 }

};

std::vector<std::vector<GLfloat>> coordsBenches = {
	{ 53.5,0.00,-356.02},
	{ 53.5,0.00,-575.78},
	{ 68.5,0.00,367.69},
	{68.5,0.00,642.01},




};

std::vector<std::vector<GLfloat>> coordsBenches1 = {
	{ -47.69, -0.00, 514.49},
	{ -45.70, 0.00, -428.98},
};

std::vector<std::vector<GLfloat>> coordsBenches2 = {
{ 530.74,0.00,68.94},
{ 805.00,0.00,44.34 },
{ -530.74,0.00,47.57},
{ -805.00, 0.00, 51.16}

};
std::vector<std::vector<GLfloat>> coordsBoxnitro{
	{95.39,-0.00,160.99},
	{ -83.04,0.00,163.78},
	{ 11.51,0.00,166.35},
	{ 94.49,0.00,-170.89},
	{-91.62,0.00,-158.01 },
	{ 17.65,0.00,-178.13}
};

std::vector<std::vector<GLfloat>> coordsBoxtnt{
	{210.36,-0.00,70.01},
	{207.70,-0.00,-6.22},
	{204.72,-0.00,-91.38},
	{270.61,0.00,-101.24},
	{269.07,0.00,-12.74},
	{267.63,0.00,69.72},
	{ -173.51,0.00,72.05},
	{ -172.96,0.00,2.44},
	{ -176.41,0.00,-96.23},
	{ -255.62,0.00,-102.45},
	{ -246.78,0.00,-1.39},
	{ -239.21,0.00,85.09}
};

std::vector<std::vector<GLfloat>> coordsJumping{
	{ -482.38,0.00,-325.19},
{ -837.73,0.00,-330.34},
{- 869.94,0.00,-575.84},
{ - 491.94,0.00,-588.32},
{ - 693.65,0.00,-581.25},
{ - 661.57,0.00,-325.07},
{ -437.64, 0.00, -436.67},
{ -896.14, 0.00, -433.49}
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
//para reflectores
SpotLight spotlighttemp[MAX_SPOT_LIGHTS];
int s = 0;


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

		// Billboard para explosión (plano que siempre mira a la cámara)
	unsigned int billboardIndices[] = {
		0, 1, 2,
		0, 2, 3
	};

	GLfloat billboardVertices[] = {
		//	x      y      z		u	  v			nx	  ny    nz
		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		 1.0f,  1.0f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f,
		-1.0f,  1.0f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 1.0f
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


	Mesh *billboard = new Mesh();
	billboard->CreateMesh(billboardVertices, billboardIndices, 32, 6);
	meshList.push_back(billboard);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	

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

int loadSoundGroups() {

	result = ma_sound_group_init(&eng, 0, NULL, &ambiental);
	VERIFY(result);
	ma_sound_group_set_volume(&ambiental, 0.3);

	result = ma_sound_group_init(&eng, 0, NULL, &effects);
	VERIFY(result);
	ma_sound_group_set_volume(&effects,0.4);
	//ma_sound_group_set_volume(&effects, 0.5);
	return MA_SUCCESS;
}
//s_boat,s_judge,s_box_ring
int loadSounds() {
	//soundtrack
	result = ma_sound_init_from_file(&eng, "Audio/Soundtrack.mp3", MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_NO_SPATIALIZATION, NULL, NULL, &s_soundtrack);
	VERIFY(result);
	ma_sound_set_volume(&s_soundtrack,0.2);

	
	result = ma_sound_init_from_file(&eng, "Audio/explosion.wav", MA_SOUND_FLAG_DECODE, &effects, NULL, &s_explosion);
	VERIFY(result);
	ma_sound_set_volume(&s_explosion,0.5);
	
	result = ma_sound_init_from_file(&eng,"Audio/Box_Bell.wav",MA_SOUND_FLAG_DECODE,&effects,NULL,&s_box_bell);
	ma_sound_set_volume(&s_box_bell,0.3);

	VERIFY(result);
	
	result = ma_sound_init_from_file(&eng,"Audio/clock.wav",MA_SOUND_FLAG_DECODE,&effects,NULL,&s_clock);
	ma_sound_set_volume(&s_clock,0.5);
	VERIFY(result);

	result = ma_sound_init_from_file(&eng,"Audio/Sonido_Campana.wav",MA_SOUND_FLAG_DECODE,&effects,NULL,&s_bell);
	ma_sound_set_volume(&s_bell,0.3);
	VERIFY(result);
	
	// Comentado temporalmente - usa s_bell dos veces
	result = ma_sound_init_from_file(&eng,"Audio/Crash_gema.wav",MA_SOUND_FLAG_DECODE,&effects,NULL,&s_gema);
	ma_sound_set_volume(&s_gema,0.1f);
	ma_sound_set_looping(&s_gema, MA_TRUE);

	VERIFY(result);

	//Ambientales
	result = ma_sound_init_from_file(&eng,"Audio/Crash_Bandicoot.mp3",MA_SOUND_FLAG_DECODE,&ambiental,NULL,&s_crash_bandicoot);
	ma_sound_set_looping(&s_crash_bandicoot, MA_TRUE);
	ma_sound_set_volume(&s_crash_bandicoot, 0.35f);
	// Configurar atenuación: min_distance = 50 unidades de audio (500 en mundo), rolloff = 1.0
	ma_sound_set_min_distance(&s_crash_bandicoot, 3.0f);
	ma_sound_set_rolloff(&s_crash_bandicoot, 4.0f);
	ma_sound_start(&s_crash_bandicoot);

	result = ma_sound_init_from_file(&eng,"Audio/Ambiental_Barco.mp3",MA_SOUND_FLAG_DECODE,&ambiental,NULL,&s_boat);
	ma_sound_set_looping(&s_boat, MA_TRUE);
	ma_sound_set_volume(&s_boat, 5.0f);
	ma_sound_set_min_distance(&s_boat, 5.0f);
	ma_sound_set_rolloff(&s_boat, 1.0f);
	ma_sound_start(&s_boat);
	
	result = ma_sound_init_from_file(&eng,"Audio/Ambiental_Juzgado.mp3",MA_SOUND_FLAG_DECODE,&ambiental,NULL,&s_judge);
	ma_sound_set_looping(&s_judge, MA_TRUE);
	ma_sound_set_volume(&s_judge, 0.7f);
	ma_sound_set_min_distance(&s_judge, 3.0f);
	ma_sound_set_rolloff(&s_judge, 4.0f);
	ma_sound_start(&s_judge);
	
	result = ma_sound_init_from_file(&eng,"Audio/Ambiental_Luchas.mp3",MA_SOUND_FLAG_DECODE,&ambiental,NULL,&s_box_ring);
	ma_sound_set_looping(&s_box_ring, MA_TRUE);
	ma_sound_set_volume(&s_box_ring, 0.7f);
	ma_sound_set_min_distance(&s_box_ring, 3.0f);
	ma_sound_set_rolloff(&s_box_ring, 4.0f);
	ma_sound_start(&s_box_ring);

	result = ma_sound_init_from_file(&eng,"Audio/Ambiental_Piramide.mp3",MA_SOUND_FLAG_DECODE,&ambiental,NULL,&s_pyramid);
	ma_sound_set_looping(&s_pyramid, MA_TRUE);
	ma_sound_set_volume(&s_pyramid, 10.0f);
	ma_sound_set_min_distance(&s_pyramid, 3.0f);
	ma_sound_set_rolloff(&s_pyramid, 4.0f);
	ma_sound_start(&s_pyramid);


	return MA_SUCCESS;

}

void unloadSoundsGroups()
{
	ma_sound_group_uninit(&ambiental);
	ma_sound_group_uninit(&effects);
}

void unloadSounds() {
	if(ma_sound_is_playing(&s_explosion)==MA_TRUE){
		ma_sound_stop(&s_explosion);
	}
	ma_sound_uninit(&s_explosion);

	if(ma_sound_is_looping(&s_soundtrack)==MA_TRUE){
		ma_sound_set_looping(&s_soundtrack,MA_FALSE);
		ma_sound_stop(&s_soundtrack);
	}
	ma_sound_uninit(&s_soundtrack);

	// Limpiar sonidos de efectos
	if(ma_sound_is_playing(&s_box_bell)==MA_TRUE){
		ma_sound_stop(&s_box_bell);
	}
	ma_sound_uninit(&s_box_bell);

	if(ma_sound_is_playing(&s_clock)==MA_TRUE){
		ma_sound_stop(&s_clock);
	}
	ma_sound_uninit(&s_clock);

	if(ma_sound_is_playing(&s_bell)==MA_TRUE){
		ma_sound_stop(&s_bell);
	}
	ma_sound_uninit(&s_bell);

	// Limpiar sonidos ambientales
	if(ma_sound_is_looping(&s_crash_bandicoot)==MA_TRUE){
		ma_sound_set_looping(&s_crash_bandicoot,MA_FALSE);
		ma_sound_stop(&s_crash_bandicoot);
	}
	ma_sound_uninit(&s_crash_bandicoot);

	if(ma_sound_is_looping(&s_boat)==MA_TRUE){
		ma_sound_set_looping(&s_boat,MA_FALSE);
		ma_sound_stop(&s_boat);
	}
	ma_sound_uninit(&s_boat);

	if(ma_sound_is_looping(&s_judge)==MA_TRUE){
		ma_sound_set_looping(&s_judge,MA_FALSE);
		ma_sound_stop(&s_judge);
	}
	ma_sound_uninit(&s_judge);

	if(ma_sound_is_looping(&s_box_ring)==MA_TRUE){
		ma_sound_set_looping(&s_box_ring,MA_FALSE);
		ma_sound_stop(&s_box_ring);
	}
	ma_sound_uninit(&s_box_ring);

	if(ma_sound_is_looping(&s_pyramid)==MA_TRUE){
		ma_sound_set_looping(&s_pyramid,MA_FALSE);
		ma_sound_stop(&s_pyramid);
	}
	ma_sound_uninit(&s_pyramid);
}

//funciones para el ciclo de día
void setNight(std::vector<std::string> skyboxNight)
{
	skybox = Skybox(skyboxNight);
	// Luz direccional azulada, baja intensidad
	mainLight = DirectionalLight(
		0.4f, 0.5f, 1.0f,   // Azul tenue
		0.15f, 0.15f,       // Intensidad baja
		0.0f, 0.0f, -1.0f   // Dirección
	);
}

void setDay(std::vector<std::string> skyboxDay) {
	skybox = Skybox(skyboxDay);
	// Luz direccional blanca, intensidad moderada
	mainLight = DirectionalLight(
		1.0f, 1.0f, 1.0f,   // Blanco
		0.5f, 0.5f,         // Intensidad moderada
		0.0f, 0.0f, -1.0f   // Dirección
	);
}

// Función para crear una matriz billboard que siempre mira hacia la cámara
glm::mat4 CreateBillboardMatrix(glm::vec3 position, glm::vec3 cameraPos, glm::vec3 cameraUp,glm::vec3 trans)
{
	// Vector que apunta de la posición del billboard hacia la cámara
	glm::vec3 look = glm::normalize(cameraPos - position);
	
	// Vector derecha (perpendicular a look y up)
	glm::vec3 right = glm::normalize(glm::cross(cameraUp, look));
	
	// Vector arriba recalculado (perpendicular a look y right)
	glm::vec3 up = glm::cross(look, right);

	// Construir matriz de transformación
	glm::mat4 billboardMatrix(1.0f);
	billboardMatrix[0] = glm::vec4(right, 0.0f);
	billboardMatrix[1] = glm::vec4(up, 0.0f);
	billboardMatrix[2] = glm::vec4(look, 0.0f);
	billboardMatrix[3] = glm::vec4(position, 1.0f);
	billboardMatrix = glm::translate(billboardMatrix,trans);
	return billboardMatrix;
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
	

	for(std::vector<float> v:coordTorch)
	{
		lights.push_back(
			PointLight(1.0f, 1.0f, 0.0f,
			0.0f, 1.0f,
			v[0], v[1]+14.0, v[2]*2.0f/1.5f,
			0.01f, 0.01f, 0.001f)
		);
	}

	// Inicializar sistema de animaciones por keyframes*/
	InitKeyframeAnimations();
	
	// Asociar sonidos a las animaciones de efectos especiales
	KeyframeAnimation* bellAnim = g_AnimationManager.GetAnimation("Bell");
	
	
	KeyframeAnimation* ringBellAnim = g_AnimationManager.GetAnimation("Ring_Bell");
	if(ringBellAnim) {
		ringBellAnim->SetSound(&s_box_bell);
	}
	
	KeyframeAnimation* relojAnim = g_AnimationManager.GetAnimation("Reloj");
	
	
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);
	
	// Configurar puntos de interés para la cámara
	camera.addPointOfInterest(glm::vec3(8.47f, 76.42f, 218.42f), glm::vec3(6.75f, 59.06f, 119.95f), "Ring de lucha");
	camera.addPointOfInterest(glm::vec3(-690.08f, 196.69f, -29.28f), glm::vec3(-688.41f, 167.45f, -124.89f), "Barco");
	camera.addPointOfInterest(glm::vec3(373.02f, 67.41f, -97.56f), glm::vec3(443.72f, 65.66f, -168.26f), "Piramide");
	camera.addPointOfInterest(glm::vec3(661.21f, 40.04f, 211.61f), glm::vec3(666.44f, 38.29f, 311.45f), "Casa crash");
	camera.addPointOfInterest(glm::vec3(-444.85f, 49.15f, 619.07f), glm::vec3(-527.30f, 38.70f, 563.46f), "Juzgado");
	camera.addPointOfInterest(glm::vec3(247.47f, 28.32f, 87.69f), glm::vec3(240.76f, 0.76f, -8.21f), "TNT");
	camera.addPointOfInterest(glm::vec3(-325.49f, 99.74f, -85.75f), glm::vec3(-231.58f, 65.54f, -89.03f), "Opa-Opa");
	
	barco = Model();
	barco.LoadModel("Models/Barco.obj");
	piso=Model();
	piso.LoadModel("Models/piso.obj");
	TNT = Model();
	TNT.LoadModel("Models/Caja_TNT_sin_tapa.obj");
	tapa = Model();
	tapa.LoadModel("Models/tapa_TNT.obj");
	tori = Model();
	tori.LoadModel("Models/Tori.obj");
	miniReflector = Model();
	miniReflector.LoadModel("Models/Mini_Reflector.obj");
	bell = Model();
	bell.LoadModel("Models/Bell.obj");
	badajo = Model();
	badajo.LoadModel("Models/Badajo.obj");
	soporte_bell = Model();
	soporte_bell.LoadModel("Models/Boxing_Bell_soporte.obj");
	palanca = Model();
	palanca.LoadModel("Models/Boxing_Bell_palanca.obj");
	ring_bell = Model();
	ring_bell.LoadModel("Models/Boxing_Bell.obj");
	Reloj_Crash = Model();
	Reloj_Crash.LoadModel("Models/reloj_crash.obj");
	Reloj_Minuto = Model();
	Reloj_Minuto.LoadModel("Models/reloj_minutero.obj");
	Reloj_Hora = Model();
	Reloj_Hora.LoadModel("Models/reloj_flecha.obj");
	/*
	torchAce = Model();
	torchAce.LoadModel("Models/Antorcha_Ace_Attorney.obj");
	torchCrash = Model();
	torchCrash.LoadModel("Models/antorcha_crash.obj");
	torchSonic = Model();
	torchSonic.LoadModel("Models/Antorcha_Sonic.obj");
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
	arbol_tronco.LoadModel("Models/Arbol12.obj");*/
	anillo = Model();
	anillo.LoadModel("Models/Anillo.obj");
	ring = Model();
	ring.LoadModel("Models/Boxing Ring.obj");
	piramide = Model();
	piramide.LoadModel("Models/Piramide.obj");/*
	cabeza_olmeca = Model();
	cabeza_olmeca.LoadModel("Models/CabezaOlmeca.obj");
	bote_basura = Model();
	bote_basura.LoadModel("Models/Basurero.obj");
	bano = Model();
	bano.LoadModel("Models/baño.obj");
	fuente = Model();
	fuente.LoadModel("Models/Fuente.obj");
	bancas= Model();
	bancas.LoadModel("Models/Banca2.obj");
	reflector = Model();
	reflector.LoadModel("Models/Reflector.obj");
	cajas_nitro = Model();
	cajas_nitro.LoadModel("Models/Caja_Nitro.obj");
	cajas_tnt = Model();
	cajas_tnt.LoadModel("Models/Caja_TNT.obj");
	jumping = Model();
	jumping.LoadModel("Models/jumping.obj");*/
	columna_juzgado = Model();
	columna_juzgado.LoadModel("Models/ColumnaJuzgado.obj");
	lugar_juzgado = Model();
	lugar_juzgado.LoadModel("Models/JuezLugar.obj");
	valla_juzgado = Model();
	valla_juzgado.LoadModel("Models/VallaJuzgado.obj");
	silla_juzgado = Model();
	silla_juzgado.LoadModel("Models/SillaJuzgado.obj");
	casa_aku_aku = Model();
	casa_aku_aku.LoadModel("Models/CasaAkuAku.obj");
	gemaAzul = Model();
	gemaAzul.LoadModel("Models/GemaAzul.obj");
	gemaRoja = Model();
	gemaRoja.LoadModel("Models/GemaRojo.obj");
	gemaPurpura = Model();
	gemaPurpura.LoadModel("Models/GemaPurpura.obj");
	gemaAmarilla = Model();
	gemaAmarilla.LoadModel("Models/GemaAmarillo.obj");
	
	//personajes
	/*akuaku = Model();	
	akuaku.LoadModel("Models/Aku-Aku.obj");
	crash = Model();
	crash.LoadModel("Models/CrashBandicoot.obj");
	ukauka = Model();
	ukauka.LoadModel("Models/ukauka.obj");
	mayafey = Model();
	mayafey.LoadModel("Models/Maya Fey.obj");
	edgeworth = Model();
	edgeworth.LoadModel("Models/edgeworth.obj");
	alexkid = Model();
	alexkid.LoadModel("Models/alexKid.obj");
	dbjoe = Model();
	dbjoe.LoadModel("Models/DBJoe.obj");*/
	opaopa = Model();
	opaopa.LoadModel("Models/Opa-Opa.obj");
	std::vector<std::string> ModelAce={"BrazoDerechoAce","BrazoIzquierdoAce","CuerpoAce","HombroDerechoAce","HombroIzquierdoAce",
	"MusloDerechoAce","MusloIzquierdoAce","PiernaDerechaAce","PiernaIzquierdaAce"};
	for(std::string s:ModelAce){
		ace[s] = Model();
		ace[s].LoadModel("Models/Principal/"+s+".obj");
	}

		//Cycle day

	//Skybox Night

	std::vector<std::string> skyboxNight;
	skyboxNight.push_back("Textures/Skybox/Zona_Nebulosa.png");
	skyboxNight.push_back("Textures/Skybox/Zona_Estrellas.png");
	skyboxNight.push_back("Textures/Skybox/Zona_Estrellas.png");
	skyboxNight.push_back("Textures/Skybox/Zona_Estrellas.png");
	skyboxNight.push_back("Textures/Skybox/Zona_Planeta.png");
	skyboxNight.push_back("Textures/Skybox/Zona_Luna.png");

	//Skybox Day
	std::vector<std::string> skyboxDay;
	skyboxDay.push_back("Textures/Skybox/sh_rt.png");
	skyboxDay.push_back("Textures/Skybox/sh_lf.png");
	skyboxDay.push_back("Textures/Skybox/sh_dn.png");
	skyboxDay.push_back("Textures/Skybox/sh_up.png");
	skyboxDay.push_back("Textures/Skybox/sh_bk.png");
	skyboxDay.push_back("Textures/Skybox/sh_ft.png");

	std::vector<std::string> skyboxFaces = skyboxDay;
	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256.0f);
	Material_opaco = Material(0.0f, 0.0f);  // Sin reflexión especular, como si no tuviera material
	Material_personaje = Material(0.2f, 14);
	mascaras = Texture("Textures/Masks.png"); mascaras.LoadTextureA();
	numeros= Texture("Textures/Numeros.png"); numeros.LoadTextureA();
	explosion = Texture("Textures/Explosion.png"); explosion.LoadTextureA();
	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	//para los reflectores 
	glm::vec3 ringPos = glm::vec3(8.55f, 0.0f, -12.67f);
	glm::vec3 piraPos = glm::vec3(696.86f, 195.00f, -413.49f);
	glm::vec3 reflectorPos1 = glm::vec3(-253.01f, 125.0f, 197.44f); // encima del reflector
	glm::vec3 dir = glm::normalize(ringPos-reflectorPos1);
	
	spotLights[0] = SpotLight(
		1.0f, 1.0f, 1.0f,          
		5.0f, 5.0f,                // ambient, diffuse
		reflectorPos1.x, reflectorPos1.y,  reflectorPos1.z,  // posición
		dir.x, dir.y, dir.z,       // dirección hacia el ring
		1.0f, 0.01f, 0.001f,      
		100.0f                     
	);
	
	glm::vec3 reflectorPos2 = glm::vec3(-234.55, 125.00, -206.86); // encima del reflector
	glm::vec3 dir2 = glm::normalize(ringPos - reflectorPos2);
	
	spotLights[1] = SpotLight(
		1.0f, 1.0f, 1.0f,          
		5.0f, 5.0f,                // ambient, diffuse
		reflectorPos2.x, reflectorPos2.y, reflectorPos2.z,  // posición
		dir2.x, dir2.y, dir2.z,       // dirección hacia el ring
		1.0f, 0.01f, 0.001f,       
		100.0f                      
	);
	glm::vec3 reflectorPos3 = glm::vec3(337.30, 125.00, -143.94); // encima del reflector
	glm::vec3 dir3 = glm::normalize(ringPos - reflectorPos3);
	
	spotLights[2] = SpotLight(
		1.0f, 1.0f, 1.0f,          
		5.0f, 5.0f,                // ambient, diffuse
		reflectorPos3.x, reflectorPos3.y, reflectorPos3.z,  // posición
		dir3.x, dir3.y, dir3.z,       // dirección hacia el ring
		1.0f, 0.01f, 0.001f,       
		100.0f                      
	);
	
	glm::vec3 reflectorPos4= glm::vec3(253.84, 125.00, 203.23); // encima del reflector
	glm::vec3 dir4 = glm::normalize(ringPos - reflectorPos4);
	
	spotLights[3] = SpotLight(
		1.0f, 1.0f, 1.0f,       
		5.0f, 5.0f,                // ambient, diffuse
		reflectorPos4.x, reflectorPos4.y, reflectorPos4.z,  // posición
		dir4.x, dir4.y, dir4.z,       // dirección hacia el ring
		1.0f, 0.01f, 0.001f,      
		100.0f                     
	);

	glm::vec3 minireflectorPos1 = glm::vec3(995.15f, 8.611f, -407.96f); // encima del reflector
	glm::vec3 dir5 = glm::normalize(piraPos - minireflectorPos1);
	
	spotLights[4] = SpotLight(
		1.0f, 0.0f, 0.0f,			// ambient
		5.0f, 5.0f,                // ambient, diffuse
		minireflectorPos1.x, minireflectorPos1.y, minireflectorPos1.z,  // posición
		dir5.x, dir5.y, dir5.z,       // dirección hacia el ring
		1.0f, 0.01f, 0.001f,
		60.0f
	);
	
	glm::vec3 minireflectorPos2 = glm::vec3(698.30f, 8.611f, -746.17f); // encima del reflector
	glm::vec3 dir6 = glm::normalize(piraPos - minireflectorPos2);
	
	spotLights[5] = SpotLight(
		0.0f, 1.0f, 01.0f,			// ambient
		5.0f, 5.0f,                // ambient, diffuse
		minireflectorPos2.x, minireflectorPos2.y, minireflectorPos2.z,  // posición
		dir6.x, dir6.y, dir6.z,       // dirección hacia el ring
		1.0f, 0.01f, 0.001f,
		60.0f
	);
	
	glm::vec3 minireflectorPos3 = glm::vec3(353.17f, 8.611f, -404.60f); // encima del reflector
	glm::vec3 dir7 = glm::normalize(piraPos - minireflectorPos3);
	
	spotLights[6] = SpotLight(
		0.0f, 0.0f, 1.0f,			// ambient
		5.0f, 5.0f,                // ambient, diffuse
		minireflectorPos3.x, minireflectorPos3.y, minireflectorPos3.z,  // posición
		dir7.x, dir7.y, dir7.z,       // dirección hacia el ring
		1.0f, 0.01f, 0.001f,
		60.0f
	);
	
	glm::vec3 minireflectorPos4 = glm::vec3(694.67f, 8.611f, -141.98f); // encima del reflector
	glm::vec3 dir8 = glm::normalize(piraPos - minireflectorPos4);
	
	spotLights[7] = SpotLight(
		1.0f, 1.0f, 0.0f,			// ambient
		5.0f, 5.0f,                // ambient, diffuse
		minireflectorPos4.x, minireflectorPos4.y, minireflectorPos4.z,  // posición
		dir8.x, dir8.y, dir8.z,       // dirección hacia el ring
		1.0f, 0.01f, 0.001f,
		60.0f
	);

	//Creación del motor de sonidos
	result=ma_engine_init(NULL,&eng);
	VERIFY(result);

	//Carga grupos de sonidos

	VERIFY(loadSoundGroups());

	//Carga de los sonidos
	VERIFY(loadSounds());
	
	//Area de sonidos
	ma_sound_set_looping(&s_soundtrack,MA_TRUE);
	ma_sound_start(&s_gema);
    ma_sound_start(&s_soundtrack);

	glm::vec2 offset;
	int idx,aux;
	//variables para el ciclo de dia y noche
	float lastSwitchTime = 0.0f;
	float switchInterval = 500.0f;//5 minutos
	//se crean mas luces puntuales y spotlight 
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelboat(1.0);
	glm::mat4 modeljuz(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::vec3 p;
	glm::vec4 rot;
	
	// Variables del loop principal declaradas fuera para optimización
	GLfloat now;
	glm::mat4 view;
	glm::vec3 camPos, lowerLight;
	float da, db;
	int l;
	glm::vec3 explosionOffset, explosionWorldPos, toCamera, up_billboard, right_billboard;
	glm::mat4 billboardRotation;
	glm::vec3 savedPosition;
	float currentScale;
	float rotateZ=0.0f, posCampana_z, rotCampana_x, rotPalanca_x;
	float manecilla_hora, manecilla_minuto;
	glm::vec3 acePosition;
	glm::mat4 bodyModel, rightShoulderModel, leftShoulderModel;
	glm::mat4 rightThighModel, leftThighModel;
	
	int pointLightCount,spotLightCount=0;
	//para animacion de las gemas
	float radius = 200.0f;
	float gemRotationAngle = 0.0f;
	float angle = 0.0f;
	// Tomando como Centro la posicion de la casa de aku aku 
	glm::vec3 centerPosition = glm::vec3(640.59, 20.0f, 456.90);
	projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 5000.0f);
	
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose()){

	
		now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		

		if (now - lastSwitchTime > switchInterval) {
				day = !day;
				lastSwitchTime = now;
				if (day) {
					printf("Llego el dia\n");
					setDay(skyboxDay);
			}
			else {
				printf("Llego la noche\n");
				setNight(skyboxNight);
				}
			}

		s = 0;
		if (mainWindow.prendido()) {
			spotlighttemp[s++] = spotLights[0];
			spotlighttemp[s++] = spotLights[1];
			spotlighttemp[s++] = spotLights[2];
			spotlighttemp[s++] = spotLights[3];
		
		}
		if (mainWindow.prendido2()) {
			spotlighttemp[s++] = spotLights[4];
			spotlighttemp[s++] = spotLights[5];
			spotlighttemp[s++] = spotLights[6];
			spotlighttemp[s++] = spotLights[7];
		}

		//Recibir eventos del usuario
		glfwPollEvents();
		
		// Cambio de modo de cámara con teclas numéricas
		if (mainWindow.getsKeys()[GLFW_KEY_1])
		{
			camera.setCameraMode(THIRD_PERSON);
		}
		if (mainWindow.getsKeys()[GLFW_KEY_2])
		{
			camera.setCameraMode(AERIAL_FREE);
			SetWalkingActive(false); // Detener animación al cambiar de cámara
		}
		if (mainWindow.getsKeys()[GLFW_KEY_3])
		{
			camera.setCameraMode(POINT_OF_INTEREST);
			SetWalkingActive(false); // Detener animación al cambiar de cámara
		}
		if (mainWindow.getsKeys()[GLFW_KEY_4])
		{
			camera.setCameraMode(FREE_CAMERA);
			SetWalkingActive(false); // Detener animación al cambiar de cámara
		}

		// Control específico según el modo de cámara
		CameraMode currentMode = camera.getCameraMode();
		
		if (currentMode == THIRD_PERSON)
		{
			// Control del personaje en modo tercera persona
			characterIsMoving = false;
			GLfloat velocity = characterMoveSpeed * deltaTime;
			glm::vec3 cameraForward = camera.getCameraDirection();
			cameraForward.y = 0.0f;
			cameraForward = glm::normalize(cameraForward);

			if (mainWindow.getsKeys()[GLFW_KEY_W])
			{
				characterPosition += cameraForward * velocity;
				characterIsMoving = true;
			}

			if (mainWindow.getsKeys()[GLFW_KEY_S])
			{
				characterPosition -= cameraForward * velocity;
				characterIsMoving = true;
			}

			// Activar/desactivar animación de caminata según el movimiento
			SetWalkingActive(characterIsMoving);

			// Control del mouse para rotar la cámara (invertido X e Y solo en tercera persona)
			camera.mouseControl(-mainWindow.getXChange(), -mainWindow.getYChange());
			
			// Actualizar la cámara tercera persona
			camera.updateThirdPerson(characterPosition, characterRotation, characterIsMoving, deltaTime);
		}
		else if (currentMode == AERIAL_FREE)
		{
			// Control de cámara aérea libre
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
			camera.updateAerial(mainWindow.getsKeys(), deltaTime);
		}
		else if (currentMode == POINT_OF_INTEREST)
		{
			// Navegación entre puntos de interés con flechas izquierda/derecha
			static bool leftKeyPressed = false;
			static bool rightKeyPressed = false;

			if (mainWindow.getsKeys()[GLFW_KEY_LEFT] && !leftKeyPressed)
			{
				camera.previousPointOfInterest();
				leftKeyPressed = true;
			}
			else if (!mainWindow.getsKeys()[GLFW_KEY_LEFT])
			{
				leftKeyPressed = false;
			}

			if (mainWindow.getsKeys()[GLFW_KEY_RIGHT] && !rightKeyPressed)
			{
				camera.nextPointOfInterest();
				rightKeyPressed = true;
			}
			else if (!mainWindow.getsKeys()[GLFW_KEY_RIGHT])
			{
				rightKeyPressed = false;
			}
		}
		else if (currentMode == FREE_CAMERA)
		{
			// Modo cámara libre original (WASD + mouse)
			camera.keyControl(mainWindow.getsKeys(), deltaTime);
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		}

		// Actualizar animaciones por keyframes
		UpdateKeyframeAnimations();

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
		view = camera.calculateViewMatrix();
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// Ordenar luces por distancia a la cámara (más cercanas primero)
		std::sort(lights.begin(), lights.end(),
		[&](const PointLight& a, const PointLight& b) {
			camPos = camera.getCameraPosition();
			da = glm::distance(a.GetPosition(), camPos);
			db = glm::distance(b.GetPosition(), camPos);
			return da < db;
		});
		
		// Copiar las 11 luces más cercanas al arreglo pointLights
		// Solo si están a menos de 300 metros
		pointLightCount = 0;
		for(int i = 0; i < lights.size() && pointLightCount < MAX_POINT_LIGHTS; i++){
			float distanceToCamera = glm::distance(lights[i].GetPosition(), camera.getCameraPosition());
			if(distanceToCamera < 300.0f){
				pointLights[pointLightCount] = lights[i];
				pointLightCount++;
			} else {
				// Como están ordenadas, si esta está muy lejos, las siguientes también
				break;
			}
		}
		
		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		
		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		if(!day)
			shaderList[0].SetPointLights(pointLights, pointLightCount);
			shaderList[0].SetSpotLights(spotlighttemp, s);
		
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		//Sincronizacion del listener y camara
		glm::vec3 camPos = camera.getCameraPosition()*0.01f;
		ma_engine_listener_set_position(&eng, 0, camPos.x, camPos.y, camPos.z);

		// Establecer material opaco por defecto para todos los modelos
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		//Piso 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		piso.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-694.59f,0.00,-418.33f));
		model = AnimationShip(model);
		modelboat = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		barco.RenderModel();
		
		// Posicionar sonido ambiental del barco
		glm::vec3 barcoWorldPos = glm::vec3(model[3]);
		ma_sound_set_position(&s_boat, 
			barcoWorldPos.x * AUDIO_SCALE, 
			barcoWorldPos.y * AUDIO_SCALE, 
			barcoWorldPos.z * AUDIO_SCALE);

		//ciclo for para las toris
		/*for (std::vector <GLfloat> v : coordsToris) {
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(v[0], v[1], v[2]));
			model = glm::rotate(model, glm::radians(v[3]), glm::vec3(0.0, 1.0, 0.0));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			tori.RenderModel();
		
		}

		aux = 0;
		for (const auto& coor : coordTorch) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(coor[0], coor[1], coor[2]*2.0f/1.5f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			switch (aux) {
			case 0:
				break;
			case 1:
				model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0.0f));
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
	    */
		//ring
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(8.55, 0.00, -12.67));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ring.RenderModel();
		
		// Posicionar sonido ambiental del ring de boxeo
		glm::vec3 ringWorldPos = glm::vec3(model[3]);
		ma_sound_set_position(&s_box_ring, 
			ringWorldPos.x * AUDIO_SCALE, 
			ringWorldPos.y * AUDIO_SCALE, 
			ringWorldPos.z * AUDIO_SCALE);
		
		//piramide
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(696.86, 0.00, -413.49));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		piramide.RenderModel();
		
		// Posicionar sonido ambiental de la pirámide
		glm::vec3 piramideWorldPos = glm::vec3(model[3]);
		ma_sound_set_position(&s_pyramid, 
			piramideWorldPos.x * AUDIO_SCALE, 
			piramideWorldPos.y * AUDIO_SCALE, 
			piramideWorldPos.z * AUDIO_SCALE);
				//ciclo for para cabezas olmecas
		/*for (std::vector <GLfloat> v : coordsOlmechead) {
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(v[0], v[1], v[2]));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			cabeza_olmeca.RenderModel();
		}
		//ciclo for para los botes de basura
		for (std::vector <GLfloat> v : coordsTrash) {
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(v[0], v[1], v[2]));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			bote_basura.RenderModel();
		}
		//banos
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-121.24, 0.00, 271.83));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bano.RenderModel();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-117.98, 0.00, -264.41));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bano.RenderModel();

		//fuentes
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-194.56, 0.00, 554.75));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		fuente.RenderModel();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1080.15, 0.00, -502.91));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		fuente.RenderModel();
		//bancas
		for (std::vector <GLfloat> v : coordsBenches) {
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(v[0], v[1], v[2]));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			bancas.RenderModel();
		}
		
		for (std::vector <GLfloat> v : coordsBenches1) {
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(v[0], v[1], v[2]));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			bancas.RenderModel();
		}
		for (std::vector <GLfloat> v : coordsBenches2) {
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(v[0], v[1], v[2]));
			model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			bancas.RenderModel();
		}
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-728.09, 0.00, -36.75));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bancas.RenderModel();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(793.39, 0.00, -40.21));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bancas.RenderModel();
		
		//reflectores*/
		//lado del juzgado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-253.01, 0.00, 197.44));
		modelaux = model;
		spotLights[3].SetPos(glm::vec3(modelaux[3].x, modelaux[3].y + 125.0f, modelaux[3].z));
		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		reflector.RenderModel();
		
		//lado del barco
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-234.55, 0.00, -206.86));
		modelaux = model;
		spotLights[3].SetPos(glm::vec3(modelaux[3].x, modelaux[3].y + 125.0f, modelaux[3].z));
		model = glm::rotate(model, glm::radians(55.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		reflector.RenderModel();
		
		//lado de crash
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(337.30, 0.00, -143.94));
		modelaux = model;
		spotLights[3].SetPos(glm::vec3(modelaux[3].x, modelaux[3].y + 125.0f, modelaux[3].z));
		model = glm::rotate(model, glm::radians(280.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		reflector.RenderModel();
		
		//lado de la piramide
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(253.84, 0.00, 203.23));
		modelaux = model;
		spotLights[3].SetPos(glm::vec3(modelaux[3].x, modelaux[3].y + 125.0f, modelaux[3].z));
		model = glm::rotate(model, glm::radians(245.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		reflector.RenderModel();

		//Minireflector
		//lado izquierdo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(994.05f, 0.00f, -407.96f));
		modelaux = model;
		spotLights[4].SetPos(glm::vec3(modelaux[3].x + 1.1f, modelaux[3].y + 8.611f, modelaux[3].z));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		miniReflector.RenderModel();
		
		//atras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(698.30f, 0.00f, -745.47f));
		modelaux = model;
		spotLights[5].SetPos(glm::vec3(modelaux[3].x, modelaux[3].y + 8.611f, modelaux[3].z - 1.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		miniReflector.RenderModel();
		
		//lado derecho
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(354.27f, 0.00f, -404.60f));
		modelaux = model;
		spotLights[6].SetPos(glm::vec3(modelaux[3].x - 1.1f, modelaux[3].y + 8.611f, modelaux[3].z));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		miniReflector.RenderModel();
		
		//enfrente
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(694.67f, 0.00f, -143.08f));
		modelaux = model;
		spotLights[7].SetPos(glm::vec3(modelaux[3].x, modelaux[3].y + 8.611f, modelaux[3].z + 1.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		miniReflector.RenderModel();
		/*
		//ciclo for para las cajas TNT
		for (std::vector <GLfloat> v : coordsBoxtnt) {
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(v[0], v[1], v[2]));
			model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			cajas_tnt.RenderModel();
		}
		
		//ciclo for para las cajas nitro
		for (std::vector <GLfloat> v : coordsBoxnitro) {
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(v[0], v[1], v[2]));
			model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			cajas_nitro.RenderModel();
		}
		
		//ciclo for para los jumping
		for (std::vector <GLfloat> v : coordsJumping) {
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(v[0], v[1], v[2]));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			jumping.RenderModel();
		}*/

		//juzgado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-678.49, 0.00, 345.40));
		modeljuz = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));	
		columna_juzgado.RenderModel();

		model = modeljuz;
		model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 61.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lugar_juzgado.RenderModel();
		
		// Posicionar sonido ambiental del juzgado
		glm::vec3 juzgadoWorldPos = glm::vec3(model[3]);
		ma_sound_set_position(&s_judge, 
			juzgadoWorldPos.x * AUDIO_SCALE, 
			juzgadoWorldPos.y * AUDIO_SCALE, 
			juzgadoWorldPos.z * AUDIO_SCALE);
	
		model = modeljuz;
		model = glm::translate(model, glm::vec3(-5.99f, 0.0f, 118.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla_juzgado.RenderModel();
		
		model = modeljuz;
		model = glm::translate(model, glm::vec3(9.78f, 0.0f, 218.68f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		silla_juzgado.RenderModel();

		//escenario aku aku 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(640.59, 0.00, 456.90));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		casa_aku_aku.RenderModel();
		
		// Posicionar sonido ambiental de Crash Bandicoot en la casa
		glm::vec3 casaWorldPos = glm::vec3(model[3]);
		ma_sound_set_position(&s_crash_bandicoot, 
			casaWorldPos.x * AUDIO_SCALE, 
			casaWorldPos.y * AUDIO_SCALE, 
			casaWorldPos.z * AUDIO_SCALE);
		
		ma_sound_set_position(&s_gema,
			centerPosition.x*AUDIO_SCALE,
			centerPosition.y*AUDIO_SCALE,
			centerPosition.z*AUDIO_SCALE
		);
		//Animacion gemas orbitando
		gemRotationAngle += 0.001f;
		for (int i = 0; i < 4; i++) {
			angle = gemRotationAngle + (i * glm::radians(90.0f));
			gemPositions[i] = centerPosition + glm::vec3(cos(angle) * radius, 0.0f, sin(angle) * radius);
		}

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		model = glm::mat4(1.0f);
		model = glm::translate(model, gemPositions[0]);
		model = glm::rotate(model, now * glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		gemaAzul.RenderModel();

		model = glm::mat4(1.0f);
		model = glm::translate(model, gemPositions[1]);
		model = glm::rotate(model, now * glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		gemaRoja.RenderModel();

		model = glm::mat4(1.0f);
		model = glm::translate(model, gemPositions[2]);
		model = glm::rotate(model, now * glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		gemaPurpura.RenderModel();

		model = glm::mat4(1.0f);
		model = glm::translate(model, gemPositions[3]);
		model = glm::rotate(model, now * glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		gemaAmarilla.RenderModel();
		// Restaurar material opaco para los demás modelos
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		
		//Personajes
		//Universo Crash
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(541.43, 1.50, 392.59));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		akuaku.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(793.55, 5.00, 405.88));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ukauka.RenderModel();

		// Crash permanece en posición fija (no es el personaje controlable)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(31.78, 23.00, -11.65));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		crash.RenderModel();

		//Universo Ace Attorney
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(292.36, 0.00, -722.11));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mayafey.RenderModel();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-687.69, 0.00, 456.59));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		edgeworth.RenderModel();
		//Universo Sonic

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-643.37, 0.00, -725.40));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		alexkid.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1043.61, 0.00, -183.27));
		model = glm::rotate(model, glm::radians(125.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dbjoe.RenderModel();

		//ANILLO
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-398.50, 25.00, -253.83));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		anillo.RenderModel();
		// Restaurar material opaco para los demás modelos
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(248.0f, 0.0f, 31.0f));
		model = AnimationTNT(model);  // Aplicar tambaleo
		modelaux = model;
		// Posicionar el sonido de explosión en la TNT
		glm::vec3 tntWorldPos = glm::vec3(model[3])*AUDIO_SCALE;
		ma_sound_set_position(&s_explosion, tntWorldPos.x,tntWorldPos.y,tntWorldPos.z);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TNT.RenderModel();

		// Animación tapa: elevación
		model = modelaux;
		model = AnimationTapa(model,s_explosion);  // Aplicar elevación
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tapa.RenderModel();
		
		// Billboard de explosión (aparece progresivamente desde el segundo 4)
		if (explosionScale > 0.0f) {
			// Activar blending para transparencia
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			
			// JERARQUÍA: Partir de la transformación del padre (TNT)
			model = modelaux; // modelaux tiene la posición y animación de la TNT
			
			// Offset relativo respecto a la TNT (subir 15 unidades en Y)
			explosionOffset = glm::vec3(0.0f, 15.0f, 0.0f);
			model = glm::translate(model, explosionOffset);
			
			// Extraer la posición mundial del billboard para calcular dirección a cámara
			explosionWorldPos = glm::vec3(model[3]);
			
			// Calcular vectores para orientar el billboard hacia la cámara
			toCamera = glm::normalize(camera.getCameraPosition() - explosionWorldPos);
			up_billboard = glm::vec3(0.0f, 1.0f, 0.0f);
			right_billboard = glm::normalize(glm::cross(up_billboard, toCamera));
			up_billboard = glm::cross(toCamera, right_billboard);
			
			// Crear matriz de rotación del billboard (mantiene posición jerárquica)
			billboardRotation = glm::mat4(1.0f);
			billboardRotation[0] = glm::vec4(right_billboard, 0.0f);
			billboardRotation[1] = glm::vec4(up_billboard, 0.0f);
			billboardRotation[2] = glm::vec4(toCamera, 0.0f);
			
			// Aplicar rotación billboard (sin perder la posición jerárquica)
			savedPosition = glm::vec3(model[3]);
			model = glm::mat4(1.0f);
			model = glm::translate(model, savedPosition);
			model = model * billboardRotation;
			
			// Escalar según explosionScale (crece progresivamente con la tapa)
			currentScale = 25.0f * explosionScale;
			model = glm::scale(model, glm::vec3(currentScale, currentScale, 1.0f));
			
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			
			// Usar textura de explosión
			explosion.UseTexture();
			
			// Renderizar el billboard (mesh index 3 - el que agregamos en CreateObjects)
			meshList[3]->RenderMesh();
			
			glDisable(GL_BLEND);
		}

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-200.0f, 0.0f, -50.0f));
		model = AnimationOpa(model);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		opaopa.RenderModel();
		
		// ========== Animación jerárquica de caminata (Ace) ==========
		// Actualizar el ciclo de caminata
		UpdateWalkCycle();
		
		// ========== Renderizado con animación por keyframes (Bell) ==========
		bellAnim = g_AnimationManager.GetAnimation("Bell");
		if(bellAnim && bellAnim->IsPlaying()){
		   
			
			
			// Reproducir sonido cuando la campana comienza a rotar
			if (!bellSoundPlayed && fabs(rotateZ) > 1.0f) {
				ma_sound_set_looping(&s_bell,MA_TRUE);
				ma_sound_start(&s_bell);
				bellSoundPlayed = true;
			}
			
			model = modelboat;
			model = glm::translate(model, glm::vec3(31.174f, 84.0f, 10.257f));
			// Posicionar el sonido de bell ligado por jerarquía
			glm::vec3 bellWorldPos = glm::vec3(model[3]);
			ma_sound_set_position(&s_bell, bellWorldPos.x * AUDIO_SCALE, bellWorldPos.y * AUDIO_SCALE, bellWorldPos.z * AUDIO_SCALE);
			
			rotateZ = bellAnim->GetValue1();
			model = glm::rotate(model, glm::radians(rotateZ), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
			modelaux = model;
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			bell.RenderModel();

			model = modelaux;
			badajo_rot = bellAnim->GetValue2();
			model = glm::rotate(model, glm::radians(badajo_rot), glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			badajo.RenderModel();
		}
		else {
			// Resetear bandera cuando la animación no está activa
			bellSoundPlayed = false;
		}
		bellAnim = g_AnimationManager.GetAnimation("Ring_Bell");
		if(bellAnim && bellAnim->IsPlaying()){
		    model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(116.f, 4.0f, -80.0f));
			posCampana_z = bellAnim->GetValue1();
			rotCampana_x = bellAnim->GetValue2();
			rotPalanca_x = bellAnim->GetValue3();
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
			modelaux = model;
			
			// Posicionar el sonido de box_bell ligado por jerarquía
			glm::vec3 boxBellWorldPos = glm::vec3(model[3]);
			ma_sound_set_position(&s_box_bell, boxBellWorldPos.x * AUDIO_SCALE, boxBellWorldPos.y * AUDIO_SCALE, boxBellWorldPos.z * AUDIO_SCALE);
			
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			soporte_bell.RenderModel();

			// Campana - Traslación en Z y Rotación en X (balanceo)
			model = modelaux;
			
			// Reproducir sonido cuando la campana comienza a moverse
			/*if (!ringBellSoundPlayed && (fabs(posCampana_z) > 0.1f || fabs(rotCampana_x) > 0.5f)) {
				ma_sound_set_looping(&s_box_bell,MA_TRUE);
				ma_sound_start(&s_box_bell);
				ringBellSoundPlayed = true;
			}*/
		
			model = glm::translate(model, glm::vec3(0.0f,3.5f, posCampana_z));
			model = glm::rotate(model, rotCampana_x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			ring_bell.RenderModel();
			
			// Palanca - Rotación en X solamente
			model = modelaux; 
			model = glm::translate(model, glm::vec3(-0.65f, 2.56f, 0.0f));
			model = glm::rotate(model, rotPalanca_x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			palanca.RenderModel();

			
		}
		else {
			// Resetear bandera cuando la animación no está activa
			ringBellSoundPlayed = false;
		}

		bellAnim = g_AnimationManager.GetAnimation("Reloj");
		if(bellAnim && bellAnim->IsPlaying()){
		    model = glm::mat4(1.0f);
		    model = glm::translate(model, glm::vec3(640.59f, 15.00f, 466.5f));
			manecilla_hora = bellAnim->GetValue1();
			manecilla_minuto = bellAnim->GetValue2();
			model = glm::rotate(model, glm::radians(225.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
			modelaux = model;
			
			// Posicionar el sonido del reloj ligado por jerarquía
			glm::vec3 clockWorldPos = glm::vec3(modelaux[3]);
			ma_sound_set_position(&s_clock, clockWorldPos.x * AUDIO_SCALE, clockWorldPos.y * AUDIO_SCALE, clockWorldPos.z * AUDIO_SCALE);
			
			// Reproducir sonido cada vez que la manecilla del minuto cambia significativamente
			if (!clockSoundPlayed && fabs(manecilla_minuto - lastClockRotation) > 5.0f) {
				ma_sound_set_looping(&s_bell,MA_TRUE);
				ma_sound_start(&s_clock);
				clockSoundPlayed = true;
				lastClockRotation = manecilla_minuto;
			}
			// Resetear bandera después de un cambio grande
			if (fabs(manecilla_minuto - lastClockRotation) > 15.0f) {
				clockSoundPlayed = false;
			}
			
			//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Reloj_Crash.RenderModel();

			model = modelaux;
			model = glm::translate(model, glm::vec3(0.366549f, 0.751359f, -0.037627f));
			model = glm::rotate(model, manecilla_hora * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Reloj_Hora.RenderModel();

			model = modelaux;
			model = glm::translate(model, glm::vec3(0.362738, 0.775956, -0.021455f));
			model = glm::rotate(model, manecilla_minuto * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Reloj_Minuto.RenderModel();
			

			
		}
		else {
			// Resetear bandera cuando la animación no está activa
			clockSoundPlayed = false;
			lastClockRotation = 0.0f;
		}
		
		// (El else se eliminó para permitir loop infinito)

		// Posición base del personaje (ahora usa la variable dinámica)
		acePosition = characterPosition;
		
		// 1. Renderizar el cuerpo (raíz de la jerarquía)

		//Seleccionamos el material del personaje
		Material_personaje.UseMaterial(uniformSpecularIntensity, uniformShininess);

		model = glm::mat4(1.0);
		model = glm::translate(model, acePosition);
		model = glm::rotate(model, characterRotation, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model,glm::vec3(0.6f,0.6f,0.6f));
		model = AnimateBody(model);
		bodyModel = model; // Guardar la transformación del cuerpo
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ace["CuerpoAce"].RenderModel();
		
		// 2. Renderizar hombro derecho + brazo derecho (jerarquía: Cuerpo -> Hombro -> Brazo)
		model = bodyModel;
		model = glm::translate(model, glm::vec3(-7.9714f,20.5293f,-1.55739f));
		model = AnimateRightShoulder(model);
		rightShoulderModel = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ace["HombroDerechoAce"].RenderModel();
		
		model = AnimateRightArm(rightShoulderModel);
		model = glm::translate(model,glm::vec3(1.15075f,1.2403f,0.13481f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ace["BrazoDerechoAce"].RenderModel();
		
		// 3. Renderizar hombro izquierdo + brazo izquierdo (jerarquía: Cuerpo -> Hombro -> Brazo)
		model = bodyModel;
		model = glm::translate(model, glm::vec3(7.624f,20.6028f,-1.58114f));
		model = AnimateLeftShoulder(model);
		leftShoulderModel = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ace["HombroIzquierdoAce"].RenderModel();
		
		model = AnimateLeftArm(leftShoulderModel);
		model = glm::translate(model,glm::vec3(-0.47100118f,1.2567f,0.28935f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ace["BrazoIzquierdoAce"].RenderModel();
		
		// 4. Renderizar muslo derecho + pierna derecha (jerarquía: Cuerpo -> Muslo -> Pierna)
		model = bodyModel;
		model = glm::translate(model,glm::vec3(0.0f, 0.1138f, 0.0f));
		model = AnimateRightThigh(model);
		rightThighModel = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ace["MusloDerechoAce"].RenderModel();
		
		model = rightThighModel;
		model = glm::translate(model,glm::vec3(-3.35244f,-13.1455f,-0.441f));
		model = AnimateRightLeg(model);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ace["PiernaDerechaAce"].RenderModel();
		
		// 5. Renderizar muslo izquierdo + pierna izquierda (jerarquía: Cuerpo -> Muslo -> Pierna)
		model = bodyModel;
		model = glm::translate(model,glm::vec3(0.0f,0.1138f,0.0f));
		model = AnimateLeftThigh(model);
		leftThighModel = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ace["MusloIzquierdoAce"].RenderModel();
		
		model = leftThighModel;
		model = glm::translate(model, glm::vec3(+3.35244f, -13.1455f, -0.441f));
		model = AnimateLeftLeg(model);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ace["PiernaIzquierdaAce"].RenderModel();

		// Restaurar material opaco para los demás modelos
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		// ========== Fin animación jerárquica de caminata ==========

		
















































































		

		
		
		
		
	
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(8.55, 0.00, -12.67));
		modelaux=model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ring.RenderModel(); /*

		// Renderizar paredes del ring con texturas de máscaras
		mascaras.UseTexture();
		
		// Pared Norte (meshList[5])
		model = modelaux;
		model = glm::translate(model,glm::vec3(0.0f,1.5f,0.0f));
		offset = getMaskUVOffset(0);  // Pared 0
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(offset));
		meshList[5]->RenderMesh();
		
		// Pared Este (meshList[6])
		model = modelaux;
		model = glm::translate(model,glm::vec3(0.0f,1.5f,-22.7f));
		offset = getMaskUVOffset(1);  // Pared 1
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(offset));

		meshList[6]->RenderMesh();
		
		// Pared Sur (meshList[7])
		model = modelaux;
		model = glm::translate(model,glm::vec3(-0.8f,1.5f,0.0f));
		offset = getMaskUVOffset(2);  // Pared 2
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(offset));
		meshList[7]->RenderMesh();
		
		// Pared Oeste (meshList[8])
		model = modelaux;
		model = glm::translate(model,glm::vec3(0.0f,1.5f,22.7f));
		offset = getMaskUVOffset(3);  // Pared 3
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(offset));
		meshList[8]->RenderMesh();

		offset = glm::vec2(0.0f,0.0f);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(offset));
		
		// Renderizar contador de rounds - DÍGITO DE DECENAS
		firstDigit = roundCounter / 10;
		model = modelaux;
		model = glm::translate(model, glm::vec3(-96.7+195.f, 50.0f, -97.5f)); // Ajusta Y para visibilidad
		model = glm::rotate(model,glm::radians(90.0f),glm::vec3(0.0,1.0,0.0));
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
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f)); // Separación entre dígitos
		model = glm::scale(model, glm::vec3(2.0f, 4.0f, 1.0f));
		offset = getUVNumber(secondDigit);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(offset));
		numeros.UseTexture();
		meshList[4]->RenderMesh();*/

		offset = glm::vec2(0.0f,0.0f);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(offset));
		
		
		glUseProgram(0);

		mainWindow.swapBuffers();
	}
	//Limpia de variables
    unloadSounds();
    unloadSoundsGroups();
    ma_engine_uninit(&eng);
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
		-2.0f, -2.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		 2.0f, -2.0f, 0.0f,		0.2f, 0.0f,		0.0f, 0.0f, 1.0f,
		 2.0f,  2.0f, 0.0f,		0.2f, 0.5f,		0.0f, 0.0f, 1.0f,
		-2.0f,  2.0f, 0.0f,		0.0f, 0.5f,		0.0f, 0.0f, 1.0f
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
	
	float wallWidth = 125.0f;   // Ancho de cada pared
	float wallHeight = 20.0f;  // Altura de cada pared
	float ringRadius = 100.0f;  // Distancia desde el centro del ring
	
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

