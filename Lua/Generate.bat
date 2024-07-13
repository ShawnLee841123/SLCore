@set SourceDir=src
@set VSPath=Build
@set CurPath=%CD%

mkdir %VSPath%
cd %VSPath%

cmake -G"Visual Studio 17 2022" -A x64 %CurPath%/%SourceDir%

cd ..
