//Archivo para guardar las animaciones

#include <glew.h>
#include <glfw3.h>
#include "KeyframeAnimation.h"
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "miniaudio.h"
#include "Model.h"
#include "KeyframeAnimation.h"

const float A = 300.0f;
const float B = 200.0f;
const float C = 50.0f;
const int numberRings = 10;
const float phaseRing = 360.0f / (float)numberRings;
const float heightR = 25.0f;
const float maxOpaRadio= 20.0f;


float WShip = 0.0f;
float WRing = 0.0f;
float R=50.0f;
double TNTStartTime = -1.0;  // Tiempo de inicio de animación TNT
bool TNTExploded = false;  // Estado de la explosión
float explosionScale = 0.1f;  // Escala de la explosión (0.0 a 1.0)
bool TNTAnimationActive = false;  // Indica si la animación está activa
bool TNTAnimationComplete = false;  // Indica si la animación terminó completamente
float opaRadio = 0.0f;
float maxHigh=25.0f;
bool animationOpa=false;
float velocity=0.04;
float curHigh=0.0f;
float curWalk=0.0f;
float maxWalk=25.0f;
float angulovaria = 0.0f;
extern float deltaTime;
int stateOpa = 0;
// Variables para continuidad entre estados
glm::vec3 lastSpiralPos = glm::vec3(0.0f);
float lastSpiralAngle = 0.0f;
float lastAngleOpa = 0.0f;
double pauseStartTime = -1.0; // Tiempo de inicio de pausa entre estados
glm::vec3 lastCurvePos = glm::vec3(0.0f); // Posición final de la curva
float lastCurveAngle = 0.0f; // Ángulo final de la curva

// Variables para animación de caminata del humanoide
bool walkingActive = false;
float walkCycle = 0.0f;
float walkSpeed = 4.0f;

// Banderas de sonidos
bool TNT_explosion = false;

// Funciones de animación


glm::mat4 AnimationShip(glm::mat4 model)
{
    WShip += 0.2f * deltaTime;
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
glm::mat4 AnimationTapa(glm::mat4 model,ma_sound &s)
{
    // Solo calcular si la animación está activa y ya se inició
    if (TNTAnimationActive && TNTStartTime >= 0.0) {

        double currentTime = glfwGetTime();
        double elapsedTime = currentTime - TNTStartTime;
        
        // La tapa comienza a subir en el segundo 4
        if (elapsedTime >= 4.0) {
            if(!TNT_explosion){
                ma_sound_start(&s);
                TNT_explosion = true;
            }
                
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
                TNT_explosion = false;
                ma_sound_stop(&s);
                ma_sound_seek_to_pcm_frame(&s,0);
                // Marcar animación como completa para poder reiniciarla
                if (!TNTAnimationComplete) {
                    TNTAnimationComplete = true;
                }
            }
            
            model = glm::translate(model, glm::vec3(0.0f, elevacion, 0.0f));
        }
        else {
            // Antes de que comience a subir, explosión no visible
            TNT_explosion = false;
            explosionScale = 0.0f;
        }
    }
    
    return model;
}

