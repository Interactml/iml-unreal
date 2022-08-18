@echo off
set BRANCH_DIR=%~dp0\..
set BUILDS_DIR=%BRANCH_DIR%\Builds
set TOOLS_DIR=%BRANCH_DIR%\Tools

REM ---- EDIT PLUGIN VERSION HERE ----
REM increment IML version for each new release of the plugin to Marketplace
REM (not for for re-submission cycle, only for feature changes and fixes)
set IML_VERSION=3
REM ----------------------------------

GOTO skip
REM ---- 4.25 ----
set UE_VERSION=4.25
call %TOOLS_DIR%\package_plugin.cmd
if ERRORLEVEL 1 goto error
:skip

REM ---- 4.26 ----
set UE_VERSION=4.26
call %TOOLS_DIR%\package_plugin.cmd
if ERRORLEVEL 1 goto error

REM ---- 4.27 ----
set UE_VERSION=4.27
call %TOOLS_DIR%\package_plugin.cmd
if ERRORLEVEL 1 goto error

REM ---- 5.0 ----
set UE_VERSION=5.0
set UE_PATH_UE=5.0
set UE_PATH_UAT=Engine\Binaries\DotNET\AutomationTool\AutomationTool.exe
call %TOOLS_DIR%\package_plugin.cmd
if ERRORLEVEL 1 goto error

REM ----------------------------------
REM Built versions can be found in the Builds directory
explorer.exe %BUILDS_DIR%
goto end
:error
echo BUILD FAILED, REVIEW LOG ABOVE, THEN PRESS ANY KEY
pause>nul
:end

set UE_VERSION=
set UE_PATH_UE=
set UE_PATH_UAT=