# OpenGL Dynamic Shapes and Background

This project demonstrates rendering animated shapes (triangles and a circle outline) with a dynamically changing background color using OpenGL. It features animated transformations, shader management, and refactoring suggestions for organized code.

## Features

- **Dynamic Background Color**: Smooth color transitions over time based on trigonometric functions.
- **Animated Shapes**: Two rotating and color-changing triangles.
- **Circle Outline**: A hollow circle (outline) generated with vertices to simulate a smooth curve.
- **Modular Design**: Refactoring suggestions for shader, matrix, and shape management.

## Prerequisites

- **OpenGL**: Ensure OpenGL is installed.
- **GLFW**: For window and input management.
- **GLAD**: To load OpenGL function pointers.
- **C++ Compiler**: A modern C++ compiler (e.g., g++ or clang++) for compilation.

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/MarinaPollak/OpenGL/new/main/ShapeTransformations/OpenGlWindows
   cd ShapeTransformations-OpenGlWindows
   ```
# _Move OpenGlWindows to the repos on your computer_
## Functions and Methods
Shader Management: Vertex and fragment shaders are created and managed to apply transformations and color changes.
Matrix Transformations: Provides functions for rotation and translation matrices, allowing dynamic animations.

## Usage
After running the program, you should see:
<img width="397" alt="Screenshot 2024-10-31 at 7 26 04 PM" src="https://github.com/user-attachments/assets/481186d5-6d41-4aef-83a3-227b7a6d7993">

A window with a background color that changes smoothly over time.
Two rotating triangles that change color independently.
A static circle outline.
These elements demonstrate key OpenGL techniques like color transitions, shape transformations, and drawing outlines.

## License
This project is open-source and available under the MIT License.
