# Sistema de Animaciones por Keyframes

## 📋 Descripción

Este sistema permite cargar y reproducir múltiples animaciones por keyframes desde archivos de texto. Cada animación puede ser controlada independientemente mediante teclado.

## 🎯 Características

- ✅ **Múltiples animaciones simultáneas**: Carga y controla varias animaciones
- ✅ **Carga desde archivos**: Lee keyframes desde archivos `.txt`
- ✅ **Interpolación suave**: Transiciones fluidas entre keyframes
- ✅ **Control por teclado**: Play/Pause/Stop para cada animación
- ✅ **Loop opcional**: Las animaciones pueden repetirse automáticamente
- ✅ **Fácil integración**: Sistema modular y reutilizable

## 📁 Estructura de Archivos

### Archivos principales:
- `KeyframeAnimation.h` - Definición de clases
- `KeyframeAnimation.cpp` - Implementación
- `animations.h` - Incluye el manager global
- `animations.cpp` - Funciones de inicialización
- `Window.cpp` - Controles de teclado

### Archivos de datos:
- `keyframes_Ace.txt` - Ejemplo de keyframes para personaje Ace
- `keyframes_*.txt` - Otros archivos de keyframes

## 📝 Formato de Archivo de Keyframes

Cada línea del archivo `.txt` contiene 3 valores separados por espacios:

```
valor1 valor2 valor3
```

**Ejemplo (`keyframes_Ace.txt`):**
```
0.0 0.0 0.0
0.0 0.0 -7.5
0.0 0.0 -15.0
0.0 0.0 7.5
0.2 0.0 0.0
```

Los valores pueden representar:
- **Posición**: X, Y, Z
- **Rotación**: X, Y, Z (en grados)
- **Escala**: X, Y, Z
- **Cualquier combinación** de transformaciones

## 🚀 Cómo Usar

### 1. Agregar una Nueva Animación

En `animations.cpp`, función `InitKeyframeAnimations()`:

```cpp
void InitKeyframeAnimations()
{
    // Cargar animación de Ace
    g_AnimationManager.AddAnimation("Ace", "keyframes_Ace.txt", 100);
    
    // Agregar más animaciones
    g_AnimationManager.AddAnimation("Barco", "keyframes_Barco.txt", 80);
    g_AnimationManager.AddAnimation("Lampara", "keyframes_Lampara.txt", 50);
    
    g_AnimationManager.ListAnimations();
}
```

**Parámetros:**
- `nombre`: Identificador único de la animación
- `archivo`: Ruta al archivo `.txt` con keyframes
- `steps`: Número de pasos de interpolación (mayor = más lento)

### 2. Configurar Controles de Teclado

En `Window.cpp`, función `ManejaTeclado()`:

```cpp
// Tecla 1: Play/Pause animación "Ace"
if (key == GLFW_KEY_1 && action == GLFW_RELEASE) {
    KeyframeAnimation* anim = g_AnimationManager.GetAnimation("Ace");
    if (anim) {
        if (anim->IsPlaying()) {
            g_AnimationManager.PauseAnimation("Ace");
        } else {
            g_AnimationManager.PlayAnimation("Ace");
        }
    }
}

// Tecla 2: Stop/Reset animación "Ace"
if (key == GLFW_KEY_2 && action == GLFW_RELEASE) {
    g_AnimationManager.StopAnimation("Ace");
}

// Tecla 3: Play animación "Barco"
if (key == GLFW_KEY_3 && action == GLFW_RELEASE) {
    g_AnimationManager.PlayAnimation("Barco");
}
```

### 3. Aplicar la Animación al Modelo

En el loop de renderizado de `main.cpp`:

```cpp
// Obtener animación
KeyframeAnimation* aceAnim = g_AnimationManager.GetAnimation("Ace");

if (aceAnim) {
    // Obtener valores interpolados
    float posX = aceAnim->GetValue1();
    float posY = aceAnim->GetValue2();
    float posZ = aceAnim->GetValue3();
    
    // Aplicar transformación
    model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(posX, posY, posZ));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    ace["CuerpoAce"].RenderModel();
}
```

### 4. Animación con Rotación

```cpp
KeyframeAnimation* barcoAnim = g_AnimationManager.GetAnimation("Barco");

if (barcoAnim) {
    float rotX = barcoAnim->GetValue1();
    float rotY = barcoAnim->GetValue2();
    float rotZ = barcoAnim->GetValue3();
    
    model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    barco.RenderModel();
}
```

