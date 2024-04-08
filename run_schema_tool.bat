@echo off

schema_tool.exe ./src/Packets/LoginUdp_11/LoginUdp_11.schm ./src/Packets/LoginUdp_11/LoginUdp_11.cpp

IF ERRORLEVEL 1 (
  echo Running schema_tool failed!
  exit /b 1
)

echo Schema tool is done parsing!