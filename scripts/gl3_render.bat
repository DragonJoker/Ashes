@echo OFF

cd /d %~dp0

:loop
if not "%1" == "" (
	%1 -gl3 > %1_gl3.log
	shift
	goto loop
)
