#pragma once
#ifndef KEYFRAME_ANIMATION_H
#define KEYFRAME_ANIMATION_H

#include <glm.hpp>
#include <string>
#include <vector>
#include "miniaudio.h"

#define MAX_FRAMES 100

// Estructura de un keyframe genérico con 3 valores (puede ser posición X,Y,Z o rotación X,Y,Z, etc.)
struct Keyframe
{
    float value1;      // Primer valor (ej: posX, rotX)
    float value2;      // Segundo valor (ej: posY, rotY)
    float value3;      // Tercer valor (ej: posZ, rotZ)
    
    // Incrementos para interpolación
    float value1Inc;
    float value2Inc;
    float value3Inc;
};

// Clase para manejar una animación por keyframes
class KeyframeAnimation
{
public:
    KeyframeAnimation();
    KeyframeAnimation(const std::string& filename, int maxSteps = 100);
    ~KeyframeAnimation();

    // Cargar keyframes desde archivo
    bool LoadFromFile(const std::string& filename);
    
    // Control de animación
    void Play();           // Iniciar/reanudar animación
    void Pause();          // Pausar animación
    void Stop();           // Detener y resetear animación
    void Reset();          // Volver al primer frame
    
    // Actualizar animación (llamar en cada frame del render loop)
    void Update();
    
    // Obtener valores actuales interpolados
    float GetValue1() const { return currentValue1; }
    float GetValue2() const { return currentValue2; }
    float GetValue3() const { return currentValue3; }
    
    // Obtener estado
    bool IsPlaying() const { return isPlaying; }
    int GetCurrentFrame() const { return playIndex; }
    int GetTotalFrames() const { return frameCount; }
    
    // Configuración
    void SetInterpolationSteps(int steps) { maxSteps = steps; }
    void SetLooping(bool loop) { looping = loop; }
    void SetSound(ma_sound* sound) { soundPtr = sound; }
    bool IsLooping() const { return looping; }
    int GetPlayIndex() const { return playIndex; }

private:
    void Interpolate();    // Calcular incrementos entre frames
    
    Keyframe frames[MAX_FRAMES];
    
    // Estado de la animación
    float currentValue1;
    float currentValue2;
    float currentValue3;
    
    int frameCount;        // Número total de frames cargados
    int playIndex;         // Frame actual en reproducción
    int currentStep;       // Paso actual de interpolación
    int maxSteps;          // Pasos totales para interpolación (controla velocidad)
    
    bool isPlaying;
    bool looping;          // Si la animación se repite
    
    std::string animationName;
    ma_sound* soundPtr;    // Puntero al sonido asociado (opcional)
};

// Manager para múltiples animaciones
class KeyframeAnimationManager
{
public:
    KeyframeAnimationManager();
    ~KeyframeAnimationManager();
    
    // Agregar una animación al manager
    bool AddAnimation(const std::string& name, const std::string& filename, int maxSteps = 100);
    
    // Control de animaciones por nombre
    void PlayAnimation(const std::string& name);
    void PauseAnimation(const std::string& name);
    void StopAnimation(const std::string& name);
    void ResetAnimation(const std::string& name);
    // Actualizar todas las animaciones activas
    void UpdateAll();
    
    // Obtener referencia a una animación
    KeyframeAnimation* GetAnimation(const std::string& name);
    
    // Listar animaciones
    void ListAnimations();

private:
    std::vector<std::pair<std::string, KeyframeAnimation*>> animations;
};

#endif // KEYFRAME_ANIMATION_H
