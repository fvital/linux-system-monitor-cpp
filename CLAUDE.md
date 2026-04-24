# Linux System Monitor — Project Context

## Overview
Terminal-based Linux system monitor written in C++. Built as part of the
Udacity C++ Nanodegree (Object-Oriented Programming module, course cd0424)
and used as a portfolio piece targeting robotics and embedded systems roles.

Original starter code by Udacity:
https://github.com/udacity/cd0424-object-oriented-programming-project

## AI Assistance Policy

This project is subject to the [Udacity Honor Code](https://www.udacity.com/legal/honor-code)
AI usage policy. All implementation files reflect the student's own independent
work. AI tools are used as reference aids — in the same capacity as
documentation, tutorials, or Stack Overflow — not as a substitute for
independent implementation.

**Permitted AI uses in this project:**
- Reviewing and giving feedback on student-written code
- Explaining concepts and debugging
- Reviewing and reorganizing non-implementation files (CMakeLists, docs)
- Generating commit messages from student-authored changes

**Not permitted:**
- Writing `.cpp` or `.h` implementation files on the student's behalf
- Completing any assessed project deliverable

**Exception:** `CLAUDE.md` is AI-generated and is not part of any assessed
deliverable.

## Project Structure
See README.md for full file listing. Key constraints:
- `ncurses_display.h` / `ncurses_display.cpp` — provided by Udacity,
  do not modify
- `main.cpp` — provided by Udacity; treat as stable and only suggest
  changes if strictly required by a specific extension, with justification
- All other `.cpp` files and headers are mine to implement
- `src/CMakeLists.txt` — builds `monitor_lib` (library for tests) and `monitor` executable
- `tests/CMakeLists.txt` — GoogleTest suite; links against `monitor_lib`


## Build
```bash
# Make
make build
./build/src/monitor

# CMake (via CMakePresets.json)
cmake --preset default
cmake --build --preset default
./build/src/monitor

# Run tests
ctest --preset default
```

## Planned Extensions
- [ ] Dynamic CPU utilization (delta between samples, not cumulative)
- [x] CMake modernization (explicit source listing, verify presets)
- [ ] Abstract base classes with pure virtual functions for portability
- [ ] `const` correctness audit
- [ ] GoogleTest unit tests (behavior-based, no hardcoded machine values)
- [ ] GitHub Actions CI with `-Werror`

## Testing Philosophy
Tests must be machine-independent. No hardcoded expected values tied to
this specific machine or OS installation. Acceptable patterns:
- Assert value is within a valid range (e.g. memory utilization 0.0–1.0)
- Assert structural properties (e.g. PID 1 always exists)
- Assert non-empty returns for always-populated fields
- Fixture-file based tests for parsing logic