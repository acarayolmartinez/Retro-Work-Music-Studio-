# Roadmap / Hoja de ruta

The roadmap is directional, not a promise of delivery dates.  
La hoja de ruta marca una dirección; no constituye un compromiso de fechas.

## v0.1.0 — Genesis / Project Bootstrap

- [x] Establish repository and initial documentation
- [x] Define project vision and initial architecture direction
- [x] Document contribution and versioning conventions
- [x] Set C++11 as the language standard for the entire application
- [ ] Select and add an open-source license

## v0.2.0 — Application Skeleton

- [x] Create the CMake project with C++11 explicitly enforced
- [x] Add the initial core module and executable entry point
- [x] Add compiler warnings and a basic build configuration
- [ ] Establish audio, DSP, analyzer, and desktop UI modules
- [ ] Evaluate and select a C++11-compatible desktop UI toolkit
- [ ] Add automated build and test workflow
- [ ] Add unit tests and select a C++11-compatible test framework
- [ ] Define the first `.rms` project-file specification

## v0.3.0 — Audio Foundation

- [ ] Evaluate and integrate PortAudio for supported platforms
- [ ] Define audio import and decoding abstractions
- [ ] Support basic file metadata and waveform display
- [ ] Add audio transport controls
- [ ] Add unit tests for audio-domain components

## v0.4.0 — Analysis Foundations

- [ ] Implement C++11 DSP utilities
- [ ] Select or implement an FFT
- [ ] Explore BPM estimation
- [ ] Explore key and pitch estimation
- [ ] Document accuracy limitations and test datasets

## v0.5.0 — Music Analyzer Preview

- [ ] Integrate analysis results into the desktop UI
- [ ] Provide editable annotations and analysis summaries
- [ ] Save and reopen analysis sessions
- [ ] Prepare an internal preview

## Future exploration

- Melody extraction and transcription assistance
- Tracker-oriented export workflows, including research into WYZTracker compatibility
- Platform-specific music workflows: Game Boy, NES, Mega Drive/Genesis, and SNES
- Optional ML-assisted analysis only if a suitable C++ integration preserves the project's build and portability goals
