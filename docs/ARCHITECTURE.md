# Architecture / Arquitectura

**Status:** Provisional — subject to change.  
**Estado:** Provisional — sujeto a cambios.

## Goals / Objetivos

- Cross-platform desktop application.
- Keep domain logic independent from UI frameworks and audio backends.
- Make analysis algorithms testable without audio hardware.
- Allow audio and export implementations to evolve behind interfaces.

## Proposed layers / Capas propuestas

1. **Presentation** — Avalonia UI, MVVM, navigation, view models.
2. **Application** — use cases, orchestration, commands, and session workflows.
3. **Domain** — tracks, projects, analysis results, tempo/key concepts, and rules.
4. **Audio** — decoding, playback, device access, and audio abstractions.
5. **DSP** — FFT, filters, onset detection, pitch and tempo estimation.
6. **Infrastructure** — file system, JSON serialization, logging, and persistence.

## Technology direction / Orientación tecnológica

| Area | Candidate | Notes |
|---|---|---|
| Language | C# | Primary implementation language |
| Runtime | .NET | Target framework to be selected |
| UI | Avalonia UI | Cross-platform desktop |
| Pattern | MVVM | Presentation separation |
| Audio | NAudio / abstractions | Backend choice requires platform validation |
| DSP | Custom components | FFT and analysis utilities |
| Tests | xUnit | Unit and integration tests |
| Serialization | JSON | Initial metadata/configuration format |
| ML (future) | ONNX Runtime | Optional, not part of bootstrap |

## Project file / Archivo de proyecto

The proposed native extension is `.rms`. Initially it may be a documented JSON-based container or manifest. The exact schema, version field, asset handling, and migration strategy must be specified before implementation.

## Important constraints / Restricciones

- Do not couple domain models to Avalonia controls.
- Do not assume one audio backend works identically on Windows, Linux, and macOS.
- Keep DSP algorithms deterministic where possible.
- Record sample rate, channel count, and analysis parameters with results.
- Treat ML-assisted features as optional future capabilities.

## Open decisions / Decisiones pendientes

- Supported .NET target framework.
- Audio backend per operating system.
- Audio file formats and decoding dependencies.
- Internal sample representation and precision.
- `.rms` container and compatibility policy.
- License and third-party dependency policy.
