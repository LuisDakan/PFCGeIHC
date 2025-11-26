#pragma once

#include <string>
#include <vector>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

enum CameraMode {
	FREE_CAMERA,     // Cámara libre original (modo por defecto)
	THIRD_PERSON,    // Cámara tercera persona siguiendo al personaje
	AERIAL_FREE,     // Cámara aérea libre
	POINT_OF_INTEREST // Cámara de puntos de interés
};

struct PointOfInterest {
	glm::vec3 position;
	glm::vec3 lookAt;
	std::string name;
};

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);

	// Control de cámara tercera persona
	void updateThirdPerson(glm::vec3 targetPosition, GLfloat& targetRotation, bool isMoving, GLfloat deltaTime);
	
	// Control de cámara aérea
	void updateAerial(bool* keys, GLfloat deltaTime);
	
	// Control de puntos de interés
	void addPointOfInterest(glm::vec3 position, glm::vec3 lookAt, std::string name);
	void nextPointOfInterest();
	void previousPointOfInterest();
	void updatePointOfInterest();

	// Cambiar modo de cámara
	void setCameraMode(CameraMode mode);
	CameraMode getCameraMode() { return cameraMode; }

	glm::vec3 getCameraPosition();
	glm::vec3 getCameraDirection();
	glm::mat4 calculateViewMatrix();

	~Camera();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	// Variables para cámara tercera persona
	GLfloat thirdPersonDistance;
	GLfloat thirdPersonHeight;
	GLfloat thirdPersonAngle;

	// Variables para cámara aérea
	GLfloat aerialHeight;

	// Variables para puntos de interés
	std::vector<PointOfInterest> pointsOfInterest;
	int currentPOIIndex;

	CameraMode cameraMode;

	void update();
};

