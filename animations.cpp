//Archivo para guardar las animaciones
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "Model.h"

const float A = 300.0f;
const float B = 200.0f;
const float C = 50.0f;
const int numberRings = 10;
const float phaseRing = 360.0f / (float)numberRings;
const float heightR = 25.0f;

float WShip = 0.0f;
float WRing = 0.0f;
float R=50.0f;
double TNTStartTime = -1.0;  // Tiempo de inicio de animación TNT
bool TNTExploded = false;  // Estado de la explosión
float explosionScale = 0.1f;  // Escala de la explosión (0.0 a 1.0)
bool TNTAnimationActive = false;  // Indica si la animación está activa
bool TNTAnimationComplete = false;  // Indica si la animación terminó completamente

// Funciones de animación

glm::mat4 AnimationShip(glm::mat4 model)
{
    WShip += 0.1f;
    model=glm::translate(model,glm::vec3(A*glm::cos(glm::radians(WShip)),C*(1+glm::sin(glm::radians(WShip))),  B*sin(glm::radians(WShip))));
    model=glm::rotate(model,glm::radians(WShip+90.0f),glm::vec3(0.0f,-1.0f,0.0f));
    // Efecto de ola: balanceo lateral (roll - eje Z)
    float rollAngle = 5.0f * glm::sin(glm::radians(WShip * 2.0f)); // ±5 grados
    model = glm::rotate(model, glm::radians(rollAngle), glm::vec3(0.0f, 0.0f, 1.0f));

    // Efecto de ola: cabeceo (pitch - eje X)
    float pitchAngle = 3.0f * glm::sin(glm::radians(WShip * 3.0f)); // ±3 grados
    model = glm::rotate(model, glm::radians(pitchAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    return model;
}

glm::mat4 AnimationRing(glm::mat4 model,int id)
{
    WRing +=0.1f;
   
    model = glm::translate(model, glm::vec3(
        R * glm::cos(glm::radians(phaseRing * (float)id)),
        heightR*(1+glm::sin(phaseRing*(float)id)),
        R * glm::sin(glm::radians(phaseRing * (float)id))
    ));

    
    model = glm::translate(model, glm::vec3(
        R * glm::cos(glm::radians(WRing)),
        heightR*(1+glm::sin(WRing)),
        R * glm::sin(glm::radians(WRing))
    ));

    model = glm::rotate(model,glm::radians(WRing),glm::vec3(0.0f,-1.0f,0.0f));

    return model;
}



// Animación de TNT: tambaleo usando ruido simulado con senos/cosinos desfasados
glm::mat4 AnimationTNT(glm::mat4 model)
{
    // Si la animación no está activa, simplemente retornar sin animar
    if (!TNTAnimationActive) {
        return model;
    }
    
    // Inicializar tiempo de inicio en la primera llamada después de activarse
    if (TNTStartTime < 0.0) {
        TNTStartTime = glfwGetTime();
        TNTAnimationComplete = false;
    }
    
    // Calcular tiempo transcurrido en segundos
    double currentTime = glfwGetTime();
    double elapsedTime = currentTime - TNTStartTime;
    
    // Tambalear durante los primeros 7 segundos
    if (elapsedTime <= 7.0) {
        // Convertir tiempo a grados para las funciones trigonométricas
        float timeAngle = (float)(elapsedTime * 60.0f); // Simular 60 grados por segundo
        
        // Simular ruido con múltiples frecuencias de rotación
        float wobbleX = 3.0f * glm::sin(glm::radians(timeAngle * 2.3f)) + 
                        1.5f * glm::sin(glm::radians(timeAngle * 5.7f));
        
        float wobbleZ = 3.0f * glm::cos(glm::radians(timeAngle * 1.9f)) + 
                        1.5f * glm::cos(glm::radians(timeAngle * 4.3f));
        
        // Aplicar rotaciones de tambaleo
        model = glm::rotate(model, glm::radians(wobbleX), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(wobbleZ), glm::vec3(0.0f, 0.0f, 1.0f));
    }
    else {
        // Marcar como explotada después de 7 segundos
        if (!TNTExploded) {
            TNTExploded = true;
        }
    }
    
    return model;
}

// Función para iniciar/reiniciar la animación de TNT
void StartTNTAnimation()
{
    // Solo permitir iniciar si no hay animación en progreso o si la anterior ya terminó
    if (!TNTAnimationActive || TNTAnimationComplete) {
        TNTAnimationActive = true;
        TNTStartTime = -1.0;  // Reiniciar tiempo (se inicializará en la primera llamada)
        TNTExploded = false;
        explosionScale = 0.0f;
        TNTAnimationComplete = false;
        printf("Animacion TNT iniciada!\n");
    }
    else {
        printf("Animacion TNT en progreso, espera a que termine.\n");
    }
}
 
 
// Animación de la tapa: elevación desde el segundo 4 y regreso
glm::mat4 AnimationTapa(glm::mat4 model)
{
    // Solo calcular si la animación está activa y ya se inició
    if (TNTAnimationActive && TNTStartTime >= 0.0) {
        double currentTime = glfwGetTime();
        double elapsedTime = currentTime - TNTStartTime;
        
        // La tapa comienza a subir en el segundo 4
        if (elapsedTime >= 4.0) {
            // Calcular tiempo de elevación desde el segundo 4
            float tiempoElevacion = (float)(elapsedTime - 4.0);
            
            // Duración total del ciclo: 3 segundos (1.5s subir + 1.5s bajar)
            float duracionCiclo = 3.0f;
            float alturaMaxima = 20.0f;
            
            float elevacion = 0.0f;
            
            if (tiempoElevacion <= duracionCiclo) {
                // Usar una función sinusoidal para subir y bajar suavemente
                // sin(0 a PI) va de 0 -> 1 -> 0
                float progreso = tiempoElevacion / duracionCiclo; // 0 a 1
                elevacion = alturaMaxima * glm::sin(progreso * 3.14159265f); // 0 -> max -> 0
                
                // Calcular escala de explosión sincronizada con la elevación
                // Explosión crece desde 0.1 (muy pequeña) hasta 1.0 (tamaño completo)
                explosionScale = 0.1f + 0.9f * glm::sin(progreso * 3.14159265f);
            }
            else {
                // Después del ciclo, la explosión permanece a tamaño completo
                explosionScale = 0.1f;
                
                // Marcar animación como completa para poder reiniciarla
                if (!TNTAnimationComplete) {
                    TNTAnimationComplete = true;
                }
            }
            
            model = glm::translate(model, glm::vec3(0.0f, elevacion, 0.0f));
        }
        else {
            // Antes de que comience a subir, explosión no visible
            explosionScale = 0.0f;
        }
    }
    
    return model;
}

