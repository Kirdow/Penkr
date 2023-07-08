@echo off
pushd %~dp0\..\
echo ^|------------------------------^|
echo ^|- GENERATING VS2022 SOLUTION -^|
echo ^|------------------------------^|
call vendor\premake\bin\premake5.exe vs2022
echo ^|------------------------------^|
echo ^|------------ DONE ------------^|
echo ^|------------------------------^|
popd
PAUSE
