# Versioning / Versionado

RMS follows Semantic Versioning where practical: `MAJOR.MINOR.PATCH`.

- **MAJOR**: incompatible public API, project format, or workflow changes.
- **MINOR**: backward-compatible functionality.
- **PATCH**: backward-compatible fixes and documentation corrections.

## Pre-release development

Before 1.0.0, minor versions may include changes to internal APIs and project structure. Compatibility guarantees will be explicitly documented.

## Project file compatibility

The application version and `.rms` document-format version should be tracked separately. A future project manifest should include a format version so RMS can detect unsupported files and provide migrations.

## Changelog

Every release should have a corresponding entry in `CHANGELOG.md`, including notable additions, changes, fixes, and compatibility notes.

## Current baseline

**v0.1.0 — Genesis / Project Bootstrap**  
Documentation and repository foundation only; no functional audio application is included.
