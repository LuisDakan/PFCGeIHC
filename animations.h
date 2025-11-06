#pragma once
#ifndef ANIMATIONS_H

#define ANIMATIONS_H

#include <glm.hpp>

#include <glm.hpp>

// Constantes de animación

extern const float A;// Declaraciones de funciones de animación

extern const float B;glm::mat4 AnimationShip(glm::mat4 model);

extern const float C;glm::mat4 AnimationRing(glm::mat4 model, int id);

extern const int numberRings;glm::mat4 AnimationTNT(glm::mat4 model);

extern const float phaseRing;glm::mat4 AnimationTapa(glm::mat4 model);

extern const float heightR;void StartTNTAnimation();



// Variables globales de animación// Variables externas

extern float WShip;extern float explosionScale;

extern float WRing;extern bool TNTExploded;

extern float R;

extern double TNTStartTime;

extern bool TNTExploded;
extern float explosionScale;
extern bool TNTAnimationActive;
extern bool TNTAnimationComplete;

// Declaraciones de funciones de animación
glm::mat4 AnimationShip(glm::mat4 model);
glm::mat4 AnimationRing(glm::mat4 model, int id);
glm::mat4 AnimationTNT(glm::mat4 model);
void StartTNTAnimation();
glm::mat4 AnimationTapa(glm::mat4 model);

#endif