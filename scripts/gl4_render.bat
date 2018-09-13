@echo OFF

cd /d %~dp0

:loop
if not "%1" == "" (
	%1 -gl4
	shift
	goto loop
)
