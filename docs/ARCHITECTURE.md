# Architecture / Arquitectura

**Status:** Initial direction — implementation details remain open.  
**Estado:** Orientación inicial — los detalles de implementación siguen pendientes.

## Goals / Objetivos

- Cross-platform desktop application implemented entirely in C++11.
- Keep domain logic independent from UI frameworks and audio backends.
- Make analysis algorithms testable without audio hardware.
- Allow audio and export implementations to evolve behind interfaces.
- Require C++11 compatibility from all compiled application modules and selected dependencies.

## Proposed modules / Módulos propuestos

- **Core / Domain** — project model, tracks, analysis results, tempo/key concepts, and shared types.
- **Application** — use cases, orchestration, commands, and session workflows.
- **Audio** — decoding, playback, device access, and audio abstractions.
- **DSP** — FFT, filters, onset detection, pitch and tempo estimation.
- **Analyzer** — analysis pipelines and result interpretation.
- **Desktop** — native desktop interface and presentation logic.
- **Infrastructure** — file system, serialization, logging, and persistence.

Suggested source layout:

```text
include/
src/
  core/
  audio/
  dsp/
  analyzer/
  desktop/
tests/
docs/
CMakeLists.txt
```

## Technology direction / Orientación tecnológica

| Area | Candidate | Notes |
|---|---|---|
| Language | C++11 | Required project-wide, including desktop UI |
| Build | CMake | Cross-platform build configuration |
| Audio I/O | PortAudio | Candidate backend; validate supported platforms |
| DSP | Custom C++11 modules | Keep algorithms testable and independent |
| FFT | Own implementation or FFTW3 | Decide after evaluating requirements and integration |
| Desktop UI | wxWidgets or FLTK | Evaluate C++11 support, accessibility, and platform behavior |
| Tests | CTest + Catch2 v2 or another C++11-compatible framework | Verify exact dependency versions |
| Serialization | JSON or a documented container | Select library only after C++11 compatibility review |

These are candidates, not final dependency commitments. Qt 6 is not selected because the project requires C++11; any GUI toolkit must satisfy that constraint.

## Build standard / Estándar de compilación

CMake configuration must explicitly request C++11 and disable compiler-specific language extensions where practical. CI should build with more than one supported compiler and treat accidental use of newer C++ features as a failure.

## Project file / Archivo de proyecto

The proposed native extension is `.rms`. Initially it may be a documented JSON-based container or manifest. The exact schema, version field, asset handling, and migration strategy must be specified before implementation.

## Important constraints / Restricciones

- Do not couple domain models to desktop UI controls.
- Do not assume one audio backend works identically on Windows, Linux, and macOS.
- Keep DSP algorithms deterministic where possible.
- Record sample rate, channel count, and analysis parameters with results.
- Do not introduce dependencies that silently raise the required C++ standard.
- Keep platform-specific code behind narrow interfaces.

## Open decisions / Decisiones pendientes

- Desktop UI toolkit: wxWidgets, FLTK, or another verified C++11-compatible option.
- Audio backend details and supported operating systems.
- Audio file formats and decoding dependencies.
- Internal sample representation and precision.
- FFT implementation and licensing/dependency implications.
- `.rms` container and compatibility policy.
- License and third-party dependency policy.
