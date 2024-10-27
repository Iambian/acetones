@echo off
if not exist "bin\" mkdir bin\
tooling\spasm -E dbgtest.z80 bin\dbgtest.8xp
