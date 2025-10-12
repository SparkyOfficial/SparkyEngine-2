off
powershell
-Command
(Get-Content CMakeLists.txt) -replace 'src/WeaponSystem.cpp', 'src/WeaponSystem.cpp^
    src/Gun.cpp^
    src/GunImpl.cpp'
^>
CMakeLists.txt.tmp
move
CMakeLists.txt.tmp
CMakeLists.txt
