@echo off
set BRANCH_DIR=%~dp0\..
set BUILDS_DIR=%BRANCH_DIR%\Builds
set TOOLS_DIR=%BRANCH_DIR%\Tools

REM increment IML version for each new release of the plugin to Marketplace
REM (not for for re-submission cycle, only for feature changes and fixes)
REM Typically major version change is for bit features changes or breaking changes
REM Typically minor version change is for fixes and improvements and non-breaking changes

REM ---- EDIT PLUGIN VERSION HERE ----
set IML_VERSION=1.0
REM ----------------------------------

REM ---- 4.25 ----
set UE_VERSION=4.25
call %TOOLS_DIR%\package_plugin.cmd
if ERRORLEVEL 1 goto error

REM ---- 4.26 ----
set UE_VERSION=4.26
call %TOOLS_DIR%\package_plugin.cmd
if ERRORLEVEL 1 goto error

REM ---- 4.27 ----
set UE_VERSION=4.27
call %TOOLS_DIR%\package_plugin.cmd
if ERRORLEVEL 1 goto error

REM Built versions can be found in the Builds directory
explorer.exe %BUILDS_DIR%
goto end
:error
echo BUILD FAILED, REVIEW LOG ABOVE, THEN PRESS ANY KEY
pause>nul
:end