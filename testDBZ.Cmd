@echo off
rem testdbz.cmd - test errorlevel returns from dbz
rem by John Carmack, MarchHare@momeraths.org
rem
rem Public domain software.
rem

@echo on
dbz %1 %2 %3 %4 %5 %6 %7 %8
@echo off
if errorlevel 98 goto configErr
if errorlevel 49 goto userErr
if errorlevel 1 goto dbzErr

@echo no error from program
goto End

:configErr
@echo There is a problem with your configuration.
goto End

:userErr
@echo There is a porblem understanding your input.
goto End

:dbzErr
@echo dbz could not find/add the key you input.
goto End

:End

