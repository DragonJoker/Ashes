@echo OFF

cd /d %~dp0

:loop
if not "%1" == "" (
	%1 -d3d11
	shift
	goto loop
)
