
# 🐍 C++ Snake Game

A simple implementation of the classic Snake game in C++, using Object-Oriented Programming (OOP) principles. This version leverages **SFML** for graphics and keyboard input handling.

## 🚀 Features

- **OOP Design**: Clean, object-oriented structure.
- **Graphics**: Uses SFML for rendering and handling user input.
- **Responsive Input**: Real-time keyboard controls for smooth gameplay.

## 📦 Dependencies

Make sure SFML is installed on your system:

```bash
$ sudo apt-get install libsfml-dev
```

## 🛠️ Build Instructions

To compile the game, follow these steps:

1. Clone the repository or download the source files.
2. Open a terminal and navigate to the project directory.

```bash
$ cd /path/to/snake
```

3. Create a build directory and compile the project using CMake:

```bash
$ mkdir build && cd build
$ cmake .. && make -j12
```

The `-j12` option allows parallel compilation across 12 threads. Adjust this number based on your CPU.

---