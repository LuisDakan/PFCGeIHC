# Manual de Usuario - PFCGeIHC
## Entorno Virtual Interactivo 3D

---

## 📋 Tabla de Contenidos
1. [Introducción](#introducción)
2. [Requisitos del Sistema](#requisitos-del-sistema)
3. [Instalación](#instalación)
4. [Inicio Rápido](#inicio-rápido)
5. [Controles y Navegación](#controles-y-navegación)
6. [Funcionalidades del Sistema](#funcionalidades-del-sistema)
7. [Solución de Problemas](#solución-de-problemas)
8. [Contacto y Soporte](#contacto-y-soporte)

---

## 🎮 Introducción

**PFCGeIHC** es un entorno virtual interactivo en 3D que representa una zona abierta inspirada en elementos de la cultura prehispánica y la lucha libre mexicana. El proyecto combina estilos visuales de universos ficticios como Ace Attorney, Crash Bandicoot y juegos clásicos de SEGA.

### Características principales:
- ✨ Ciclo día/noche automático con cambios de iluminación
- 🎨 Skybox dinámico con cambio de texturas
- 🔦 Sistema de iluminación realista (luces direccionales, puntuales y spots)
- 🎯 Cámara en primera persona con control completo
- 🏺 Modelos 3D con texturas realistas
- 🕹️ Controles interactivos para modificar la escena

---

## 💻 Requisitos del Sistema

### Hardware Mínimo:
- **Procesador:** Intel Core i3 o equivalente
- **Memoria RAM:** 4 GB
- **Tarjeta gráfica:** Compatible con OpenGL 4.3 o superior
- **Espacio en disco:** 500 MB

### Hardware Recomendado:
- **Procesador:** Intel Core i5 o superior
- **Memoria RAM:** 8 GB o más
- **Tarjeta gráfica:** NVIDIA GTX 960 / AMD Radeon R9 380 o superior
- **Espacio en disco:** 1 GB

### Software:
- **Sistema Operativo:** Windows 10/11 (64-bit)
- **Visual Studio 2019 o superior** (para compilación)
- **Drivers de tarjeta gráfica actualizados**

---

## 📥 Instalación

### Paso 1: Descargar el Proyecto
1. Clone el repositorio desde GitHub:
   ```bash
   git clone https://github.com/LuisDakan/PFCGeIHC.git
   ```
2. O descargue el archivo ZIP y extraiga el contenido.

### Paso 2: Abrir el Proyecto
1. Navegue a la carpeta del proyecto.
2. Abra el archivo `PFCGEIHC.vcxproj` con Visual Studio.

### Paso 3: Configurar Dependencias
El proyecto incluye todas las librerías necesarias:
- ✅ OpenGL (GLEW, GLFW)
- ✅ GLM (matemáticas)
- ✅ Assimp (carga de modelos)
- ✅ stb_image (carga de texturas)

### Paso 4: Compilar
1. Seleccione la configuración **Debug** o **Release**.
2. Seleccione la plataforma **x64**.
3. Presione **F5** o haga clic en **Iniciar depuración**.

---

## 🚀 Inicio Rápido

### Primera Ejecución:
1. Compile y ejecute el proyecto.
2. Se abrirá una ventana de 800x600 píxeles mostrando el entorno 3D.
3. Verá un piso con múltiples modelos de antorchas distribuidos por la escena.
4. El ciclo día/noche comenzará automáticamente cada 5 segundos.

### Navegación Básica:
- Use **W, A, S, D** para moverse por la escena.
- Mueva el **mouse** para rotar la cámara.
- Presione **ESC** para cerrar la aplicación.

---

## 🎮 Controles y Navegación

### Movimiento de la Cámara

| Tecla | Acción |
|-------|--------|
| **W** | Mover hacia adelante |
| **S** | Mover hacia atrás |
| **A** | Mover a la izquierda |
| **D** | Mover a la derecha |
| **Mouse** | Rotar la cámara (vista en primera persona) |

### Controles de Iluminación

| Tecla | Acción |
|-------|--------|
| **M** | Encender/Apagar lámpara principal |
| **B** | Activar/Desactivar antorcha |
| **N** | Activar/Desactivar pistola de rayos (RayGun) |

### Controles de Articulaciones

Estos controles permiten manipular partes articuladas de modelos en la escena:

| Tecla | Acción |
|-------|--------|
| **F** | Incrementar articulación 1 (+9°) |
| **G** | Decrementar articulación 1 (-9°) |
| **H** | Incrementar articulación 2 (+9°) |
| **J** | Decrementar articulación 2 (-9°) |
| **K** | Incrementar articulación 3 (+9°) |
| **L** | Decrementar articulación 3 (-10°) |
| **Z** | Incrementar articulación 4 (+9°) |
| **X** | Decrementar articulación 4 (-9°) |
| **C** | Incrementar articulación 5 (+9°, máx. 45°) |
| **V** | Decrementar articulación 5 (-9°, mín. 0°) |

### Controles de Movimiento de Objetos

| Tecla | Acción |
|-------|--------|
| **Y** | Mover objeto hacia adelante (-1.0) |
| **U** | Mover objeto hacia atrás (+1.0) |

### Control General

| Tecla | Acción |
|-------|--------|
| **ESC** | Salir de la aplicación |

---

## 🌟 Funcionalidades del Sistema

### 1. Ciclo Día/Noche Automático

El sistema cambia automáticamente entre día y noche cada **5 segundos**.

#### Durante el Día:
- ☀️ Skybox claro y brillante
- 💡 Luz direccional blanca intensa (1.0, 1.0, 1.0)
- 🌤️ Ambiente luminoso y cálido

#### Durante la Noche:
- 🌙 Skybox oscuro y estrellado
- 💙 Luz direccional azulada de baja intensidad (0.3, 0.3, 0.7)
- 🌌 Ambiente nocturno y frío

> **Nota:** El cambio se notifica en la consola con los mensajes:
> - `"Llego el dia"`
> - `"Llego la noche"`

---

### 2. Sistema de Iluminación

El proyecto cuenta con un avanzado sistema de iluminación en tiempo real:

#### Tipos de Luces:
1. **Luz Direccional (mainLight)**
   - Simula la luz del sol/luna
   - Cambia según el ciclo día/noche
   - Ilumina toda la escena uniformemente

2. **Luces Puntuales (PointLights)**
   - Hasta **MAX_POINT_LIGHTS** luces simultáneas
   - Emiten luz en todas direcciones desde un punto
   - Ideal para antorchas, lámparas, etc.

3. **Luces Spot (SpotLights)**
   - Hasta **MAX_SPOT_LIGHTS** luces simultáneas
   - Emiten luz en forma de cono
   - Incluyen:
     - 🔦 Flash de cámara (sigue al jugador)
     - 💡 Linterna de cofre
     - 🚗 Faros delanteros y traseros

#### Control Dinámico de Luces:
El sistema permite encender y apagar luces específicas por nombre sin incrementar innecesariamente los contadores. Las funciones internas son:
- `turnOnPoint(id, count)` - Activa una luz puntual
- `turnOffPoint(id, count)` - Desactiva una luz puntual
- `turnOnSpot(id, count)` - Activa una luz spot
- `turnOffSpot(id, count)` - Desactiva una luz spot

---

### 3. Modelos 3D y Texturas

#### Modelos Cargados:

**Piso (pisot.obj):**
- Representa el suelo del escenario
- Escala: 2.0 x 1.0 x 1.5 (multiplicado por 1.5)
- Posición: Y = -1.5 (debajo del nivel del jugador)

**Antorchas (torchModel):**
- **166 antorchas** distribuidas por toda la escena
- Escala: 5.0 x 5.0 x 5.0
- Coordenadas predefinidas almacenadas en el vector `coords`

#### Sistema de Texturas:
- Las texturas se cargan automáticamente desde archivos `.mtl` de los modelos
- Soporte para formatos: `.png`, `.tga`, `.jpg`
- Textura por defecto: `plain.png` (cuando falla la carga)

---

### 4. Skybox Dinámico

El skybox crea la ilusión de un entorno infinito que rodea la escena.

#### Características:
- 📦 Cubemap con 6 caras (arriba, abajo, norte, sur, este, oeste)
- 🔄 Cambio dinámico de texturas sin recrear el objeto
- 🎨 Diferentes skyboxes para día y noche

#### Funcionamiento:
El método `Skybox.updateTextures()` permite cambiar las texturas del skybox en tiempo real sin perder el rendimiento.

---

### 5. Cámara en Primera Persona

La cámara ofrece control fluido y responsivo.

#### Características:
- **Vista en Primera Persona:** Simula los ojos del jugador
- **Control con Mouse:** Rotación suave 360°
- **Control con Teclado:** Movimiento en 4 direcciones (W, A, S, D)
- **Velocidad Ajustable:** Definida en la inicialización de la cámara

#### Posición y Orientación:
- Posición inicial configurable
- Yaw (rotación horizontal) y Pitch (rotación vertical)
- Límites de Pitch para evitar gimbal lock

---

### 6. Materiales

El sistema utiliza dos tipos de materiales:

1. **Material Brillante (Material_brillante)**
   - Alta intensidad especular
   - Refleja mucho la luz
   - Ideal para metales y superficies pulidas

2. **Material Opaco (Material_opaco)**
   - Baja intensidad especular
   - Refleja poca luz
   - Ideal para madera, piedra, tela

---

## 🔧 Solución de Problemas

### Problema: La ventana no se abre
**Soluciones:**
- ✅ Verifique que las DLLs estén en la carpeta del ejecutable:
  - `glew32.dll`
  - `glfw3.dll`
  - `assimp-vc143-mt.dll`
- ✅ Reinstale los drivers de su tarjeta gráfica
- ✅ Ejecute como administrador

---

### Problema: Modelos no se ven / Pantalla negra
**Soluciones:**
- ✅ Verifique que las carpetas `Models/` y `Shaders/` existan
- ✅ Compruebe que los archivos `.obj` y `.mtl` estén en `Models/`
- ✅ Revise la consola para mensajes de error como:
  ```
  Falló en cargar el modelo: [nombre].obj
  Falló en cargar la Textura: [nombre].png
  ```
- ✅ Asegúrese de que OpenGL 4.3 esté soportado por su GPU

---

### Problema: La cámara se mueve muy rápido/lento
**Solución:**
- Los valores de velocidad se configuran en el código fuente
- Modifique `moveSpeed` y `turnSpeed` en la inicialización de `Camera`

---

### Problema: Errores de compilación
**Soluciones:**
- ✅ Asegúrese de compilar en **x64**, no en x86
- ✅ Verifique que todas las rutas de include estén configuradas
- ✅ Limpie la solución: **Build > Clean Solution**
- ✅ Reconstruya: **Build > Rebuild Solution**

---

### Problema: FPS bajo / Rendimiento lento
**Soluciones:**
- ✅ Cierre otras aplicaciones que usen GPU
- ✅ Actualice los drivers de la tarjeta gráfica
- ✅ Reduzca el número de luces activas
- ✅ Cambie a modo **Release** en lugar de **Debug**

---

### Problema: "Failed to initialize GLFW"
**Soluciones:**
- ✅ Reinstale las librerías de GLFW
- ✅ Verifique compatibilidad con su sistema operativo
- ✅ Ejecute el programa con privilegios de administrador

---

## 📞 Contacto y Soporte

### Colaboradores del Proyecto:
- **Diego Antonio Araiza Valdés**
- **Luis Daniel Salazar Islas**
- **Osbaldo Velazquez Caudillo**

### Repositorio del Proyecto:
🔗 [https://github.com/LuisDakan/PFCGeIHC](https://github.com/LuisDakan/PFCGeIHC)

### Reportar Problemas:
Para reportar bugs o solicitar nuevas características, abra un **Issue** en GitHub:
1. Vaya a la pestaña **Issues**
2. Haga clic en **New Issue**
3. Describa el problema detalladamente
4. Incluya capturas de pantalla si es posible

---

## 📝 Notas Adicionales

### Archivos Importantes:
- `main.cpp` - Punto de entrada y lógica principal
- `Window.cpp/h` - Gestión de ventana y controles
- `Camera.cpp/h` - Control de cámara
- `Model.cpp/h` - Carga de modelos 3D
- `Skybox.cpp/h` - Sistema de skybox
- `Shader_light.vert/frag` - Shaders de iluminación

### Estructura de Carpetas:
```
PFCGEIHC/
├── Models/          # Modelos 3D (.obj, .mtl)
├── Shaders/         # Shaders de OpenGL
├── glm/             # Librería matemática
├── include/         # Headers de librerías
├── lib/             # Librerías compiladas
└── *.cpp, *.h       # Código fuente
```

---

## 🎓 Créditos

**Proyecto Final de Computación Gráfica e Interacción Humano-Computadora**

Desarrollado como parte del curso de Computación Gráfica.

**Inspiraciones:**
- 🎮 Ace Attorney (diseño visual)
- 🦊 Crash Bandicoot (estilo artístico)
- 🎯 Juegos clásicos de SEGA (ambientación)

---

## 📄 Licencia

Este proyecto está bajo la licencia incluida en el archivo `LICENSE` del repositorio.

---

**¡Gracias por usar PFCGeIHC!** 🎉

*Versión del Manual: 1.0*  
*Fecha: Noviembre 2025*