glm::mat4 AnimationOpa(glm::mat4 model)
{
    // Factor de escala del modelo
    const float SCALE_FACTOR = 2.5f;

    // Altura mínima del ciclo (30% de maxHigh para evitar descenso excesivo)
    const float MIN_HEIGHT_RATIO = 0.3f;
    const float minCycleHeight = maxHigh * MIN_HEIGHT_RATIO;

    // La animación siempre está activa (ciclo continuo)
    switch (stateOpa)
    {
    case 0: // Estado 1: Elevarse con suavizado (desde altura mínima)
    {
        curHigh += velocity * deltaTime;
        if (curHigh >= maxHigh) {
            stateOpa = 1; // Pasar a avanzar con espiral
            curWalk = 0.0f; // Resetear contador para avance
        }

        // Aplicar ease-in-out para suavizar inicio y final
        float progress = curHigh / maxHigh; // 0 a 1
        float easedProgress = progress * progress * (3.0f - 2.0f * progress); // smoothstep

        // Calcular altura: desde minCycleHeight hasta maxHigh
        float heightRange = maxHigh - minCycleHeight;
        float smoothHeight = minCycleHeight + heightRange * easedProgress;

        model = glm::translate(model, glm::vec3(0.0f, smoothHeight * SCALE_FACTOR, 0.0f));
        break;
    }

    case 1: // Estado 2: Avanzar en línea recta mientras forma espiral que se cierra
    {
        // Desacelerar al final: usar velocidad variable
        float spiralProgress = curWalk / maxWalk; // 0 a 1

        // Calcular velocidad con desaceleración al final (ease-out)
        float velocityMultiplier = 1.0f - (spiralProgress * spiralProgress); // 1.0 → 0.0 cuadrático
        float adjustedVelocity = velocity * glm::max(0.3f, velocityMultiplier); // Mínimo 30% de velocidad

        curWalk += adjustedVelocity * deltaTime;

        // Ease-in para inicio suave desde estado 0
        float easedProgress = spiralProgress * spiralProgress; // ease-in cuadrático
        opaRadio = maxOpaRadio * easedProgress; // Radio pequeño → grande con suavizado

        // Ángulo de rotación de la espiral (muchas vueltas)
        float angle = curWalk * 30.0f; // Mayor velocidad angular para más vueltas

        // Calcular posición: avanza en X + movimiento circular en YZ (con escala)
        float forwardDistance = curWalk * SCALE_FACTOR;
        float x = forwardDistance;
        float y = opaRadio * glm::sin(glm::radians(angle)) * SCALE_FACTOR;
        float z = opaRadio * glm::cos(glm::radians(angle)) * SCALE_FACTOR;

        // Guardar la última posición y ángulo antes de cambiar de estado
        if (curWalk >= maxWalk) {
            lastSpiralPos = glm::vec3(z, curHigh * SCALE_FACTOR + y, -x);
            lastSpiralAngle = angle;
            stateOpa = 15; // Pasar a pausa antes de la curva
            curWalk = 0.0f;
            pauseStartTime = glfwGetTime(); // Iniciar cronómetro de pausa
        }

        model = glm::translate(model, glm::vec3(z, curHigh * SCALE_FACTOR + y, -x));
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        break;
    }

    case 15: // Estado de pausa: esperar 0.5 segundos antes de la curva
    {
        double currentTime = glfwGetTime();
        double elapsedTime = currentTime - pauseStartTime;

        if (elapsedTime >= 0.5) {
            stateOpa = 2; // Pasar a la curva después de 0.5 segundos
            curWalk = 0.0f;
        }

        // Mantener posición final de la espiral sin moverse
        model = glm::translate(model, lastSpiralPos);
        model = glm::rotate(model, glm::radians(lastSpiralAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        break;
    }

    case 2: // Estado 3: Dar una vuelta suave (180 grados) con suavizado
    {
        curWalk += velocity * 0.5f * deltaTime; // Más lento para transición suave
        float turnDuration = maxWalk * 0.8f;

        // Limitar curWalk para evitar overshoot
        float turnProgress = glm::min(curWalk / turnDuration, 1.0f); // 0 a 1

        // Aplicar ease-in-out para curva muy suave
        float easedTurnProgress = turnProgress * turnProgress * (3.0f - 2.0f * turnProgress); // smoothstep
        float turnAngle = 360.0f * easedTurnProgress - lastSpiralAngle;

        // Radio de la curva (escalado)
        float turnRadius = 5.0f * SCALE_FACTOR;

        // Calcular offset de la curva
        float curveOffsetX = turnRadius * glm::sin(glm::radians(turnAngle));
        float curveOffsetZ = -turnRadius * (1.0f - glm::cos(glm::radians(turnAngle)));

        // Posición = última posición de espiral + offset de curva
        float x = lastSpiralPos.x + curveOffsetZ;
        float y = lastSpiralPos.y;
        float z = lastSpiralPos.z - curveOffsetX;

        model = glm::translate(model, glm::vec3(x, y, z));
        model = glm::rotate(model, glm::radians(lastSpiralAngle + turnAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(turnAngle / 2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        lastAngleOpa = lastSpiralAngle + turnAngle;

        // Guardar posición y ángulo final de la curva ANTES de cambiar de estado
        if (curWalk >= turnDuration) {
            lastCurvePos = glm::vec3(x, y, z);
            lastCurveAngle = lastSpiralAngle + turnAngle;
            stateOpa = 25; // Pasar a estado de pausa antes del regreso
            pauseStartTime = glfwGetTime(); // Reiniciar cronómetro de pausa
            curWalk = 0.0f;
        }
        break;
    }

    case 25: // Estado de pausa: esperar 0.5 segundos antes de regresar
    {
        double currentTime = glfwGetTime();
        double elapsedTime = currentTime - pauseStartTime;

        if (elapsedTime >= 0.5) {
            stateOpa = 3; // Pasar a regresar después de 0.5 segundos
            curWalk = 0.0f;
        }

        // Mantener posición final de la curva sin moverse
        model = glm::translate(model, lastCurvePos);
        model = glm::rotate(model, glm::radians(lastCurveAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians((360.0f - lastSpiralAngle) / 2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        break;
    }

    case 3: // Estado 4: Regresar al punto de inicio con suavizado
    {
        curWalk += velocity * deltaTime;

        // Limitar returnProgress para evitar overshoot
        float returnProgress = glm::min(curWalk / maxWalk, 1.0f); // 0 a 1

        // Aplicar ease-in-out para regreso suave
        float easedReturnProgress = returnProgress * returnProgress * (3.0f - 2.0f * returnProgress); // smoothstep

        // Interpolar desde lastCurvePos hacia (0, lastCurvePos.y, 0) - mantener altura
        float x = lastCurvePos.x * (1.0f - easedReturnProgress);
        float y = lastCurvePos.y; // Mantener la misma altura durante todo el regreso
        float z = lastCurvePos.z * (1.0f - easedReturnProgress);

        model = glm::translate(model, glm::vec3(x, y, z));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        if (curWalk >= maxWalk) {
            stateOpa = 4; // Pasar a descender
            curWalk = 0.0f;
            // curHigh representa el progreso, iniciamos desde maxHigh para descender
            curHigh = maxHigh;
        }
        break;
    }

    case 4: // Estado 5: Descender con suavizado (hasta altura mínima)
    {
        curHigh -= velocity * deltaTime;

        // Guardar la altura de inicio (altura donde terminó estado 3)
        static float startHeight = lastCurvePos.y / SCALE_FACTOR; // Convertir a escala original

        // Si es el primer frame del estado 4, guardar la altura inicial
        if (curHigh == maxHigh) {
            startHeight = lastCurvePos.y / SCALE_FACTOR;
        }

        // Calcular altura con suavizado desde startHeight hasta minCycleHeight
        float heightRange = startHeight - minCycleHeight;
        float progress = glm::clamp((curHigh - minCycleHeight) / (startHeight - minCycleHeight), 0.0f, 1.0f);
        float easedProgress = progress * progress * (3.0f - 2.0f * progress); // smoothstep
        float smoothHeight = minCycleHeight + heightRange * easedProgress;

        model = glm::translate(model, glm::vec3(0.0f, smoothHeight * SCALE_FACTOR, 0.0f));

        if (curHigh <= minCycleHeight) {
            // Reiniciar ciclo (animación continua)
            stateOpa = 0;
            curHigh = 0.0f;
            curWalk = 0.0f;
            opaRadio = 0.0f;
            lastSpiralPos = glm::vec3(0.0f);
            lastSpiralAngle = 0.0f;
            lastCurvePos = glm::vec3(0.0f);
            lastCurveAngle = 0.0f;
            pauseStartTime = -1.0;
        }
        break;
    }

    default:
        break;
    }

    return model;
}

// Función para iniciar la animación de Opa-Opa (opcional, ya que es continua)
void StartOpaAnimation()
{
    // Resetear al estado inicial si se desea reiniciar manualmente
    stateOpa = 0;
    curHigh = 0.0f;
    curWalk = 0.0f;
    opaRadio = 0.0f;
    lastSpiralPos = glm::vec3(0.0f);
    lastSpiralAngle = 0.0f;
    lastAngleOpa = 0.0f;
    lastCurvePos = glm::vec3(0.0f);
    lastCurveAngle = 0.0f;
    pauseStartTime = -1.0;
    printf("Animacion Opa-Opa reiniciada!\n");
}

// Función para activar/desactivar la caminata
void ToggleWalking()
{
    walkingActive = !walkingActive;
    if (!walkingActive) {
        walkCycle = 0.0f; // Resetear el ciclo cuando se detiene
    }
}

// Función para establecer el estado de caminata
void SetWalkingActive(bool active)
{
    walkingActive = active;
    if (!walkingActive) {
        walkCycle = 0.0f; // Resetear el ciclo cuando se detiene
    }
}

// Función para obtener el estado de caminata
bool IsWalkingActive()
{
    return walkingActive;
}

// Animación jerárquica del cuerpo (torso)
glm::mat4 AnimateBody(glm::mat4 model)
{
    if (walkingActive) {
        // Ligero balanceo del torso al caminar
        float bodySwing = 3.0f * glm::sin(glm::radians(walkCycle));
        model = glm::rotate(model, glm::radians(bodySwing), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    return model;
}

// Animación jerárquica del hombro derecho
glm::mat4 AnimateRightShoulder(glm::mat4 parentModel)
{
    glm::mat4 model = parentModel;
    // Offset del hombro derecho respecto al cuerpo
   
    
    if (walkingActive) {
        // Rotación del hombro (movimiento del brazo)
        float shoulderAngle = 30.0f * glm::sin(glm::radians(walkCycle));
        model = glm::rotate(model, glm::radians(shoulderAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    }
    return model;
}

// Animación jerárquica del brazo derecho
glm::mat4 AnimateRightArm(glm::mat4 parentModel)
{
    glm::mat4 model = parentModel;
    // Offset del brazo respecto al hombro
    
    if (walkingActive) {
        // Ligera flexión adicional del codo al caminar
        float elbowBend = 10.0f * glm::sin(glm::radians(walkCycle * 2.0f));
        model = glm::rotate(model, glm::radians(elbowBend), glm::vec3(1.0f, 0.0f, 0.0f));
    }
    return model;
}

// Animación jerárquica del hombro izquierdo
glm::mat4 AnimateLeftShoulder(glm::mat4 parentModel)
{
    glm::mat4 model = parentModel;
    // Offset del hombro izquierdo respecto al cuerpo

    
    if (walkingActive) {
        // Rotación del hombro (movimiento del brazo opuesto al derecho)
        float shoulderAngle = -30.0f * glm::sin(glm::radians(walkCycle));
        model = glm::rotate(model, glm::radians(shoulderAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    }
    return model;
}

// Animación jerárquica del brazo izquierdo
glm::mat4 AnimateLeftArm(glm::mat4 parentModel)
{
    glm::mat4 model = parentModel;
    // Offset del brazo respecto al hombro
    //model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
    
    if (walkingActive) {
        // Ligera flexión adicional del codo al caminar
        float elbowBend = -10.0f * glm::sin(glm::radians(walkCycle * 2.0f));
        model = glm::rotate(model, glm::radians(elbowBend), glm::vec3(1.0f, 0.0f, 0.0f));
    }
    return model;
}

// Animación jerárquica del muslo derecho
glm::mat4 AnimateRightThigh(glm::mat4 parentModel)
{
    glm::mat4 model = parentModel;
    // Offset del muslo derecho respecto al cuerpo
    
    if (walkingActive) {
        // Rotación del muslo (pierna opuesta al brazo derecho)
        float thighAngle = -50.0f * glm::sin(glm::radians(walkCycle));
        model = glm::rotate(model, glm::radians(thighAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    }
    return model;
}

// Animación jerárquica de la pierna derecha
glm::mat4 AnimateRightLeg(glm::mat4 parentModel)
{
    glm::mat4 model = parentModel;
    // Offset de la pierna respecto al muslo

    
    if (walkingActive) {
        // Flexión de la rodilla (solo cuando la pierna va hacia adelante)
        float kneeAngle =glm::min(45-glm::max(0.0f, 30.0f * glm::sin(glm::radians(walkCycle))),30.0f);
        model = glm::rotate(model, glm::radians(kneeAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    }
    return model;
}

// Animación jerárquica del muslo izquierdo
glm::mat4 AnimateLeftThigh(glm::mat4 parentModel)
{
    glm::mat4 model = parentModel;
    // Offset del muslo izquierdo respecto al cuerpo
    
    if (walkingActive) {
        // Rotación del muslo (pierna opuesta al brazo izquierdo)
        float thighAngle = 50.0f * glm::sin(glm::radians(walkCycle));
        model = glm::rotate(model, glm::radians(thighAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    }
    return model;
}

// Animación jerárquica de la pierna izquierda
glm::mat4 AnimateLeftLeg(glm::mat4 parentModel)
{
    glm::mat4 model = parentModel;
    // Solo aplicar rotación si está caminando
    if (walkingActive) {
        // Flexión de la rodilla (solo cuando la pierna va hacia adelante)
        float kneeAngle = glm::min(45 - glm::max(0.0f, 30.0f * glm::sin(glm::radians(walkCycle))), 30.0f);
        model = glm::rotate(model, glm::radians(45.0f-kneeAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    }
    return model;
}

//Animacion anillo
glm::mat4 AnimateRing(glm::mat4 model)
{
    angulovaria += 10.0f * deltaTime;
    model = glm::translate(model, glm::vec3(-398.50, 25.00 + 1 * sin(glm::radians(angulovaria)), -253.83));
    model = glm::rotate(model, glm::radians(angulovaria) , glm::vec3(0.0f, 1.0f, 0.0f));
    return model;
}

// Actualizar el ciclo de caminata
void UpdateWalkCycle()
{
    if (walkingActive) {
        walkCycle += walkSpeed * deltaTime;
        if (walkCycle >= 360.0f) {
            walkCycle -= 360.0f;
        }
    }
}

// ========== Sistema de Animaciones por Keyframes ==========

// Manager global de animaciones
KeyframeAnimationManager g_AnimationManager;

void InitKeyframeAnimations()
{
    printf("\n=== Inicializando animaciones por keyframes ===\n");
    
    // Cargar animaciones
    g_AnimationManager.AddAnimation("Ring_Bell", "keyframes_ring_bell.txt", 50);
    g_AnimationManager.AddAnimation("Reloj","keyframes_Reloj.txt",100);
    g_AnimationManager.AddAnimation("Bell","keyframes_bell.txt",25);
    
    // Configurar animaciones para que se ejecuten en loop automáticamente
    KeyframeAnimation* ringBellAnim = g_AnimationManager.GetAnimation("Ring_Bell");
    if (ringBellAnim) {
        ringBellAnim->SetLooping(true);
        ringBellAnim->Play();
        printf("Ring_Bell: Loop activado y reproduciendo\n");
    }
    
    KeyframeAnimation* relojAnim = g_AnimationManager.GetAnimation("Reloj");
    if (relojAnim) {
        relojAnim->SetLooping(true);
        relojAnim->Play();
        printf("Reloj: Loop activado y reproduciendo\n");
    }
    
    KeyframeAnimation* bellAnim = g_AnimationManager.GetAnimation("Bell");
    if (bellAnim) {
        bellAnim->SetLooping(true);
        bellAnim->Play();
        printf("Bell: Loop activado y reproduciendo\n");
    }
    
    // Listar animaciones cargadas
    g_AnimationManager.ListAnimations();
    
    printf("=== Animaciones inicializadas y en loop ===\n\n");
}

void UpdateKeyframeAnimations()
{
    // Actualizar todas las animaciones activas
    g_AnimationManager.UpdateAll();
}

