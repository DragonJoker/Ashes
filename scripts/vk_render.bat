@echo OFF

cd /d %~dp0

:loop
if not "%1" == "" (
	%1 -vk
	shift
	goto loop
)
