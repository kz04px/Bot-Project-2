@echo off
gcc main.c callbacks.c io.c matrix.c shaders.c world.c bot.c buffers.c fnn.c simulation.c -DNDEBUG -O3 -s -lglfw3 -lglew32 -lgdi32 -lopengl32 -pthread -o "bin\main(release).exe"
PAUSE