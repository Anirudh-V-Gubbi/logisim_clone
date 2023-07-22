<div align="center">
  <h1 align="center">Logisim-Clone</h1>
</div>

## Introduction
This is an attempt to recreate the famous Logisim software used to design and simulate digital circuits. The project will utilise modern C++ and modern OpenGL(3.3) along with GLFW to handle graphic related load. The main goal of the project is to realise the applications of Object-Oriented Programming with memory-safe constructs in C++, while creating a solid(flexible upto some extent) rendering framework in OpenGL to apply various graphic concepts(such as object picking).

## Progress
- [x] Create a basic framework, involving logger, shaders, textures, renderers, entities and viewport windows(framebuffers).
- [x] Have a grid system(coordinate system) and an input handler in place.
- [x] Create gate, wire and switch entities as well as a dedicated renderer.
- [ ] Establish Socket-Socket communication. (work in progress)
- [ ] Introduce an object-picking system for user to interact with the circuits.
- [ ] Create a toolbar interface with icons and texts.
- [ ] Establish different modes / states for the application.
- [ ] Create a file system to save progress.
- [ ] Create a project space for user to browse through project directories.

## Installation Guide

**Note**: The project needs glew and glfw3 libraries to be installed. On MacOS, you install these easily through Homebrew:
  ```
  brew install <package-name>
  ```
  
1. Clone the master branch of the repository.
  ```
  git clone https://github.com/Anirudh-V-Gubbi/logisim_clone.git
  ```
2. Change directory into the Logisim(clone) folder
  ```
  cd logisim_clone
  ```
3. Create a build folder and change directory.
  ```
  mkdir build
  ```
  ```
  cd build
  ```
4. Run the cmake and make commands.
  ```
  cmake ..
  ```
  ```
  make
  ```
5. Run the EXE file generated from the terminal.
  ```
  ./logisim_clone
  ```
  
