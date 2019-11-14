@echo OFF

cd /d %~dp0

:loop
if not "%1" == "" (
	set FILE=%1
	echo "Testing %1"
	%1 -vk > %1_vk.log
	%1 -gl4 > %1_gl4.log
	%1 -d3d11 > %1_d3d11.log
	shift
	goto loop
)
