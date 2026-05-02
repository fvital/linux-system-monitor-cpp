# Linux System Monitor

A terminal-based Linux system monitor written in C++, displaying real-time
CPU utilization, memory usage, and per-process statistics using an ncurses UI.

*Target output:*
![System Monitor Screenshot](images/monitor(original_target).png)


*Current output:*
![System Monitor Screenshot](images/monitor(current).png)

---

## Attribution

This project is based on the starter code provided by Udacity for course
**cd0424 — Object-Oriented Programming in C++**. The original starter
repository is available at:
https://github.com/udacity/cd0424-object-oriented-programming-project

Starter code © 2012–2020 Udacity, Inc., licensed under
[CC BY-NC-ND 4.0](http://creativecommons.org/licenses/by-nc-nd/4.0).
All implementation work in this repository is my own.

---

## Project Structure

```
.
├── CMakeLists.txt                  # Root build; minimum CMake 3.22, fetches GoogleTest
├── CMakePresets.json               # Out-of-source build presets (default output: build/)
├── Makefile                        # Convenience wrapper around cmake commands
├── README.md                       
├── include/    
│   ├── format.h                    # Time formatting utilities 
│   ├── linux_parser.h              # Namespace for /proc filesystem parsing
│   ├── meminfo.h                   # MemInfo class — /proc/meminfo parser
│   ├── ncurses_display.h           # Terminal UI declarations 
│   ├── process.h                   # Per-process abstraction
│   ├── processor.h                 # CPU abstraction
│   ├── stat.h                      # CpuStat and ProcessStat — /proc/stat parsers
│   └── system.h                    # Top-level system abstraction
├── src/        
│   ├── CMakeLists.txt              # Builds monitor_lib (shared with tests) and monitor
│   ├── format.cpp                  # Time formatting implementation
│   ├── linux_parser.cpp            # /proc parsing implementation
│   ├── main.cpp                    # Entry point 
│   ├── meminfo.cpp                 # MemInfo parsing implementation
│   ├── ncurses_display.cpp         # Terminal UI implementation
│   ├── process.cpp                 # Process attribute accessors
│   ├── processor.cpp               # CPU utilization calculation
│   ├── stat.cpp                    # CpuStat and ProcessStat implementation
│   └── system.cpp                  # System-wide data aggregation
├── tests/    
│   ├── CMakeLists.txt              # GoogleTest executable; injects fixture path at compile time
│   ├── fixtures/                   # Static /proc snapshots for fixture-based tests
│   │   ├── etc/    
│   │   │   ├── os-release    
│   │   │   └── passwd    
│   │   └── proc/   
│   │       ├── meminfo   
│   │       ├── stat    
│   │       ├── uptime    
│   │       ├── version   
│   │       ├── 1234/               # process fixture
│   │       │   ├── stat    
│   │       │   └── status    
│   │       └── 2599/               # process fixture
│   │           ├── cmdline   
│   │           ├── stat    
│   │           └── status    
│   ├── format_test.cpp   
│   ├── linux_parser_test.cpp   
│   ├── meminfo_test.cpp    
│   ├── process_test.cpp    
│   ├── processor_test.cpp    
│   ├── stat_test.cpp   
│   └── system_test.cpp   
└── images/   
    ├── monitor(current).png    
    └── monitor(original_target).png    # Reference screenshot
```

---

## Implementation Status

### Core Classes and Namespaces

- [x] **`LinuxParser` namespace** — parses `/proc` and `/etc` filesystem entries
  to provide system-wide data (OS, kernel, uptime, memory utilization, total
  and running process counts, aggregate CPU jiffie counts across all states)
  and per-process data (PID list, command, RAM, UID, username, uptime, and
  per-process active jiffies for CPU utilization calculation)
- [x] **`Format` namespace** — converts elapsed time in seconds to a
  `HH:MM:SS` string (e.g. `30:48:17`) used by the display layer for system
  and per-process uptime
- [x] **`Processor` class** — computes and exposes aggregate CPU utilization
  as a float, using `LinuxParser` CPU data internally
- [x] **`Process` class** — exposes per-process attributes (PID, user, command,
  CPU utilization, RAM, uptime) and implements `operator<` for sorting the
  process list
- [x] **`System` class** — top-level aggregator; owns the `Processor` and
  `Process` list, and exposes OS name, kernel version, memory utilization,
  total process count, running process count, and uptime

`NCursesDisplay` is provided in full and drives the terminal UI — it is not
modified as part of this project.

### Extensions *(planned)*

- [ ] **Dynamic CPU utilization** — compute utilization from a rolling delta
  between two samples rather than a cumulative average from boot
- [x] **CMake modernization** — the starter `CMakeLists.txt` has several issues
  worth addressing: `cmake_minimum_required` is set to 2.6 (circa 2008),
  `file(GLOB ...)` is used for sources (explicitly discouraged by CMake —
  silent build failures when files are added or removed), and there is no
  `CMakePresets.json` for out-of-source builds. Plan: bump minimum to 3.24,
  replace glob with explicit source listing, add `CMakePresets.json`
- [ ] **Abstract base classes** — introduce interfaces via abstract classes and
  pure virtual functions to decouple the display layer from Linux-specific
  implementations, making the design more portable
- [x] **`const` correctness audit** — all getters `const`-qualified, read-only
  parameters passed as `const&`
- [x] **Unit tests** — GoogleTest fixture-based tests covering `LinuxParser`,
  `MemInfo`, `CpuStat`, `ProcessStat`, `Processor`, `Process`, `System`, and
  `Format`. All tests are machine-independent (see *What I Implemented* below).
- [ ] **GitHub Actions CI** — automated build on push, with `-Werror` enabled
  (fulfilling the existing TODO comment in `CMakeLists.txt`)

---

## Dependencies

| Dependency | Notes |
|---|---|
| GCC / G++ with C++17 support | GCC 7 or later; verify with `g++ --version` |
| CMake | 3.24 or later; verify with `cmake --version` |
| ncurses | Install via `sudo apt install libncurses5-dev libncursesw5-dev` |
| Make | Standard on most Linux distributions |
| Linux kernel (any version) | Required — this project reads from `/proc` directly |

---

## Build & Run

> ⚠️ This project targets **Linux only**. It reads directly from the `/proc`
> virtual filesystem and will not build or run on macOS or Windows.

```bash
# Build
make build

# Build (CMake via CMakePresets.json)
cmake --preset default
cmake --build --preset default

# Run
./build/src/monitor

# Run tests
ctest --preset default

# Debug build (includes debug symbols)
make debug

# Apply clang-format
make format

# Clean build artifacts
make clean
```

---

## What I Implemented

### Injectable root path — the testability spine

Every function that reads from the filesystem accepts a `const std::string& root`
parameter defaulting to `"/"`. This means any test can pass a fixture directory
instead of the live root, making the entire test suite machine-independent without
a mocking framework.

To prevent tests from passing accidentally, string identifiers that are stable 
across reboots — OS name, kernel version, and process names — are replaced with 
clearly synthetic values ("Test Ubuntu 22.04.5 LTS", "0.0.0-kernelTest", 
test-entry0). Numeric fields (memory sizes, jiffy counts, timing values) are left 
at their original captured values: these change with every reboot, so an 
accidental match would require both the exact numeric value and the PID to match 
simultaneously, which is effectively impossible for non-fixed process IDs.

The fixture path itself is injected at compile time via a `TEST_FIXTURE_DIR` CMake
definition, so no paths are hardcoded in test source.

### MemInfo — /proc/meminfo parsing

Rather than extending `LinuxParser::MemoryUtilization()` to return multiple
fields, I introduced `MemInfo` — a struct whose public members map directly to
`/proc/meminfo` keys. Parsing is done via a `std::unordered_map<std::string,
float*>` that maps key strings to pointers to the corresponding struct members.
An `operator>>(std::ifstream&, MemInfo&)` overload drives the parse loop; a
`find()` check before dereferencing guards against unknown or platform-specific
keys.

`LinuxParser::MemoryUtilization()` is retained but marked `[[deprecated]]` for
interface compatibility with the original starter contract.

### CpuStat and ProcessStat — /proc/stat parsing

`CpuStat` encapsulates one `cpu*` line from `/proc/stat` with typed fields and
an `operator>>(std::istringstream&, CpuStat&)` overload for deserialization.

`ProcessStat` stores all 52 whitespace-separated fields of `/proc/[pid]/stat` as
strings in a `std::array`, with typed accessor methods that convert on demand. A
`ProcPidStatIdx` enum class (defined in `stat.cpp`, not exported) maps field names
to their 0-based positions per the `proc(5)` kernel ABI, replacing magic indices.
Time accessor methods accept a `bool in_secs` parameter — `false` returns raw
clock ticks, `true` divides by `sysconf(_SC_CLK_TCK)`.

### Promoting UpTime from long to double

Since `/proc/uptime` provides values with sub-second precision, and other 
time-related data given in jiffies or clock-ticks will also generate sub-second 
values when converted, `LinuxParser::UpTime()` was promoted to `double`. 
Since `ProcessStat::uptime()` and `utilization()` subtract and divide system uptime 
to produce per-process figures, rounding errors could accumulate, so all internal 
calculations are performed using `double` and converted to the desired datatype at 
the destination.

### Process — three-file parsing per process

Each `Process` reads from three files on construction and each refresh:

- `/proc/[pid]/status` — state character, real UID, and VmSize (virtual memory, kB)
- `/proc/[pid]/stat` — timing fields via `ProcessStat` (utime, stime, cutime,
  cstime, starttime)
- `/proc/[pid]/cmdline` — the command string

RAM is exposed in MB (`vm_size_ / 1024.0`) via `Ram(int precision = 2)` with
configurable decimal precision. `operator<` sorts by descending CPU utilization.
User lookup uses a `user_map_` passed in by `System` (built once from `/etc/passwd`), 
avoiding a re-read of the `passwd` file per process. The resolved username is stored 
in a private `user_` member and re-resolved from the map on each `update_data()` call.

### System — process lifecycle management

`Processes()` calls two private helpers on every refresh:

- `clean_process_list()` — removes `Process` objects whose PID no longer appears
  in `/proc`
- `update_process_list()` — adds `Process` objects for new PIDs; calls
  `update_data()` on existing ones

`Kernel()` and `OperatingSystem()` are lazily cached — read once on the first
call, then returned from member strings on subsequent calls. The user map is built 
once in the constructor and passed to `Process` objects on construction and on each 
`update_data()` call. `update_user_map()` exists as a private method but is not 
currently called on refresh cycles, so user account changes during a session would 
not be reflected without a restart.

### Processor — single-pass /proc/stat parsing

The original `LinuxParser` approach would read `/proc/stat` separately for each metric — aggregate jiffies, active jiffies, idle jiffies — opening the file multiple times per refresh cycle. `Processor::update_stat()` reads it once per call, capturing the aggregate cpu line, all `cpu0...cpuN` lines into a `std::vector<CpuStat>` indexed by cpu number, and the procs_running and cumulative fork count fields, eliminating the need for `LinuxParser::RunningProcesses()`.

Note: `System::TotalProcesses()` counts numeric directories in `/proc` via `LinuxParser::Pids()` rather than using the processes field from `/proc/stat`, which counts all forks since boot rather than currently-alive processes.

`Utilization()` is const and does not re-read the file — the caller controls the refresh rate by calling `update_stat()` explicitly.

### [[deprecated]] strategy for LinuxParser

The starter code's per-process free functions (`Command`, `Ram`, `Uid`, `User`,
`UpTime(pid)`, `ActiveJiffies(pid)`, `RunningProcesses`) are retained in the
`LinuxParser` namespace but marked `[[deprecated]]`. The `Process` and `Processor`
classes provide this data through a proper object interface. Retaining the
declarations preserves the original namespace structure while discouraging use of
the flat-function path.

---

## License

My implementation code in this repository is released under the
[MIT License](https://opensource.org/licenses/MIT).

The underlying starter code remains subject to the original
[Udacity license](https://github.com/udacity/cd0424-object-oriented-programming-project/blob/main/LICENSE.txt) (CC BY-NC-ND 4.0).
