#include "KeyframeAnimation.h"
#include <fstream>
#include <iostream>

// ==================== KeyframeAnimation ====================

KeyframeAnimation::KeyframeAnimation()
    : frameCount(0), playIndex(0), currentStep(0), maxSteps(100),
      isPlaying(false), looping(false),
      currentValue1(0.0f), currentValue2(0.0f), currentValue3(0.0f)
{
}

KeyframeAnimation::KeyframeAnimation(const std::string& filename, int maxSteps)
    : frameCount(0), playIndex(0), currentStep(0), maxSteps(maxSteps),
      isPlaying(false), looping(false),
      currentValue1(0.0f), currentValue2(0.0f), currentValue3(0.0f),
      animationName(filename)
{
    LoadFromFile(filename);
}

KeyframeAnimation::~KeyframeAnimation()
{
}

bool KeyframeAnimation::LoadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        printf("Error: No se pudo abrir el archivo %s\n", filename.c_str());
        return false;
    }
    
    frameCount = 0;
    float v1, v2, v3;
    
    while (frameCount < MAX_FRAMES && file >> v1 >> v2 >> v3)
    {
        frames[frameCount].value1 = v1;
        frames[frameCount].value2 = v2;
        frames[frameCount].value3 = v3;
        frameCount++;
    }
    
    file.close();
    
    if (frameCount > 0)
    {
        // Inicializar con el primer frame
        currentValue1 = frames[0].value1;
        currentValue2 = frames[0].value2;
        currentValue3 = frames[0].value3;
        
        printf("Animacion '%s' cargada: %d frames\n", filename.c_str(), frameCount);
        return true;
    }
    else
    {
        printf("Error: No se encontraron keyframes en %s\n", filename.c_str());
        return false;
    }
}

void KeyframeAnimation::Play()
{
    if (frameCount <= 1)
    {
        printf("Error: No hay suficientes frames para reproducir\n");
        return;
    }
    
    if (!isPlaying)
    {
        isPlaying = true;
        if (playIndex >= frameCount - 1)
        {
            // Si ya terminó, reiniciar
            Reset();
        }
        Interpolate();
        printf("Reproduciendo animacion: %s (frame %d/%d)\n", animationName.c_str(), playIndex, frameCount);
    }
}

void KeyframeAnimation::Pause()
{
    isPlaying = false;
    printf("Animacion pausada: %s\n", animationName.c_str());
}

void KeyframeAnimation::Stop()
{
    isPlaying = false;
    Reset();
    printf("Animacion detenida: %s\n", animationName.c_str());
}

void KeyframeAnimation::Reset()
{
    playIndex = 0;
    currentStep = 0;
    currentValue1 = frames[0].value1;
    currentValue2 = frames[0].value2;
    currentValue3 = frames[0].value3;
}

void KeyframeAnimation::Interpolate()
{
    if (playIndex >= frameCount - 1)
        return;
    
    // Calcular incrementos entre frame actual y siguiente
    frames[playIndex].value1Inc = (frames[playIndex + 1].value1 - frames[playIndex].value1) / maxSteps;
    frames[playIndex].value2Inc = (frames[playIndex + 1].value2 - frames[playIndex].value2) / maxSteps;
    frames[playIndex].value3Inc = (frames[playIndex + 1].value3 - frames[playIndex].value3) / maxSteps;
}

void KeyframeAnimation::Update()
{
    if (!isPlaying || frameCount <= 1)
        return;
    
    if (currentStep >= maxSteps) // ¿Terminó interpolación entre frames?
    {
        playIndex++;
        
        if (playIndex >= frameCount - 1) // ¿Llegó al último frame?
        {
            if (looping)
            {
                // Reiniciar animación
                playIndex = 0;
                currentStep = 0;
                currentValue1 = frames[0].value1;
                currentValue2 = frames[0].value2;
                currentValue3 = frames[0].value3;
                Interpolate();
            }
            else
            {
                // Detener animación
                isPlaying = false;
                printf("Animacion completada: %s\n", animationName.c_str());
            }
        }
        else
        {
            // Continuar con siguiente frame
            currentStep = 0;
            Interpolate();
        }
    }
    else
    {
        // Interpolar valores
        currentValue1 += frames[playIndex].value1Inc;
        currentValue2 += frames[playIndex].value2Inc;
        currentValue3 += frames[playIndex].value3Inc;
        currentStep++;
    }
}

// ==================== KeyframeAnimationManager ====================

KeyframeAnimationManager::KeyframeAnimationManager()
{
}

KeyframeAnimationManager::~KeyframeAnimationManager()
{
    // Liberar memoria de todas las animaciones
    for (auto& pair : animations)
    {
        delete pair.second;
    }
    animations.clear();
}

bool KeyframeAnimationManager::AddAnimation(const std::string& name, const std::string& filename, int maxSteps)
{
    // Verificar si ya existe una animación con ese nombre
    for (const auto& pair : animations)
    {
        if (pair.first == name)
        {
            printf("Error: Ya existe una animacion con el nombre '%s'\n", name.c_str());
            return false;
        }
    }
    
    // Crear nueva animación
    KeyframeAnimation* anim = new KeyframeAnimation(filename, maxSteps);
    
    if (anim->GetTotalFrames() > 0)
    {
        animations.push_back(std::make_pair(name, anim));
        printf("Animacion '%s' agregada al manager\n", name.c_str());
        return true;
    }
    else
    {
        delete anim;
        return false;
    }
}

void KeyframeAnimationManager::PlayAnimation(const std::string& name)
{
    KeyframeAnimation* anim = GetAnimation(name);
    if (anim)
    {
        anim->Play();
    }
    else
    {
        printf("Error: Animacion '%s' no encontrada\n", name.c_str());
    }
}

void KeyframeAnimationManager::PauseAnimation(const std::string& name)
{
    KeyframeAnimation* anim = GetAnimation(name);
    if (anim)
    {
        anim->Pause();
    }
}

void KeyframeAnimationManager::StopAnimation(const std::string& name)
{
    KeyframeAnimation* anim = GetAnimation(name);
    if (anim)
    {
        anim->Stop();
    }
}

void KeyframeAnimationManager::ResetAnimation(const std::string& name)
{
    KeyframeAnimation* anim = GetAnimation(name);
    if (anim)
    {
        anim->Reset();
    }
}

void KeyframeAnimationManager::UpdateAll()
{
    for (auto& pair : animations)
    {
        pair.second->Update();
    }
}

KeyframeAnimation* KeyframeAnimationManager::GetAnimation(const std::string& name)
{
    for (auto& pair : animations)
    {
        if (pair.first == name)
        {
            return pair.second;
        }
    }
    return nullptr;
}

void KeyframeAnimationManager::ListAnimations()
{
    printf("\n=== Animaciones cargadas ===\n");
    for (const auto& pair : animations)
    {
        KeyframeAnimation* anim = pair.second;
        printf("- '%s': %d frames, %s\n", 
            pair.first.c_str(), 
            anim->GetTotalFrames(),
            anim->IsPlaying() ? "REPRODUCIENDO" : "DETENIDA");
    }
    printf("============================\n\n");
}
