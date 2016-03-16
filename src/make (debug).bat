@echo off
gcc main.c callbacks.c io.c matrix.c shaders.c world.c bot.c buffers.c -g -gdwarf-2 -Wall -Wextra -lglfw3 -lglew32 -lgdi32 -lopengl32 -o "bin\main(debug).exe"
PAUSE