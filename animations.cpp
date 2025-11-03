//Funciones y algoritmos para animar objetos

#include <glm.hpp>
float A = 300.0f;
float B = 200.0f;
float C =50.0f;
float W = 0.0f;

glm::mat4 AnimationShip(glm::mat4 model)
{
    W += 0.1f;
    model=glm::translate(model,glm::vec3(A*glm::cos(glm::radians(W)),C*(1+glm::sin(glm::radians(W))),  B*sin(glm::radians(W))));
    model=glm::rotate(model,glm::radians(W+90.0f),glm::vec3(0.0f,-1.0f,0.0f));
    // Efecto de ola: balanceo lateral (roll - eje Z)
    float rollAngle = 5.0f * glm::sin(glm::radians(W * 2.0f)); // ±5 grados
    model = glm::rotate(model, glm::radians(rollAngle), glm::vec3(0.0f, 0.0f, 1.0f));

    // Efecto de ola: cabeceo (pitch - eje X)
    float pitchAngle = 3.0f * glm::sin(glm::radians(W * 3.0f)); // ±3 grados
    model = glm::rotate(model, glm::radians(pitchAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    return model;
}