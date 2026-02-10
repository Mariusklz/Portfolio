:: info.bat

@echo off
start /min powershell -ExecutionPolicy Bypass -File "%~dp0info.ps1"
exit