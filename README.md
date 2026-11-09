# RetroWorks Music Studio (RMS)

**A retro-focused music analysis and production toolkit.**  
**Una suite retro de análisis y producción musical.**

[English](#english) · [Español](#español)

## English

RetroWorks Music Studio (RMS) is a cross-platform project focused on music analysis and, over time, workflows for creating and adapting music for classic video game platforms.

The first milestone is a **Music Analyzer** that can import audio and help estimate tempo (BPM), key, and melodic information. Future stages may explore export workflows for tracker formats and target platforms such as Game Boy, NES, Mega Drive/Genesis, and SNES.

### Project status

**v0.1.0 — Genesis / Project Bootstrap**

This initial milestone establishes project vision, architecture direction, contribution guidelines, and development roadmap. It does not yet include an audio engine or working analyzer.

### Planned technology direction

- C# and .NET
- Avalonia UI for a cross-platform desktop interface
- MVVM architecture
- Audio I/O abstraction, with NAudio as a possible implementation
- Custom DSP and FFT components
- xUnit for tests
- JSON for configuration and project metadata
- Possible ONNX Runtime integration in a later phase

These are provisional choices and may change as the project evolves.

### Project format

The proposed native project extension is `.rms`. Its structure and compatibility rules will be defined before implementation.

## Español

RetroWorks Music Studio (RMS) es un proyecto multiplataforma centrado en el análisis musical y, progresivamente, en flujos de trabajo para crear y adaptar música a plataformas clásicas de videojuegos.

El primer hito será un **Music Analyzer** capaz de importar audio y ayudar a estimar el tempo (BPM), la tonalidad y elementos melódicos. En fases futuras se estudiará la exportación a formatos tracker y plataformas como Game Boy, NES, Mega Drive/Genesis y SNES.

### Estado del proyecto

**v0.1.0 — Genesis / Project Bootstrap**

Este hito inicial establece la visión, la orientación arquitectónica, las pautas de contribución y la hoja de ruta. Todavía no incluye un motor de audio ni un analizador funcional.

### Orientación tecnológica prevista

- C# y .NET
- Avalonia UI para una interfaz de escritorio multiplataforma
- Arquitectura MVVM
- Abstracción de entrada/salida de audio, con NAudio como posible implementación
- Componentes DSP y FFT propios
- xUnit para pruebas
- JSON para configuración y metadatos
- Posible integración de ONNX Runtime en una fase posterior

Estas decisiones son provisionales y podrán cambiar durante el desarrollo.

### Formato de proyecto

La extensión nativa propuesta es `.rms`. Su estructura y reglas de compatibilidad se definirán antes de implementarla.

## Repository structure / Estructura del repositorio

```text
.
├── docs/
│   ├── ARCHITECTURE.md
│   ├── VERSIONING.md
│   └── VISION.md
├── .gitignore
├── CHANGELOG.md
├── CONTRIBUTING.md
├── README.md
└── ROADMAP.md
```

## License / Licencia

No license has been selected yet. Until one is added, all rights remain with the copyright holder.  
Todavía no se ha seleccionado una licencia. Hasta que se añada, todos los derechos quedan reservados al titular.

## Contributions / Contribuciones

See [CONTRIBUTING.md](CONTRIBUTING.md). / Consulta [CONTRIBUTING.md](CONTRIBUTING.md).
