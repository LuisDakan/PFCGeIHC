#pragma once
#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <glm.hpp>
#include "KeyframeAnimation.h"
#include "miniaudio.h"
// Constantes de animación
extern const float A;
extern const float B;
extern const float C;
extern const int numberRings;
extern const float phaseRing;
extern const float heightR;
extern const float maxOpaRadio;

// Variables globales de animación
extern float WShip;
extern float WRing;
extern float R;
extern double TNTStartTime;
extern bool TNTExploded;
extern float explosionScale;
extern bool TNTAnimationActive;
extern bool TNTAnimationComplete;

// Variables para Opa-Opa
extern float opaRadio;
extern float maxHigh;
extern bool animationOpa;
extern float velocity;
extern float curHigh;
extern float curWalk;
extern float maxWalk;
extern int stateOpa;
extern glm::vec3 lastSpiralPos;
extern float lastSpiralAngle;
extern float lastAngleOpa;
extern double pauseStartTime;
extern glm::vec3 lastCurvePos;
extern float lastCurveAngle;

// Variables para animación de caminata
extern bool walkingActive;
extern float walkCycle;
extern float walkSpeed;

// Variables para animación de salto con inercia
extern float jumpCharge;
extern float jumpVelocity;
extern float jumpHeight;
extern bool isJumping;

// Declaraciones de funciones de animación
glm::mat4 AnimationShip(glm::mat4 model);
glm::mat4 AnimationRing(glm::mat4 model, int id);
glm::mat4 AnimationTNT(glm::mat4 model);
void StartTNTAnimation();
glm::mat4 AnimationTapa(glm::mat4 model,ma_sound &s);
glm::mat4 AnimationOpa(glm::mat4 model);
void StartOpaAnimation();


// Declaraciones de funciones de animación de caminata
void ToggleWalking();
void UpdateWalkCycle();
void SetWalkingActive(bool active);
bool IsWalkingActive();
glm::mat4 AnimateBody(glm::mat4 model);
glm::mat4 AnimateRightShoulder(glm::mat4 parentModel);
glm::mat4 AnimateRightArm(glm::mat4 parentModel);
glm::mat4 AnimateLeftShoulder(glm::mat4 parentModel);
glm::mat4 AnimateLeftArm(glm::mat4 parentModel);
glm::mat4 AnimateRightThigh(glm::mat4 parentModel);
glm::mat4 AnimateRightLeg(glm::mat4 parentModel);
glm::mat4 AnimateLeftThigh(glm::mat4 parentModel);
glm::mat4 AnimateLeftLeg(glm::mat4 parentModel);
glm::mat4 AnimateRing(glm::mat4 model);

// Declaraciones de funciones de salto
glm::mat4 AnimateJump(glm::mat4 model, bool isKeyPressed);
float GetJumpCharge();

// Manager global de animaciones por keyframes
extern KeyframeAnimationManager g_AnimationManager;

// Funciones para inicializar y controlar animaciones por keyframes
void InitKeyframeAnimations();
void UpdateKeyframeAnimations();

#endif