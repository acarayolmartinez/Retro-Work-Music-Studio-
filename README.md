# RetroWorks Music Studio (RMS)

**A retro-focused music analysis and production toolkit.**  
**Una suite retro de análisis y producción musical.**

[English](#english) · [Español](#español)

## English

RetroWorks Music Studio (RMS) is a cross-platform project focused on music analysis and, over time, workflows for creating and adapting music for classic video game platforms.

The first milestone is a **Music Analyzer** that can import audio and help estimate tempo (BPM), key, and melodic information. Future stages may explore export workflows for tracker formats and target platforms such as Game Boy, NES, Mega Drive/Genesis, and SNES.

### Project status

**v0.2.0 — Application Skeleton (in progress)**

The repository now contains an initial CMake-based C++11 executable and a small core application module. Audio, DSP, analysis, and desktop UI functionality have not been implemented yet.

### Technology direction

The entire application, including its desktop interface, will be developed in **C++11**.

- C++11 as the required language standard across all RMS modules.
- CMake for cross-platform build configuration.
- A modular architecture separating core/domain, audio, DSP, analysis, and desktop UI.
- PortAudio as the initial candidate for cross-platform audio I/O.
- Custom DSP utilities, with an FFT implementation to be selected (own implementation or a compatible library).
- A C++11-compatible desktop UI toolkit; wxWidgets and FLTK are candidates to evaluate.
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

El repositorio ya contiene un ejecutable inicial C++11 basado en CMake y un pequeño módulo de aplicación principal. Todavía no se han implementado las funciones de audio, DSP, análisis ni interfaz gráfica.

### Orientación tecnológica

Toda la aplicación, incluida la interfaz de escritorio, se desarrollará en **C++11**.

- C++11 como estándar obligatorio en todos los módulos de RMS.
- CMake para configurar compilaciones multiplataforma.
- Arquitectura modular que separa núcleo/dominio, audio, DSP, análisis e interfaz de escritorio.
- PortAudio como primera opción candidata para entrada/salida de audio multiplataforma.
- Utilidades DSP propias y una implementación FFT aún por decidir (propia o mediante una biblioteca compatible).
- Biblioteca de interfaz de escritorio compatible con C++11; wxWidgets y FLTK son candidatas para evaluar.
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
