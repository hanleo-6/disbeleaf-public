**DISBELEAF.**



https://github.com/user-attachments/assets/0aa0cf62-2c52-44ce-841d-e7824233dd2a



A 3D minigame written in C++ and OpenGL. You play as a leaf sea slug
(*Elysia chlorotica*) swimming around, nomming as many algal balls as you
can before the time limit runs out! (kind of like 3D snake). 
Not really a competition, games are just a fun medium for creation... but my highscore is 24 :D.

> [!NOTE]
> **Archived:** This was my A-level Computer Science NEA (2024–25).
>               It was my first C++ and graphics programming project,
>               and also a mark of when I became fascinated by boids.
> (Oct 2026: updated the CMake build so it runs on Linux.)

## Tech stack
C++20, OpenGL (GLAD), GLFW, GLM, Assimp, stb_image, Dear ImGui, CMake

## What I built
  - A renderer with my own shader, mesh and model abstractions (models loaded
with Assimp)
  - A free-moving 3D camera and player controls
  - A skybox and basic lighting written in GLSL
  - Fish shoals following a boid flocking algorithm because why not
  - A resource manager for shaders and textures, plus a save file for high
scores (+10 NEA points for file I/O...)
  - An ImGui debug overlay

## Building
GLFW and Assimp are taken from system if installed, otherwise CMake
downloads and builds them.
glad, GLM, stb_image and Dear ImGui are vendored.

**Linux**
```bash
# optional, makes the build much faster: Arch / Ubuntu
sudo pacman -S cmake glfw assimp      # or: sudo apt install cmake libglfw3-dev libassimp-dev
cd Disbeleaf
cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j
cd bin && ./disbeleaf
```

**Windows** (Visual Studio 2022)
```powershell
cd Disbeleaf
cmake -B build
cmake --build build --config Release
cd bin; .\disbeleaf.exe
```

> [!NOTE]
> The 3D models, textures and font aren't included in this repo for copyright
> reasons (see [CREDITS.md](CREDITS.md))
> So the game builds but obviously no assets.
> See demo video for full game!

## Some reflections

I'm thankful for this project because of the learning process I went through.
This was a time when LLMs were still in infancy and I needed to read
decade-old guides on OpenGL, scour forum snippets to make my build & include
work, then spend hours debugging why my sprite looked mutilated.

Since then, I've also learnt much more, so there are plenty of things I'd
do differently e.g.
 - manage dependencies with CMake FetchContent instead of committing binaries
 - separate game logic from rendering using a proper API
 - write unit and integration tests as I'm developing the code
 - and more...
Of course, this same project (and probably better) could be built today in
a few hours with LLM-assistance, but I wouldn't have learnt nearly as much!