## 🎮 Controles Predefinidos

| Tecla | Acción |
|-------|--------|
| **1** | Play/Pause animación "Ace" |
| **2** | Stop/Reset animación "Ace" |

*Agrega más controles según necesites*

## ⚙️ Configuración Avanzada

### Habilitar Loop (repetición automática)

```cpp
KeyframeAnimation* anim = g_AnimationManager.GetAnimation("Ace");
if (anim) {
    anim->SetLooping(true);  // La animación se repetirá infinitamente
}
```

### Cambiar Velocidad de Interpolación

```cpp
KeyframeAnimation* anim = g_AnimationManager.GetAnimation("Ace");
if (anim) {
    anim->SetInterpolationSteps(200);  // Más lento (más pasos)
    // o
    anim->SetInterpolationSteps(50);   // Más rápido (menos pasos)
}
```

### Verificar Estado de la Animación

```cpp
KeyframeAnimation* anim = g_AnimationManager.GetAnimation("Ace");
if (anim) {
    if (anim->IsPlaying()) {
        printf("Animación en reproducción\n");
    }
    
    int currentFrame = anim->GetCurrentFrame();
    int totalFrames = anim->GetTotalFrames();
    printf("Frame %d de %d\n", currentFrame, totalFrames);
}
```

## 📊 Ejemplo Completo

### 1. Crear archivo `keyframes_MiPersonaje.txt`:
```
0.0 0.0 0.0
10.0 5.0 0.0
20.0 10.0 5.0
10.0 5.0 10.0
0.0 0.0 15.0
```

### 2. Cargar en `InitKeyframeAnimations()`:
```cpp
g_AnimationManager.AddAnimation("MiPersonaje", "keyframes_MiPersonaje.txt", 100);
```

### 3. Agregar control en `Window.cpp`:
```cpp
if (key == GLFW_KEY_4 && action == GLFW_RELEASE) {
    g_AnimationManager.PlayAnimation("MiPersonaje");
}
```

### 4. Renderizar en `main.cpp`:
```cpp
KeyframeAnimation* miAnim = g_AnimationManager.GetAnimation("MiPersonaje");
if (miAnim) {
    model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(
        miAnim->GetValue1(),
        miAnim->GetValue2(),
        miAnim->GetValue3()
    ));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    miModelo.RenderModel();
}
```

## 🔧 Solución de Problemas

### La animación no se carga
- Verifica que el archivo `.txt` existe en la carpeta del proyecto
- Revisa que el formato sea correcto (3 valores por línea)
- Comprueba la consola para mensajes de error

### La animación es muy rápida/lenta
- Ajusta el parámetro `maxSteps` en `AddAnimation()`
- Mayor valor = más lento
- Menor valor = más rápido

### La animación no se actualiza
- Asegúrate de llamar `UpdateKeyframeAnimations()` en el loop principal
- Verifica que la animación esté en estado "Playing"

## 📌 Notas Importantes

1. ✅ **NO es necesario grabar keyframes** - Solo se cargan y reproducen
2. ✅ **Múltiples animaciones** pueden reproducirse simultáneamente
3. ✅ **Cada animación es independiente** con sus propios controles
4. ✅ **Los archivos .txt deben estar en la raíz** del proyecto (o especifica ruta completa)

## 🎓 Migración desde KeyFrames.cpp Antiguo

Si tenías código antiguo con `saveFrame()` y teclas para grabar:

### ❌ Código antiguo (YA NO NECESARIO):
```cpp
// NO USES ESTO
void saveFrame(void) { ... }
void inputKeyframes(bool* keys) { ... }
```

### ✅ Nuevo sistema:
```cpp
// USA ESTO
g_AnimationManager.AddAnimation("MiModelo", "keyframes_MiModelo.txt", 100);
g_AnimationManager.PlayAnimation("MiModelo");
```

## 📧 Resumen

Este sistema moderno de keyframes te permite:
- ✅ Cargar animaciones desde archivos
- ✅ Controlar múltiples animaciones independientemente
- ✅ Usar controles de teclado personalizados
- ✅ No preocuparte por grabar keyframes (ya están en archivos)
- ✅ Integrar fácilmente en tu proyecto existente

¡Disfruta animando tus modelos! 🎨
