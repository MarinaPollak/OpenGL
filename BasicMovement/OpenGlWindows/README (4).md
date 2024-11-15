
# OpenGL Shape Transformations

This project demonstrates various shape transformations and movements using OpenGL. It includes a rotating triangle, a transitioning shape that morphs between a triangle and a square, and a circle with customized vertex transformations. Additionally, there's a new small shape moving across predefined coordinates, demonstrating advanced shape manipulation and dynamic rendering techniques.

## Features

- **Rotating Triangle**: A triangle that rotates continuously.
- **Shape Transition**: Smoothly transitions between a triangle and a square.
- **Moving Circle**: Moves up and down along the y-axis.
- **Small Moving Shape**: Moves across specified coordinates in a loop.

## Setup and Installation

### Prerequisites

- **GLFW**: Required to create windows and handle input.
- **GLAD**: Used for loading OpenGL functions.

### Installation

1. **Clone the repository** (if applicable):
   ```bash
   git clone <repository_url>
   cd <repository_directory>
   ```

2. **Install GLFW and GLAD**:
   - GLFW can be installed via package managers or from the [GLFW website](https://www.glfw.org/).
   - GLAD loader files can be generated from the [GLAD website](https://glad.dav1d.de/) for OpenGL 3.3 (Core profile).

3. **Compile and Run**:
   - Use a C++ compiler with OpenGL support.
   - Example compilation command (replace paths with actual locations):
     ```bash
     g++ -o ShapeTransformations main.cpp -lglfw -lGL -ldl -lGLU
     ```
   - Run the compiled executable:
     ```bash
     ./ShapeTransformations
     ```

## Usage

This application demonstrates dynamic shape transformation and rendering with OpenGL. The window created by this code will display the following effects:

1. **Background Color**: Changes dynamically over time, based on `sin` and `cos` functions.
2. **Rotating Triangle**: Positioned at the center and rotates continuously.
3. **Morphing Shape**: Transitions smoothly from a triangle to a square and moves along the x-axis.
4. **Moving Circle**: Transforms based on a circular pattern and oscillates vertically.
5. **New Small Shape**: Moves in a predefined pattern from one corner to another in a loop.

### Controls

No additional controls are required; the program runs automatically upon execution.

## Code Structure

- **Vertex Generation**: Circle vertices are generated with `generateCircleVertices()` for smooth rendering.
- **Shader Programs**: Basic vertex and fragment shaders are defined to handle transformations and color output.
- **Matrix Functions**:
  - `createRotationMatrix()`: Generates a rotation matrix for rotating shapes.
  - `createTranslationMatrix()`: Generates a translation matrix for moving shapes.
  - `multiplyMatrices()`: Multiplies matrices for combining transformations.
- **Interpolation Functions**:
  - `lerp()`: Linearly interpolates between two values.
  - `interpolateVertices()`: Interpolates between two sets of vertices to smoothly transition between shapes.

## How It Works

- **Vertex Shader**: Applies transformations to the vertices based on the transformation matrix.
- **Fragment Shader**: Outputs the color based on a uniform value.
- **Shape Transformations**:
  - A rotating triangle is created using a rotation matrix.
  - A shape that transitions between a triangle and square uses interpolation.
  - A moving circle's y-position oscillates using `sin` values.
- **New Shape Movement**: Moves to coordinates `(0, 0)`, `(10, 10)`, `(10, -10)`, `(-10, 10)`, and `(-10, -10)` in a loop.

## Functions Explained

### `generateCircleVertices(float centerX, float centerY, float radius, int segments)`

Generates vertices for a circle outline with specified parameters. Returns a vector of vertices for the circle.

### `createRotationMatrix(float* matrix, float angle)`

Creates a 4x4 rotation matrix for rotating shapes based on the specified angle.

### `createTranslationMatrix(float* matrix, float x, float y, float z)`

Creates a 4x4 translation matrix for moving shapes to the specified x, y, z coordinates.

### `lerp(float start, float end, float t)`

Linear interpolation function to smoothly transition between two values based on a factor `t`.

### `interpolateVertices(const GLfloat* startVertices, const GLfloat* endVertices, GLfloat* result, float t, int vertexCount)`

Interpolates between two sets of vertices to facilitate smooth transformations between shapes.

## Refactoring Notes

For further refactoring, consider separating:
- **Shader Setup**: Move shader code into a `Shader` class for easier reusability.
- **Matrix Operations**: Create a `Matrix` class to handle rotation, translation, and other transformations.
- **Shape Logic**: Separate each shape (triangle, square, circle) into its own class with `draw()` methods for modularity.

## Future Improvements

- **User Input**: Allow user control to start or stop animations.
- **3D Transformations**: Extend transformations to include scaling and 3D rotation.
- **Complex Shapes**: Add more shapes like polygons or dynamically generated patterns.
