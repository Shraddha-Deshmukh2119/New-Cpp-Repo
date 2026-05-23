# Build and run tests locally (Windows + MSYS2 MinGW)
param(
    [switch]$Coverage,
    [switch]$Apps
)

$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent $PSScriptRoot
$Mingw = "C:\msys64\mingw64\bin"

if (-not (Test-Path "$Mingw\g++.exe")) {
    Write-Error "MinGW g++ not found at $Mingw. Install MSYS2 mingw-w64 toolchain."
}

$env:Path = "$Mingw;" + $env:Path
Set-Location $Root

$cmakeArgs = @(
    "-B", "build",
    "-G", "Ninja",
    "-DCMAKE_CXX_COMPILER=$Mingw/g++.exe",
    "-DCMAKE_C_COMPILER=$Mingw/gcc.exe",
    "-DCMAKE_BUILD_TYPE=Debug",
    "-DBUILD_TESTS=ON",
    "-DBUILD_APPS=$($Apps.IsPresent)"
)

if ($Coverage) {
    $cmakeArgs += "-DENABLE_COVERAGE=ON"
}

cmake @cmakeArgs
cmake --build build
ctest --test-dir build --output-on-failure

if ($Coverage) {
    pip install gcovr -q
    gcovr -r . --object-directory build --filter "Client/" --filter "Server/" `
        --xml build/coverage.xml --html-details build/coverage/index.html
    gcovr -r . --object-directory build --filter "Client/" --filter "Server/"
    Write-Host "`nCoverage XML: build/coverage.xml"
}
