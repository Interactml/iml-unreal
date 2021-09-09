@echo off
REM Handle packaging of a specific version of the InteractML plugin ready for upload to Marketplace
echo ======== Plugin Packager ========

REM ---- MAIN PATHS ----
set BRANCH_DIR=%~dp0..
set BUILDS_DIR=%BRANCH_DIR%\Builds
set STAGING_DIR=%BUILDS_DIR%\Staging

REM ---- ENV CHECKS ----
if '%UE_VERSION%'=='' (
	echo ERROR: UE_VERSION expected in form MAJOR.MINOR, e.g. 4.27
	exit /b 1
)
if '%IML_VERSION%'=='' (
	echo ERROR: IML_VERSION expected in form MAJOR.MINOR, e.g. 1.0
	exit /b 1
)
if '%IML_DATE%'=='' (
	REM generate datestamp in the form YYMMDD
	powershell.exe -nologo -noprofile -command "Get-Date -Format '\"SET IML_DATE=\"yyMMdd'">%BUILDS_DIR%\set_date.cmd
	call %BUILDS_DIR%\set_date.cmd
	del /Q %BUILDS_DIR%\set_date.cmd
)

REM ---- DEPENDENT PATHS ----
REM --package--
set ZIP_NAME=InteractML_%IML_VERSION%_%IML_DATE%_UE%UE_VERSION%.zip
set ZIP_FILE=%BUILDS_DIR%\%ZIP_NAME%
REM --plugin--
set PLUGIN_DIR=%BRANCH_DIR%\Plugins\InteractML
set PLUGIN_FILE=%PLUGIN_DIR%\InteractML.uplugin
REM --ue tooling--
set UE_DIR=c:\Program Files\Epic Games\UE_%UE_VERSION%
set UAT="%UE_DIR%\Engine\Binaries\DotNET\AutomationTool.exe"

REM ---- SOURCE CHECKS ----
if not exist %UAT% (
	echo ERROR: Unable to locate Unreal Automation Tool, is UE %UE_VERSION% installed?
	echo PATH: %UAT%
	call :cleanup
	exit /b 3
)
if not exist %PLUGIN_FILE% (
	echo ERROR: Unable to locate plugin file, is branch up to date?
	echo PATH: %PLUGIN_FILE%
	call :cleanup
	exit /b 4
)

REM ---- CREATE DIRS ----
if not exist %BUILDS_DIR% (
	echo Making Builds directory...
	md %BUILDS_DIR%
)
if not exist %STAGING_DIR% (
	echo Making Staging directory...
	md %STAGING_DIR%
)

REM ---- STATUS ----
echo Plugin:  InteractML
echo Version: %IML_VERSION%
echo Unreal:  %UE_VERSION%
echo Date:    %IML_DATE%
goto no_status
echo UAT = %UAT%
echo PLUGIN = %PLUGIN_FILE%
echo STAGING = %STAGING_DIR%
echo ZIP = %ZIP_FILE%
:no_status


echo -------- Modifying build for UE %UE_VERSION% --------
set SAVED_PLUGIN_FILE=%BUILDS_DIR%\original_uplugin_file.txt
copy /Y %PLUGIN_FILE% %SAVED_PLUGIN_FILE%
call %TOOLS_DIR%\set_engineversion.cmd %PLUGIN_FILE% %UE_VERSION%


echo -------- Packaging Plugin --------
echo CMDLINE: %UAT% BuildPlugin -Plugin=%PLUGIN_FILE% -Package=%STAGING_DIR% -CreateSubFolder
%UAT% BuildPlugin -Plugin=%PLUGIN_FILE% -Package=%STAGING_DIR% -CreateSubFolder
if ERRORLEVEL 1 (
	echo ERROR: Package failed, check output above for errors
	call :cleanup
	exit /b 2
)

echo -------- Tidying Build --------
if exist %STAGING_DIR%\Binaries (
	RMDIR /S /Q %STAGING_DIR%\Binaries
)
if exist %STAGING_DIR%\Intermediate (
	RMDIR /S /Q %STAGING_DIR%\Intermediate
)


echo -------- Zipping --------
call %TOOLS_DIR%\zip_folder.cmd %STAGING_DIR% %ZIP_FILE%
echo Built output: %ZIP_FILE%

echo -------- Cleaning up --------
call :cleanup
goto done

echo -------- FUNCTIONS --------
:cleanup
set IML_DATE=
if exist %SAVED_PLUGIN_FILE% (
	copy /Y %SAVED_PLUGIN_FILE% %PLUGIN_FILE%
	del /Q %SAVED_PLUGIN_FILE%
)
if exist %STAGING_DIR% (
	rmdir /S /Q %STAGING_DIR%
)
exit /b


:done