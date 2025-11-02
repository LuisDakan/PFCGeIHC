#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

    // Inicializar modos
    modoAereo = false;
    modoTerceraPersona = false;
    alturaFija = 5.0f;

	update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;

	// ← AGREGAR ESTE BLOQUE COMPLETO
	if (modoTerceraPersona)
	{
		// MODO 3ERA PERSONA: movimiento en plano XZ
		if (keys[GLFW_KEY_W])
		{
			position.z -= velocity;
		}
		if (keys[GLFW_KEY_S])
		{
			position.z += velocity;
		}
		if (keys[GLFW_KEY_A])
		{
			position.x -= velocity;
		}
		if (keys[GLFW_KEY_D])
		{
			position.x += velocity;
		}
		// Mantener altura constante
		position.y = 5.0f;
	}
	else if (modoAereo)
	{
		// MODO AÉREO: movimiento en plano XZ
		if (keys[GLFW_KEY_W])
		{
			position.z -= velocity;
		}
		if (keys[GLFW_KEY_S])
		{
			position.z += velocity;
		}
		if (keys[GLFW_KEY_A])
		{
			position.x -= velocity;
		}
		if (keys[GLFW_KEY_D])
		{
			position.x += velocity;
		}
	}
	else
	{
		// MODO NORMAL (tu código original)
		if (keys[GLFW_KEY_W])
		{
			position += front * velocity;
		}
		if (keys[GLFW_KEY_S])
		{
			position -= front * velocity;
		}
		if (keys[GLFW_KEY_A])
		{
			position -= right * velocity;
		}
		if (keys[GLFW_KEY_D])
		{
			position += right * velocity;
		}
		if (keys[GLFW_KEY_Q])
		{
			position += up * velocity;
		}
	}
       
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
    // Deshabilitar mouse en modos especiales
    if (modoAereo || modoTerceraPersona) return;
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update();
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}


glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(front);
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}


// ← AGREGAR ESTAS 2 FUNCIONES NUEVAS
void Camera::activarModoAereo(bool activar)
{
	modoAereo = activar;
	modoTerceraPersona = false;

	if (activar)
	{
		pitch = -89.0f;
		yaw = -90.0f;
		update();
	}
}

void Camera::activarModoTerceraPersona(bool activar)
{
	modoTerceraPersona = activar;
	modoAereo = false;

	if (activar)
	{
		pitch = -30.0f;
		yaw = -90.0f;
		position.y = 5.0f;
		update();
	}
}

Camera::~Camera()
{
}