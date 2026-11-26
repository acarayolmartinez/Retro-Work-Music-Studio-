# RetroWorks Music Studio (RMS)

**A retro-focused music analysis and production toolkit.**  
**Una suite retro de análisis y producción musical.**

[English](#english) · [Español](#español)

## English

RetroWorks Music Studio (RMS) is a cross-platform project focused on music analysis and, over time, workflows for creating and adapting music for classic video game platforms.

The first milestone is a **Music Analyzer** that can import audio and help estimate tempo (BPM), key, and melodic information. Future stages may explore export workflows for tracker formats and target platforms such as Game Boy, NES, Mega Drive/Genesis, and SNES.

### Project status

**v0.2.0 — Application Skeleton (in progress)**

The repository contains a C++11 audio-analysis core, WAV import, waveform data and viewport models, project management, and an optional wxWidgets desktop application. The desktop window can now open WAV files and display their waveform and basic audio metadata.

### Technology direction

The entire application, including its desktop interface, will be developed in **C++11**.

- C++11 as the required language standard across all RMS modules.
- wxWidgets 3.2 or newer (within the wxWidgets 3.x line) for the optional native desktop target.
- CMake for cross-platform build configuration.
- A modular architecture separating core/domain, audio, DSP, analysis, and desktop UI.
- PortAudio as the initial candidate for cross-platform audio I/O.
- Custom DSP utilities, with an FFT implementation to be selected (own implementation or a compatible library).
- wxWidgets 3.2 for the optional native desktop interface.
- CTest plus a C++11-compatible testing framework.
- Dependencies must be checked for C++11 compatibility before adoption.

No C#/.NET or separate-language UI is planned.

### Build

Requirements: CMake 3.10 or newer and a C++ compiler with C++11 support.

```sh
cmake -S . -B build
cmake --build build
./build/rms
```

On Windows, run the generated executable from the build directory (for example, `build\Debug\rms.exe` with a multi-configuration generator).


### Desktop GUI (optional)

The native desktop application is an optional target and requires wxWidgets 3.2.

On Linux Mint 22.x / Ubuntu 24.04, install the development package:

```sh
sudo apt update
sudo apt install libwxgtk3.2-dev
```

Configure and build the desktop application:

```sh
cmake -S . -B build-desktop -DRMS_BUILD_DESKTOP=ON
cmake --build build-desktop -j$(nproc)
./build-desktop/rms_desktop
```

The default build keeps `RMS_BUILD_DESKTOP=OFF`, so the core application and tests do not require wxWidgets. In the desktop app, choose **File → Open Audio…** to load a PCM WAV file and view its waveform.

Real playback is optional and uses PortAudio. On Linux Mint 22.x / Ubuntu 24.04, install its development package:

```sh
sudo apt install portaudio19-dev
```

Then configure the desktop target with audio output enabled:

```sh
cmake -S . -B build-desktop \
  -DRMS_BUILD_DESKTOP=ON \
  -DRMS_ENABLE_PORTAUDIO=ON
cmake --build build-desktop -j$(nproc)
./build-desktop/rms_desktop
```

When PortAudio is disabled, RMS can still import and display WAV files, but the Play and Stop controls remain unavailable.

### Source layout

```text
include/rms/core/   Public core headers
src/core/           Core implementation
src/main.cpp        Application entry point
tests/              Test build configuration (tests to be added)
docs/               Project documentation
```

## Español

RetroWorks Music Studio (RMS) es un proyecto multiplataforma centrado en el análisis musical y, progresivamente, en flujos de trabajo para crear y adaptar música a plataformas clásicas de videojuegos.

El primer hito será un **Music Analyzer** capaz de importar audio y ayudar a estimar el tempo (BPM), la tonalidad y elementos melódicos. En fases futuras se estudiará la exportación a formatos tracker y plataformas como Game Boy, NES, Mega Drive/Genesis y SNES.

### Estado del proyecto

**v0.2.0 — Application Skeleton (en desarrollo)**

El repositorio contiene un núcleo de análisis de audio en C++11, importación WAV, modelos de forma de onda y viewport, gestión de proyectos y una aplicación de escritorio opcional con wxWidgets. La ventana ya permite abrir archivos WAV y mostrar su forma de onda y metadatos básicos.

### Orientación tecnológica

Toda la aplicación, incluida la interfaz de escritorio, se desarrollará en **C++11**.

- C++11 como estándar obligatorio en todos los módulos de RMS.
- CMake para configurar compilaciones multiplataforma.
- Arquitectura modular que separa núcleo/dominio, audio, DSP, análisis e interfaz de escritorio.
- PortAudio como primera opción candidata para entrada/salida de audio multiplataforma.
- Utilidades DSP propias y una implementación FFT aún por decidir (propia o mediante una biblioteca compatible).
- wxWidgets 3.2 para la interfaz de escritorio nativa opcional.
- CTest y un framework de pruebas compatible con C++11.
- Se comprobará la compatibilidad con C++11 de cada dependencia antes de incorporarla.

No está previsto utilizar C#/.NET ni desarrollar la interfaz en otro lenguaje.

### Compilación

Requisitos: CMake 3.10 o superior y un compilador compatible con C++11.

```sh
cmake -S . -B build
cmake --build build
./build/rms
```

En Windows, ejecuta el archivo generado dentro de la carpeta de compilación (por ejemplo, `build\Debug\rms.exe` con un generador multiconfiguración).

La aplicación de escritorio permite seleccionar **File → Open Audio…** para cargar un archivo WAV PCM y visualizar su forma de onda.

La reproducción real es opcional y utiliza PortAudio. En Linux Mint 22.x / Ubuntu 24.04, instala sus cabeceras de desarrollo:

```sh
sudo apt install portaudio19-dev
```

Después configura la aplicación de escritorio con la salida de audio activada:

```sh
cmake -S . -B build-desktop \
  -DRMS_BUILD_DESKTOP=ON \
  -DRMS_ENABLE_PORTAUDIO=ON
cmake --build build-desktop -j$(nproc)
./build-desktop/rms_desktop
```

Si PortAudio está desactivado, RMS seguirá pudiendo importar WAV y mostrar su forma de onda, pero los controles Play y Stop no estarán disponibles.

### Estructura del código

```text
include/rms/core/   Cabeceras públicas del núcleo
src/core/           Implementación del núcleo
src/main.cpp        Punto de entrada
tests/              Configuración de pruebas (pendientes)
docs/               Documentación del proyecto
```

### Formato de proyecto

La extensión nativa propuesta es `.rms`. Su estructura y reglas de compatibilidad se definirán antes de implementarla.

## License / Licencia

No license has been selected yet. Until one is added, all rights remain with the copyright holder.  
Todavía no se ha seleccionado una licencia. Hasta que se añada, todos los derechos quedan reservados al titular.

## Contributions / Contribuciones

See [CONTRIBUTING.md](CONTRIBUTING.md). / Consulta [CONTRIBUTING.md](CONTRIBUTING.md).
