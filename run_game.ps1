# PowerShell script to run the SparkyGame and capture output
Set-Location "c:\Users\Богдан\Desktop\SparkyEngine2\build\Game\Release"
$process = Start-Process -FilePath ".\SparkyGame.exe" -PassThru -Wait
Write-Host "Process Exit Code: $($process.ExitCode)"