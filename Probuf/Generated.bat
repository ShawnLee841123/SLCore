
set RootPath=%CD%
set SourcePath=src\cmake
set BuildPath=WinBuild

mkdir %BuildPath% && cd %BuildPath%

cmake -G "Visual Studio 17 2022" -A x64 %RootPath%/%SourcePath%

cd ..

pause

