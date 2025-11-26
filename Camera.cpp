#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed*10;
	turnSpeed = startTurnSpeed*2;

	// Inicializar variables de cámara tercera persona
	thirdPersonDistance = 100.0f; // Aumentada de 50.0f a 100.0f
	thirdPersonHeight = 40.0f;     // Aumentada de 25.0f a 40.0f
	thirdPersonAngle = 0.0f;

	// Inicializar variables de cámara aérea
	aerialHeight = 400.0f; // Aumentada de 200.0f a 400.0f

	// Inicializar puntos de interés
	currentPOIIndex = 0;

	// Modo por defecto
	cameraMode = FREE_CAMERA;

	update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;

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
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
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

// Cámara tercera persona (Opción 3)
void Camera::updateThirdPerson(glm::vec3 targetPosition, GLfloat& targetRotation, bool isMoving, GLfloat deltaTime)
{
	// Si el personaje se está moviendo, rotar hacia la dirección de la cámara
	if (isMoving)
	{
		// Calcular la dirección horizontal de la cámara (proyección en el plano XZ)
		glm::vec3 cameraForward = front;
		cameraForward.y = 0.0f;
		cameraForward = glm::normalize(cameraForward);
		
		// Calcular el ángulo usando atan2 para que el eje +Z del personaje apunte hacia donde mira la cámara
		// Se resta 90 grados (-PI/2) para ajustar la orientación del modelo
		targetRotation = glm::atan(cameraForward.x, -cameraForward.z) - glm::half_pi<float>();
	}

	// Calcular la posición de la cámara detrás del personaje
	GLfloat camX = targetPosition.x - thirdPersonDistance * sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	GLfloat camY = targetPosition.y + thirdPersonHeight + thirdPersonDistance * sin(glm::radians(pitch));
	GLfloat camZ = targetPosition.z - thirdPersonDistance * cos(glm::radians(yaw)) * cos(glm::radians(pitch));

	position = glm::vec3(camX, camY, camZ);

	// Hacer que la cámara mire al personaje
	glm::vec3 targetLookAt = targetPosition + glm::vec3(0.0f, 15.0f, 0.0f);
	front = glm::normalize(targetLookAt - position);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

// Cámara aérea libre
void Camera::updateAerial(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;

	// Movimiento en el plano XZ
	glm::vec3 forwardXZ = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
	glm::vec3 rightXZ = glm::normalize(glm::vec3(right.x, 0.0f, right.z));

	if (keys[GLFW_KEY_W])
	{
		position += forwardXZ * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		position -= forwardXZ * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		position -= rightXZ * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		position += rightXZ * velocity;
	}

	// Mantener altura constante
	position.y = aerialHeight;
}

// Agregar punto de interés
void Camera::addPointOfInterest(glm::vec3 position, glm::vec3 lookAt, std::string name)
{
	PointOfInterest poi;
	poi.position = position;
	poi.lookAt = lookAt;
	poi.name = name;
	pointsOfInterest.push_back(poi);
}

// Siguiente punto de interés
void Camera::nextPointOfInterest()
{
	if (pointsOfInterest.size() > 0)
	{
		currentPOIIndex = (currentPOIIndex + 1) % pointsOfInterest.size();
		updatePointOfInterest();
	}
}

// Punto de interés anterior
void Camera::previousPointOfInterest()
{
	if (pointsOfInterest.size() > 0)
	{
		currentPOIIndex--;
		if (currentPOIIndex < 0)
		{
			currentPOIIndex = pointsOfInterest.size() - 1;
		}
		updatePointOfInterest();
	}
}

// Actualizar cámara en punto de interés actual
void Camera::updatePointOfInterest()
{
	if (pointsOfInterest.size() > 0 && currentPOIIndex >= 0 && currentPOIIndex < pointsOfInterest.size())
	{
		position = pointsOfInterest[currentPOIIndex].position;
		glm::vec3 lookAt = pointsOfInterest[currentPOIIndex].lookAt;
		
		front = glm::normalize(lookAt - position);
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}
}

// Cambiar modo de cámara
void Camera::setCameraMode(CameraMode mode)
{
	cameraMode = mode;
	
	if (mode == POINT_OF_INTEREST)
	{
		updatePointOfInterest();
	}
}

Camera::~Camera()
{
}
