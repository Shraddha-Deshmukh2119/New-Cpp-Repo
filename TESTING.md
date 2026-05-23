# Testing & Code Coverage Guide

This project uses **Google Test**, **CMake**, and **gcovr** (MinGW on Windows). The `Jenkinsfile` runs the same flow on a Windows Jenkins agent.

---

## Prerequisites

| Tool | Purpose |
|------|---------|
| [MSYS2](https://www.msys2.org/) + `mingw-w64` | `g++`, `ninja`, `gcovr` |
| CMake ≥ 3.16 | Configure and build |
| Python 3 + pip | `pip install gcovr` |
| Jenkins (optional) | CI using `Jenkinsfile` |

Ensure MinGW is on your PATH (or use the paths below):

```text
C:\msys64\mingw64\bin
```

---

## Quick start (PowerShell)

From the repository root (`online-shopping-system`):

```powershell
$env:Path = "C:\msys64\mingw64\bin;" + $env:Path
.\scripts\build_and_test.ps1 -Coverage
```

This configures, builds tests, runs them, and produces:

- `build/coverage.xml` — for Jenkins / Cobertura
- `build/coverage/index.html` — optional HTML report (open in a browser)

---

## Step-by-step commands

### 1. Configure (first time or after clean)

```powershell
cd online-shopping-system
$env:Path = "C:\msys64\mingw64\bin;" + $env:Path

cmake -B build -G Ninja `
  -DCMAKE_CXX_COMPILER=C:/msys64/mingw64/bin/g++.exe `
  -DCMAKE_C_COMPILER=C:/msys64/mingw64/bin/gcc.exe `
  -DCMAKE_BUILD_TYPE=Debug `
  -DBUILD_TESTS=ON `
  -DBUILD_APPS=OFF `
  -DENABLE_COVERAGE=ON
```

> **Note:** If Google Test download fails (e.g. OneDrive lock), ensure `build/_deps/googletest-src` exists, or re-run configure after copying the extracted googletest folder there.

### 2. Build

```powershell
cmake --build build
```

### 3. Run tests

```powershell
ctest --test-dir build --output-on-failure
```

Or run executables directly:

```powershell
.\build\tests\testClient.exe
.\build\tests\testServer.exe
```

### 4. Generate `coverage.xml` (Cobertura, for Jenkins)

```powershell
pip install gcovr

gcovr -r . --object-directory build `
  --filter "Client/" --filter "Server/" `
  --xml build/coverage.xml
```

### 5. Optional: HTML coverage report

```powershell
New-Item -ItemType Directory -Force -Path build/coverage | Out-Null

gcovr -r . --object-directory build `
  --filter "Client/" --filter "Server/" `
  --html-details build/coverage/index.html
```

Open `build/coverage/index.html` in a browser.

### 6. Optional: text summary in the terminal

```powershell
gcovr -r . --object-directory build --filter "Client/" --filter "Server/"
```

---

## CMake coverage target

If the project was configured with `-DENABLE_COVERAGE=ON`:

```powershell
cmake --build build --target coverage
```

This creates `build/coverage.xml` and `build/coverage/index.html` (requires `gcovr` on PATH).

---

## Jenkins (unified Java + C++ pipeline)

For the multi-repo job (Java + this C++ project), use **`Jenkinsfile.unified`** in Jenkins. It keeps your existing Java/Sonar/reporting stages and replaces only the **C++** build with:

- CMake + Ninja + MinGW  
- `testClient` / `testServer` (Google Test)  
- `gcovr --sonarqube` → `Server/cpp-coverage.xml` (same path Sonar expects)

**Agent requirements:** Windows, `C:\msys64\mingw64\bin` on PATH, CMake, Ninja, Git, Python.

**Repo layout (SEPARATE_REPOS):** C++ checkout must contain `Server/serverProgram.cpp` and root `CMakeLists.txt` (e.g. under `cpp-app/`).

---

## Jenkins (C++ only)

Use a **Windows** agent with MSYS2 MinGW, CMake, and Python. The pipeline is defined in `Jenkinsfile`:

1. Configure with Ninja + coverage flags  
2. Build  
3. `ctest`  
4. `gcovr` → `build/coverage.xml`  
5. Archive `build/coverage.xml` for the Cobertura plugin  

Label in `Jenkinsfile`: `windows` — change if your agent uses a different label.

---

## Test layout

```text
tests/
├── testClient/          # Client-side unit tests
│   └── Headers/         # test_Admin.cpp, test_Person.cpp, ...
├── testServer/          # Server-side unit tests
│   └── Headers/
└── common/              # MockClient, MockThread, helpers
```

---

## Clean generated artifacts

Coverage and build outputs are gitignored. To remove them locally:

```powershell
Remove-Item -Recurse -Force build/coverage -ErrorAction SilentlyContinue
Remove-Item -Force build/coverage.xml -ErrorAction SilentlyContinue
Remove-Item -Recurse -Force build -ErrorAction SilentlyContinue
```

After a clean, run configure + build + test again before generating coverage.

---

## Troubleshooting

| Issue | What to do |
|-------|------------|
| `g++` not found | Add `C:\msys64\mingw64\bin` to PATH |
| `ctest` finds no tests | Run `cmake --build build` first |
| Empty or stale coverage | Run tests, then `gcovr` (`.gcda` files are created when tests run) |
| Jenkins on Linux only | This project needs **Windows** (Winsock / Win32 threads) |
| googletest extract error | Copy `googletest-1.14.0` to `build/_deps/googletest-src` and re-run `cmake` |

---

## Related files

| File | Description |
|------|-------------|
| `CMakeLists.txt` | Root build; apps + tests |
| `tests/CMakeLists.txt` | Test targets, GTest, coverage target |
| `Jenkinsfile` | CI pipeline |
| `scripts/build_and_test.ps1` | Local build + test + coverage script |
| `PROJECT_AND_TEST_PLAN.md` | Architecture and test plan |
