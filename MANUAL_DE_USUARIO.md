# 📖 Manual de Usuario - Proyecto Final CGI

## 🎮 Controles de Teclado

Este manual describe todos los controles interactivos del proyecto mediante el teclado. El usuario puede interactuar con diversos elementos del escenario en tiempo real.

---

##  Controles Generales

| Tecla | Acción | Efecto Visual |
|-------|--------|---------------|
| **ESC** | Cerrar aplicación | Cierra la ventana y termina el programa inmediatamente |

---

## 🎯 Controles del Escenario

### 🥊 **Tecla Q** - Contador de Rounds
**Función:** Incrementa el contador de rounds del cuadrilátero de boxeo

**Efecto en el escenario:**
- Cambia el número mostrado en el marcador digital del ring
- El contador avanza de 00 a 14 (ciclo de 15 rounds)
- Al llegar a 14, vuelve a 00 automáticamente
- Se muestra en el escenario como un display de dos dígitos

**Uso:** Presiona Q repetidamente para avanzar los rounds

---

### 🎭 **Tecla P** - Rotación de Máscaras
**Función:** Cambia las texturas de las máscaras en las paredes del ring de boxeo

**Efecto en el escenario:**
- Las cuatro paredes del ring muestran diferentes máscaras
- Cada presión rota las máscaras una posición (hay 4 combinaciones diferentes)
- Las máscaras rotan de forma cíclica:
  - Posición 0 → Posición 1 → Posición 2 → Posición 3 → Posición 0
- Todas las paredes cambian simultáneamente

**Uso:** Presiona P para ver las diferentes combinaciones de máscaras

---

### 💥 **Tecla T** - Animación de la TNT
**Función:** Activa la animación de explosión de la caja de TNT

**Efecto en el escenario:**
- **Segundo 0-4:** La caja permanece cerrada
- **Segundo 4:** La tapa comienza a elevarse
- **Segundo 4-5.5:** La tapa sube mientras la explosión crece
- **Segundo 5.5-7:** La tapa desciende mientras la explosión se mantiene
- **Segundo 7:** La animación termina, tapa regresa a posición original

**Características:**
- La explosión aparece como un billboard (sprite que siempre mira a la cámara)
- La escala de la explosión crece de forma sincronizada con la tapa
- Solo se puede activar una vez por ciclo (debe terminar antes de reiniciar)
- Duración total: aproximadamente 7 segundos

**Uso:** Presiona T para iniciar la secuencia de explosión

---


---

## 🎬 Animaciones Automáticas por Keyframes

Estas animaciones se ejecutan **automáticamente en loop** desde el inicio del programa. No requieren intervención del usuario.

### 🔔 Campana del Ring (Bell)
**Animación:** Balanceo lateral continuo

**Comportamiento:**
- Rota sobre el eje Z (balanceo de izquierda a derecha)
- Rango de movimiento: -50° a +50°
- Movimiento suave y cíclico que simula una campana de boxeo
- Loop infinito desde el inicio del programa


---

### ⏰ Reloj - Minutero (Reloj)
**Animación:** Rotación completa del minutero

**Comportamiento:**
- Rota sobre el eje Y (como las manecillas de un reloj)
- Rotación completa de 360° (una vuelta completa)
- Simula el movimiento de un minutero de reloj analógico
- Velocidad constante, loop infinito

**Modelo afectado:** `reloj_minutero.obj`

---

### 🛎️ Sistema de Palanca y Campana (Ring_Bell)
**Animación:** Movimiento complejo de palanca con campana

**Comportamiento:**
- Animación jerárquica (palanca → campana)
- Movimiento en múltiples ejes (X, Y, Z)
- Simula el sistema mecánico de una campana de boxeo activada por palanca
- Incluye rotaciones y traslaciones sincronizadas
- Loop infinito con ciclo de activación/reposo

