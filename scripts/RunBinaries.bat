@echo OFF

cd /d %~dp0

:loop
if not "%1" == "" (
	set FILE=%1
	echo "Testing %1"
	%1
	%1 -gl4
	%1 -gl3
	%1 -d3d11
	shift
	goto loop
)
