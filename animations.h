#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <glm.hpp>

// Declaraciones de funciones de animación
glm::mat4 AnimationShip(glm::mat4 model);
glm::mat4 AnimationRing(glm::mat4 model, int id);
glm::mat4 AnimationTNT(glm::mat4 model);
glm::mat4 AnimationTapa(glm::mat4 model);
void StartTNTAnimation();

// Variables externas
extern float explosionScale;
extern bool TNTExploded;

#endif
