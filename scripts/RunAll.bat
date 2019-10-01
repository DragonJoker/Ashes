@echo OFF

set DATA_DIR=.\

setlocal disableDelayedExpansion
for /f "delims=" %%A in ('forfiles /s /p %DATA_DIR% /m Test-*.exe /c "cmd /c echo @relpath"') do (
	set "file=%%~A"
	setlocal enableDelayedExpansion
	echo "Testing !file:~2!"
	%DATA_DIR%\!file:~2! -vk > !file:~2!_vk.log
	%DATA_DIR%\!file:~2! -gl4 > !file:~2!_gl4.log
	%DATA_DIR%\!file:~2! -gl3 > !file:~2!_gl3.log
	%DATA_DIR%\!file:~2! -d3d11 > !file:~2!_d3d11.log
	endlocal
)

setlocal disableDelayedExpansion
for /f "delims=" %%A in ('forfiles /s /p %DATA_DIR% /m Sample-*.exe /c "cmd /c echo @relpath"') do (
	set "file=%%~A"
	setlocal enableDelayedExpansion
	echo "Testing !file:~2!"
	%DATA_DIR%\!file:~2! -vk > !file:~2!_vk.log
	%DATA_DIR%\!file:~2! -gl4 > !file:~2!_gl4.log
	%DATA_DIR%\!file:~2! -gl3 > !file:~2!_gl3.log
	%DATA_DIR%\!file:~2! -d3d11 > !file:~2!_d3d11.log
	endlocal
)
