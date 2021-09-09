@echo off
REM 1 - source folder
REM 2 - destination zip
if exist %2 (
	del /Q %2
)

powershell.exe -nologo -noprofile -command "& { Add-Type -A 'System.IO.Compression.FileSystem'; [IO.Compression.ZipFile]::CreateFromDirectory('%1', '%2'); }"