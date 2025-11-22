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

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
Model piso;
Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;

bool day=true;
float sunAngle = 0.0f;
float sunSpeed = 0.3f;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
std::vector<Mesh*> lamps;

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

void setDay(std::vector<std::string> skyboxDay){
	skybox = Skybox(skyboxDay);
	// Luz direccional blanca, intensidad moderada
	mainLight = DirectionalLight(
		1.0f, 1.0f, 1.0f,   // Blanco
		0.5f, 0.5f,         // Intensidad moderada
		0.0f, 0.0f, -1.0f   // Dirección
	);
}
// Variable para trackear el skybox actual
// ============= FUNCIÓN SIMPLE PARA ACTUALIZAR EL CICLO =============
void updateSimpleDayNight(float deltaTime) {
	// Incrementar el ángulo del sol
	sunAngle += sunSpeed * deltaTime;

	// Mantener el ángulo entre 0 y 360
	if (sunAngle >= 360.0f) {
		sunAngle -= 360.0f;
	}

	// Convertir a radianes para los cálculos
	float angleRad = sunAngle * 3.14159265f / 180.0f;

	// Calcular dirección de la luz en el plano YZ
	float dirY = sin(angleRad);  // Componente Y (altura)
	float dirZ = cos(angleRad);  // Componente Z (profundidad)

	// Calcular intensidad basada en la altura del sol
	// Cuando dirY > 0 = día (sol arriba)
	// Cuando dirY < 0 = noche (sol abajo)
	float intensity = glm::max(0.0f, dirY);  // Solo positivo cuando está arriba

	// Intensidades para día y noche
	float ambientIntensity = 0.15f + (intensity * 0.35f);  // De 0.15 a 0.5
	float diffuseIntensity = 0.2f + (intensity * 0.6f);    // De 0.2 a 0.8

	// Color de la luz (blanco en el día, azulado en la noche)
	float colorR = 0.2f + (intensity * 0.8f);  // De 0.2 a 1.0
	float colorG = 0.3f + (intensity * 0.7f);  // De 0.3 a 1.0
	float colorB = 0.6f + (intensity * 0.4f);  // De 0.6 a 1.0

	// Actualizar la luz direccional
	mainLight = DirectionalLight(
		colorR, colorG, colorB,
		ambientIntensity,
		diffuseIntensity,
		0.0f, dirY, dirZ  // Dirección en plano YZ
	);

	// Cambiar skybox según la posición del sol
	static bool wasDay = true;
	bool isDay = (dirY > 0.0f);

	// Solo cambiar skybox cuando cruza el horizonte
	if (isDay != wasDay) {
		wasDay = isDay;
		if (isDay) {
			printf("Amanecer - Ángulo: %.1f grados\n", sunAngle);
		}
		else {
			printf("Atardecer - Ángulo: %.1f grados\n", sunAngle);
		}
	}
}




int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	piso = Model();
	piso.LoadModel("Models/piso.obj");


	//Cycle day
	// casi noche
		//atardecer
	std::vector<std::string> skyboxAlmostNight;
	skyboxAlmostNight.push_back("Textures/Skybox/sh_rt_atardecer.jpg");
	skyboxAlmostNight.push_back("Textures/Skybox/sh_lf_atardecer.jpg");
	skyboxAlmostNight.push_back("Textures/Skybox/sh_dn_atardecer.jpg");
	skyboxAlmostNight.push_back("Textures/Skybox/sh_up_atardecer.jpg");
	skyboxAlmostNight.push_back("Textures/Skybox/sh_bk_atardecer.jpg");
	skyboxAlmostNight.push_back("Textures/Skybox/sh_ft_atardecer.jpg");
	//noche
	std::vector<std::string> skyboxNight;
	skyboxNight.push_back("Textures/Skybox/sh_rt_noche.jpg");
	skyboxNight.push_back("Textures/Skybox/sh_lf_noche.jpg");
	skyboxNight.push_back("Textures/Skybox/sh_dn_noche.jpg");
	skyboxNight.push_back("Textures/Skybox/sh_up_noche.jpg");
	skyboxNight.push_back("Textures/Skybox/sh_bk_noche.jpg");
	skyboxNight.push_back("Textures/Skybox/sh_ft_noche.jpg");

	//atardecer
	std::vector<std::string> skyboxSunset;
	skyboxSunset.push_back("Textures/Skybox/sh_rt_atardecer.jpg");
	skyboxSunset.push_back("Textures/Skybox/sh_lf_atardecer.jpg");
	skyboxSunset.push_back("Textures/Skybox/sh_dn_atardecer.jpg");
	skyboxSunset.push_back("Textures/Skybox/sh_up_atardecer.jpg");
	skyboxSunset.push_back("Textures/Skybox/sh_bk_atardecer.jpg");
	skyboxSunset.push_back("Textures/Skybox/sh_ft_atardecer.jpg");

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
	spots[spotLightCount] = "linterna";
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




	int currentSkyboxIndex = -1;  // Inicializar en -1 para forzar primer cambio

	int newSkyboxIndex;

	int idx;
	//se crean mas luces puntuales y spotlight 
	//float lastSwitchTime = 0.0f;
	//float switchInterval = 30.0f;
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::vec3 p;
	glm::vec4 rot;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

	
		// Actualizar ciclo día/noche
		updateSimpleDayNight(deltaTime);


		newSkyboxIndex=0;

		if (sunAngle >= 0.0f && sunAngle < 45.0f) {
			newSkyboxIndex = 2; // Amanecer - casi noche
		}
		else if (sunAngle >= 45.0f && sunAngle < 120.0f) {
			newSkyboxIndex = 0; // Día
		}
		else if (sunAngle >= 120.0f && sunAngle < 180.0f) {
			newSkyboxIndex = 1; // Atardecer
		}
		else if (sunAngle >= 180.0f && sunAngle < 240.0f) {
			newSkyboxIndex = 3; // Crepúsculo - casi noche
		}
		else if (sunAngle >= 240.0f && sunAngle < 320.0f) {
			newSkyboxIndex = 3; // Noche
		}
		else {
			newSkyboxIndex = 2; // Pre-amanecer - casi noche
		}

		// Cambiar skybox solo si es diferente
		if (newSkyboxIndex != currentSkyboxIndex) {
			currentSkyboxIndex = newSkyboxIndex;

			if (currentSkyboxIndex == 0) {
				skybox = Skybox(skyboxDay);
				printf("DÍA - Ángulo: %.1f°\n", sunAngle);
			}
			else if (currentSkyboxIndex == 1) {
				skybox = Skybox(skyboxSunset);
				printf("ATARDECER - Ángulo: % .1f°\n", sunAngle);
			}
			else if (currentSkyboxIndex == 2) {
				skybox = Skybox(skyboxAlmostNight);
				printf("CASI NOCHE - Ángulo: %.1f°\n", sunAngle);
			}
			else {
				skybox = Skybox(skyboxNight);
				printf("NOCHE - Ángulo: % .1f°\n", sunAngle);
			}
		}

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
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
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
		model = glm::translate(model, glm::vec3(0.0f, -100.0f, 0.0f));
		model = glm::scale(model,glm::vec3(0.1f,0.1,0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		piso.RenderModel();

		
		

		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
