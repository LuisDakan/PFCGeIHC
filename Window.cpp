
#include "Window.h"
#include "animations.h"

#include <algorithm>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>


// Variables globales para comunicación de eventos
extern bool g_addModelRequest;
extern glm::vec3 g_pendingModelPos;
extern bool g_removeModelRequest;
extern glm::vec3 g_removeModelPos;
extern glm::mat4 projection;
extern int roundCounter;  // Contador de rounds
extern int maskRotation;  // Rotación de texturas de máscaras


Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevex = 2.0f;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicializaci�n de GLFW
	if (!glfwInit())
	{
		printf("Fall� inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	isOn = true;
	front = true;
	antorch = false;
	raygun = false;
	prenderspot=false;
	prenderspot2=false;
	muevex=0.0f;
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "PracticaXX:Nombre de la practica", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tama�o de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fall� inicializaci�n de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se est� usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
	glfwSetMouseButtonCallback(mainWindow, ManejaMouseClick);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}




void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    	glfwSetWindowShouldClose(window, GL_TRUE);
	}
	
	if (key == GLFW_KEY_0)
	{
		theWindow->articulacion1 += 0.1f;
		theWindow->muevex+=0.1f;
		printf("art1:%f\n", theWindow->muevex);
	}

	if (key == GLFW_KEY_9)
	{
		theWindow->articulacion1 -= 0.1f;
		theWindow->muevex-=0.1f;
		printf("art1:%f\n", theWindow->muevex);
	}
	if (key == GLFW_KEY_3)
	{

		theWindow->articulacion2 += 0.1;
		printf("art2:%f\n", theWindow->articulacion2);
	}
	if (key == GLFW_KEY_4)
	{

		theWindow->articulacion2 -= 0.1;
		printf("art2:%f\n", theWindow->articulacion2);
	}
	if (key == GLFW_KEY_5)
	{
		printf("art3:%f\n", theWindow->articulacion3);
		theWindow->articulacion3 += 0.1;
	}
	if (key == GLFW_KEY_6)
	{
		printf("art3:%f\n", theWindow->articulacion3);
		theWindow->articulacion3 -= 0.1;
	}
	if (key == GLFW_KEY_Z) {
		theWindow->articulacion4 += 1.0;
	}
	if (key == GLFW_KEY_X) {
		theWindow->articulacion4 -= 1.0;
	}
	
	
	// Control del contador de rounds (tecla Q para incrementar)
	if (key == GLFW_KEY_Q && action == GLFW_RELEASE) {
		printf("Presionado\n");
		roundCounter=(roundCounter+1)%15;
	}
	
	// Tecla W para rotar texturas de máscaras en las paredes del ring
	if (key == GLFW_KEY_P && action == GLFW_RELEASE) {
		maskRotation = (maskRotation + 1) % 4;  // Ciclar entre 0-3
		printf("Rotacion de mascaras: %d\n", maskRotation);
	}

	if (key == GLFW_KEY_T && action == GLFW_RELEASE) {
		StartTNTAnimation();
	}
 
	if (key == GLFW_KEY_R && action == GLFW_RELEASE) {
		theWindow->prenderspot = !theWindow->prenderspot;
	}
	if (key == GLFW_KEY_P && action == GLFW_RELEASE) {
		theWindow->prenderspot2 = !theWindow->prenderspot2;
	}
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
	// Guardar posición global del mouse
	theWindow->lastMouseX = xPos;
	theWindow->lastMouseY = yPos;
}

void Window::ManejaMouseClick(GLFWwindow* window, int button, int action, int mods)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		// Obtener las coordenadas del mouse en pantalla
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		
		// Convertir coordenadas de pantalla (0 a width/height) a NDC (-1 a 1)
		float x = (2.0f * xpos) / theWindow->bufferWidth - 1.0f;
		float y = 1.0f - (2.0f * ypos) / theWindow->bufferHeight;
		
		// Leer profundidad del buffer en esa posición
		float depth;
		glReadPixels((int)xpos, theWindow->bufferHeight - (int)ypos, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
		
		// Si depth es 1.0, no hay geometría en ese pixel (fondo)
		if (depth >= 0.9999f) {
			printf("Click en el fondo (sin geometria)\n");
			return;
		}
		
		// Convertir depth de [0,1] a NDC [-1,1]
		float z = depth * 2.0f - 1.0f;
		
		// Crear vector en espacio de clip
		glm::vec4 clipCoords = glm::vec4(x, y, z, 1.0f);
		
		// Obtener matrices de vista y proyección (necesitas exportarlas desde main)
		extern glm::mat4 projection;
		extern glm::mat4 view;
		
		// Calcular matriz inversa
		glm::mat4 invProjView = glm::inverse(projection * view);
		
		// Transformar a espacio de mundo
		glm::vec4 worldCoords = invProjView * clipCoords;
		worldCoords /= worldCoords.w; // División de perspectiva
		
		printf("Click en: (%.2f, %.2f, %.2f)\n", worldCoords.x, worldCoords.y, worldCoords.z);
	}
}



Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
