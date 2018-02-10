@echo OFF

set DATA_DIR=.\

setlocal disableDelayedExpansion
for /f "delims=" %%A in ('forfiles /s /p %DATA_DIR% /m *.exe /c "cmd /c echo @relpath"') do (
	set "file=%%~A"
	setlocal enableDelayedExpansion
	echo "Testing !file:~2!"
	%DATA_DIR%\!file:~2!
	%DATA_DIR%\!file:~2! -gl
	endlocal
)
