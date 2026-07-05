# Build and run LRU Cache project (Windows PowerShell)
$ErrorActionPreference = "Stop"
$Cxx = "g++"
$Flags = "-std=c++17 -Wall -I."

Write-Host "Building demo (lru_cache)..." -ForegroundColor Cyan
& $Cxx $Flags.Split() -o lru_cache.exe src/main.cpp
if ($LASTEXITCODE -ne 0) { exit 1 }

Write-Host "Building tests (test_lru_cache)..." -ForegroundColor Cyan
& $Cxx $Flags.Split() -o test_lru_cache.exe tests/test_lru_cache.cpp
if ($LASTEXITCODE -ne 0) { exit 1 }

Write-Host "Running tests..." -ForegroundColor Green
& ./test_lru_cache.exe
$testExit = $LASTEXITCODE

Write-Host ""
Write-Host "Running demo..." -ForegroundColor Green
& ./lru_cache.exe

exit $testExit
