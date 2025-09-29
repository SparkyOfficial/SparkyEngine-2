# Sparky Engine Game Runner
# This script builds and runs the game

Write-Host "Building Sparky Engine..." -ForegroundColor Green

# Create build directory if it doesn't exist
if (!(Test-Path "build")) {
    New-Item -ItemType Directory -Name "build" | Out-Null
}

# Change to build directory
Set-Location build

# Generate build files with CMake
cmake .. -G "Visual Studio 17 2022" -A x64

# Build the project
cmake --build . --config Release

# Check if build was successful
if ($LASTEXITCODE -eq 0) {
    Write-Host "Build completed successfully!" -ForegroundColor Green
    Write-Host "Running the game..." -ForegroundColor Green
    
    # Run the game
    if (Test-Path "bin\Release\SparkyGame.exe") {
        Start-Process -FilePath "bin\Release\SparkyGame.exe" -Wait
    } else {
        Write-Host "Game executable not found!" -ForegroundColor Red
        Write-Host "Please check the build output for errors." -ForegroundColor Red
    }
} else {
    Write-Host "Build failed with exit code $LASTEXITCODE" -ForegroundColor Red
    Write-Host "Please check the error messages above." -ForegroundColor Red
}

# Return to original directory
Set-Location ..