#pragma once
#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <glm.hpp>

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

// Declaraciones de funciones de animación
glm::mat4 AnimationShip(glm::mat4 model);
glm::mat4 AnimationRing(glm::mat4 model, int id);
glm::mat4 AnimationTNT(glm::mat4 model);
void StartTNTAnimation();
glm::mat4 AnimationTapa(glm::mat4 model);
glm::mat4 AnimationOpa(glm::mat4 model);
void StartOpaAnimation();

#endif