**Modelos afectados:** 
- `Boxing_Bell_palanca.obj` (palanca)
- `Boxing_Bell.obj` (campana del ring)

---

## 📋 Notas Importantes

### ⚙️ Configuración del Sistema de Keyframes
- Las animaciones por keyframes están configuradas con **100 pasos de interpolación**
- Esto significa transiciones muy suaves entre fotogramas clave
- A mayor número de pasos, más lento y suave el movimiento
- A menor número de pasos, más rápido pero menos fluido

### 🔄 Loops Automáticos
- **Bell**, **Reloj** y **Ring_Bell** se ejecutan automáticamente al iniciar
- No se pueden pausar con teclas (están en loop infinito)
- Se reinician automáticamente al completar su ciclo

### 🎯 Animación TNT
- Es la única animación que **NO** está en loop automático
- Debe activarse manualmente con la tecla **T**
- Tiene protección contra activaciones múltiples simultáneas
- Espera a que termine antes de permitir nueva activación

### 🚶 Animación de Caminata
- Es independiente del sistema de keyframes
- Usa interpolación sinusoidal en tiempo real
- Se puede activar/desactivar en cualquier momento
- No interfiere con otras animaciones

---

## 🎨 Navegación en el Escenario

### 🖱️ Controles de Cámara (Ratón)
- **Movimiento del ratón:** Rota la vista de la cámara
- **Primera vez:** El cursor se oculta al mover el mouse

### ⌨️ Controles de Cámara (Teclado - WASD)
- **W:** Mover cámara hacia adelante
- **S:** Mover cámara hacia atrás
- **A:** Mover cámara a la izquierda
- **D:** Mover cámara a la derecha

**Nota:** Los controles WASD son continuos (mantén presionado para movimiento continuo)

---

## 🔧 Solución de Problemas

### La animación TNT no se activa
✅ **Solución:** Espera a que termine la animación actual (7 segundos) antes de presionar T nuevamente

### Las máscaras no cambian
✅ **Solución:** Asegúrate de estar mirando las paredes del ring. Hay 4 posiciones que rotan cíclicamente.

### El contador de rounds no se ve
✅ **Solución:** Mueve la cámara hacia el área del ring donde se encuentra el marcador digital.

### La caminata no se ve
✅ **Solución:** Busca el personaje humanoide en el escenario y asegúrate de que la tecla G esté activada.

---

## 📊 Resumen de Teclas

| Tecla | Función | Tipo |
|-------|---------|------|
| **ESC** | Salir | Sistema |
| **Q** | Contador Rounds | Interactivo |
| **P** | Rotar Máscaras | Interactivo |
| **T** | Explosión TNT | Animación (Manual) |
| **G** | Caminata | Toggle ON/OFF |
| **W/A/S/D** | Mover Cámara | Navegación |
| **Mouse** | Rotar Vista | Navegación |

---

## 🎓 Créditos

**Proyecto Final - Computación Gráfica e Interacción Humano-Computadora**

**Tecnologías utilizadas:**
- OpenGL para renderizado 3D
- GLFW para ventanas y entrada
- GLM para matemáticas vectoriales
- Sistema de keyframes personalizado
- Animaciones jerárquicas en tiempo real

---

**Versión:** 1.0  
**Fecha:** Noviembre 2025  
**Repositorio:** PFCGeIHC

---

## 📝 Guía Rápida

1. **Iniciar el programa** → Las animaciones automáticas (campana, reloj, ring_bell) comenzarán inmediatamente
2. **Presiona Q** → Para cambiar el round del marcador
3. **Presiona P** → Para rotar las máscaras del ring
4. **Presiona T** → Para ver la explosión de TNT
5. **Presiona G** → Para activar/desactivar la caminata del personaje
6. **Usa W/A/S/D + Mouse** → Para explorar el escenario
7. **Presiona ESC** → Para salir

¡Disfruta explorando el escenario! 🎮✨

