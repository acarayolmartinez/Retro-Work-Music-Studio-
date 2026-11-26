# ADR 0001: Desktop GUI toolkit

- **Status:** Accepted
- **Date:** 2026-11-26
- **Decision:** Use wxWidgets as the desktop GUI toolkit for RMS.
- **Language constraint:** All RMS application code, including the GUI, remains C++11.

## Context

RMS is a cross-platform desktop music-analysis application. Its planned interface includes menus, dialogs, transport controls, project navigation, settings, lists, and custom audio visualizations. The GUI must remain separate from the audio, DSP, project, and analysis layers.

The initial candidates were wxWidgets and FLTK. Qt 6 is not selected because the project-wide C++11 requirement takes precedence.

## Evaluation

### wxWidgets

- Cross-platform support for Windows, macOS, and Unix-like systems, including Linux through wxGTK.
- Current wxWidgets development documentation explicitly supports C++11 compilers. The stable 3.2 line should be preferred over the development line for the initial integration.
- Provides a broad set of desktop controls and uses native platform controls where available.
- Uses a modified LGPL licence with an exception that permits distributing applications without publishing their source, including when statically linking the library. RMS must still retain and comply with the applicable third-party licence notices and terms.

### FLTK

- Cross-platform support for Linux/Unix, Windows, and macOS.
- Lightweight and suitable for compact desktop applications.
- FLTK 1.5 requires C++11 and CMake. The exact FLTK release and its compiler requirements would need to be pinned and revalidated before adoption.
- Uses the GNU Library General Public License with exceptions that permit static linking.

## Decision rationale

Choose wxWidgets because RMS is expected to grow into a multi-panel desktop editor rather than a small utility window. Its broader native desktop control set should reduce the amount of custom UI infrastructure RMS must maintain. FLTK remains a reasonable alternative for projects where minimal footprint and a smaller widget set are higher priorities.

The choice does not mean that RMS should use wxWidgets for domain logic or audio rendering. The waveform, piano-roll-like views, timeline, and other specialized editors remain RMS-owned custom controls or drawing components.

## Architectural constraints

1. wxWidgets types must not leak into `rms_core`, DSP, audio, project serialization, or analysis APIs.
2. GUI code belongs in a separate desktop target and source tree.
3. GUI event handlers should call application-facing controllers/use cases rather than implement audio or analysis logic.
4. Keep the required C++ standard at C++11 for every RMS target.
5. Start with the stable wxWidgets 3.2 series. Pin and document the exact tested version when the dependency is integrated.
6. Do not introduce wxWidgets into the default build until the dependency is available and the GUI target has its own build/test validation.

## Consequences

- The desktop layer will use wxWidgets.
- The core and existing tests remain buildable without a GUI toolkit.
- The initial GUI integration will be optional in CMake, so headless builds and tests remain available.
- A later integration task must verify the installed wxWidgets package, CMake discovery, C++11 compilation, and runtime startup on Linux Mint before adding the dependency to the default developer workflow.

## References

- wxWidgets project and platform/compiler information: https://github.com/wxWidgets/wxWidgets
- wxWidgets stable releases: https://wxwidgets.org/downloads/
- wxWidgets overview: https://wxwidgets.org/about/
- FLTK project: https://www.fltk.org/
- FLTK 1.5 build prerequisites: https://github.com/fltk/fltk
