
#include "Window.h"

#include <algorithm>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "TorchSystem.h"

// Variables globales para comunicación de eventos
extern bool g_addModelRequest;
extern glm::vec3 g_pendingModelPos;
extern bool g_removeModelRequest;
extern glm::vec3 g_removeModelPos;
extern glm::mat4 projection;
#include "Camera.h"
extern Camera camera;

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
	articulacion1 = 0.0f;
	articulacion2 = 0.0f;
	articulacion3 = 0.0f;
	articulacion4 = 0.0f;
	articulacion5 = 0.0f;
	articulacion6 = 0.0f;
	articulacion7 = 0.0f;
	articulacion8 = 0.0f;
	articulacion9 = 0.0f;
	articulacion10 = 0.0f;
	prenderspot=false;
	apagadospot=false;
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

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		ScaleLastModel(0.1f);
	}
	if (key == GLFW_KEY_U && action == GLFW_PRESS)
	{
		ScaleLastModel(-0.1f);
	}
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		g_removeModelRequest = true;
		g_removeModelPos = camera.getCameraPosition();
	}
	if (key == GLFW_KEY_P && action == GLFW_RELEASE)
	{
		// Eliminar el último modelo colocado (como deshacer)
		RemoveLastModelInstance();
	}
	if (key == GLFW_KEY_F)
	{
		theWindow->articulacion1 += 9.0f;
		//printf("Valor:%f\n", theWindow->articulacion1);
	}

	if (key == GLFW_KEY_G)
	{
		theWindow->articulacion1 -= 9.0f;
		//printf("Valor:%f\n", theWindow->articulacion1);
	}
	if (key == GLFW_KEY_H)
	{

		theWindow->articulacion2 += 9.0;
	}
	if (key == GLFW_KEY_J)
	{

		theWindow->articulacion2 -= 9.0;
	}
	if (key == GLFW_KEY_K)
	{

		theWindow->articulacion3 += 9.0;
	}
	if (key == GLFW_KEY_L)
	{
		theWindow->articulacion3 -= 10.0;
	}
	if (key == GLFW_KEY_Z) {
		theWindow->articulacion4 += 9.0;
	}
	if (key == GLFW_KEY_X) {
		theWindow->articulacion4 -= 9.0;
	}
	if (key == GLFW_KEY_C) {
		theWindow->articulacion5 += 9.0;
		theWindow->articulacion5 = std::min(theWindow->articulacion5, 45.0f);
	}
	if (key == GLFW_KEY_V) {
		theWindow->articulacion5 -= 9.0;
		theWindow->articulacion5 = std::max(theWindow->articulacion5, 0.0f);
	}

	if (key == GLFW_KEY_B && action == GLFW_RELEASE) {
		theWindow->antorch = !theWindow->antorch;
	}

	if (key == GLFW_KEY_N && action == GLFW_RELEASE) {
		theWindow->raygun = !theWindow->raygun;
	}

	if (key == GLFW_KEY_M && action == GLFW_RELEASE) {
		theWindow->isOn=!theWindow->isOn;
	}
	if (key == GLFW_KEY_R && action == GLFW_RELEASE) {
		theWindow->prenderspot = !theWindow->prenderspot;

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

// Callback para clicks de mouse
void Window::ManejaMouseClick(GLFWwindow* window, int button, int action, int mods)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
		double xpos = win->lastMouseX;
		double ypos = win->lastMouseY;
		int width = (int)win->getBufferWidth();
		int height = (int)win->getBufferHeight();
		float x = (2.0f * float(xpos)) / width - 1.0f;
		float y = 1.0f - (2.0f * float(ypos)) / height;
		glm::vec4 ray_clip = glm::vec4(x, y, -1.0f, 1.0f);
	glm::mat4 invProj = glm::inverse(projection);
	glm::vec4 ray_eye = invProj * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);
	glm::mat4 viewMat = camera.calculateViewMatrix();
	glm::mat4 invView = glm::inverse(viewMat);
	glm::vec4 ray_wor = invView * ray_eye;
		glm::vec3 ray_dir = glm::normalize(glm::vec3(ray_wor));
		glm::vec3 cam_pos = camera.getCameraPosition();
		float t = -cam_pos.y / ray_dir.y;
		glm::vec3 worldPos = cam_pos + t * ray_dir;
		g_addModelRequest = true;
		g_pendingModelPos = worldPos;
	}
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
