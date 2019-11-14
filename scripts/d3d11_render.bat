@echo OFF

cd /d %~dp0

:loop
if not "%1" == "" (
	%1 -d3d11 > %1_d3d11.log
	shift
	goto loop
)
