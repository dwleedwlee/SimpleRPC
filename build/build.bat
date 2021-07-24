@echo off
if "%1"=="debug" (
	set BLDOPT=DEBUG
) else (
	set BLDOPT=RELEASE
)

cmake -G "MinGW Makefiles" 
make
