@echo off
if not exist "bin\" mkdir bin\
rem tooling\spasm -E dbgtest.z80 bin\dbgtest.8xp
tooling\spasm -E -T -L main.z80 bin\ACETONES.8xp
rem tooling\spasm -E core2.z80 F.8xp