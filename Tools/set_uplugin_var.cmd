@echo off
REM 1 = .uplugin file path
REM 2 = parameter name
REM 3 = new parameter value
powershell.exe -nologo -noprofile -executionpolicy bypass %~dp0set_uplugin_var.ps1 %1 %2 %3