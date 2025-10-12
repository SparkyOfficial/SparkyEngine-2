=
Get-Content
CMakeLists.txt
=
0
while
True
 >> modify.ps1 && echo     if ($content[$index] -eq "    src/WeaponSystem.cpp") { >> modify.ps1 && echo         $content = $content[0..$index] + "    src/WeaponSystem.cpp" + "    src/Gun.cpp" + "    src/GunImpl.cpp" + $content[($index+1)..($content.Length-1)] >> modify.ps1 && echo         break >> modify.ps1 && echo     } >> modify.ps1 && echo     $index++ >> modify.ps1 && echo 
