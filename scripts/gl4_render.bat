@echo OFF

cd /d %~dp0

:loop
if not "%1" == "" (
	%1 -gl4 > %1_gl4.log
	shift
	goto loop
)
