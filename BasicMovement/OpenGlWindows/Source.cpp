#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>

// Function to generate vertices for a circle outline
std::vector<GLfloat> generateCircleVertices(float centerX, float centerY, float radius, int segments) {
    std::vector<GLfloat> vertices;
    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * 3.14159f * i / segments;
        float x = centerX + radius * cos(angle);
        float y = centerY + radius * sin(angle);
		float z = cos(angle+x); // z-coordinate (0 for 2D shape)
		vertices.push_back(2.0f * y + x);// x-coordinate
		vertices.push_back(y); // y-coordinate
        vertices.push_back(z); // z-coordinate (0 for 2D shape)
    }
    return vertices;
}

const char* vertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"   gl_Position = transform * vec4(aPos, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 color;\n"
"void main()\n"
"{\n"
"   FragColor = color;\n"
"}\n\0";

// Function to create rotation matrix
void createRotationMatrix(float* matrix, float angle) {
    float rad = angle * 3.14159f / 180.0f;
    float cosA = cos(rad);
    float sinA = sin(rad);

    matrix[0] = cosA; matrix[1] = -sinA; matrix[2] = 0.0f; matrix[3] = 0.0f;
    matrix[4] = sinA; matrix[5] = cosA;  matrix[6] = 0.0f; matrix[7] = 0.0f;
    matrix[8] = 0.0f; matrix[9] = 0.0f;  matrix[10] = 1.0f; matrix[11] = 0.0f;
    matrix[12] = 0.0f; matrix[13] = 0.0f; matrix[14] = 0.0f; matrix[15] = 1.0f;
}

// Function to create translation matrix
void createTranslationMatrix(float* matrix, float x, float y, float z) {
    matrix[0] = 1.0f; matrix[1] = 0.0f; matrix[2] = 0.0f; matrix[3] = x;
    matrix[4] = 0.0f; matrix[5] = 1.0f; matrix[6] = 0.0f; matrix[7] = y;
    matrix[8] = 0.0f; matrix[9] = 0.0f; matrix[10] = 1.0f; matrix[11] = z;
    matrix[12] = 0.0f; matrix[13] = 0.0f; matrix[14] = 0.0f; matrix[15] = 1.0f;
}


void multiplyMatrices(const float* a, const float* b, float* result) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result[i * 4 + j] = a[i * 4 + 0] * b[0 * 4 + j] +
                a[i * 4 + 1] * b[1 * 4 + j] +
                a[i * 4 + 2] * b[2 * 4 + j] +
                a[i * 4 + 3] * b[3 * 4 + j];
        }
    }
}

// Interpolates between two values (lerp function)
float lerp(float start, float end, float t) {
    return start + t * (end - start);
}

// Interpolates between two sets of vertices
void interpolateVertices(const GLfloat* startVertices, const GLfloat* endVertices, GLfloat* result, float t, int vertexCount) {
    for (int i = 0; i < vertexCount * 3; ++i) {
        result[i] = lerp(startVertices[i], endVertices[i], t);
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL Shapes Transforming", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL();

    glViewport(0, 0, 800, 800);

    // Define vertices for the static rotating triangle and the target square
    GLfloat rotatingTriangleVertices[] = {
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    GLfloat triangleVertices[] = {
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    GLfloat squareVertices[] = {
        -0.3f, 0.3f, 0.0f,
        0.3f, 0.3f, 0.0f,
        0.3f, -0.3f, 0.0f,
        -0.3f, -0.3f, 0.0f
    };

    // Interpolated vertices array to hold transition values
    GLfloat interpolatedVertices[12]; // Enough space for up to 4 vertices (triangle and square)

    // Generate circle vertices (radius 0.3, 50 segments for smoothness)
    std::vector<GLfloat> circleVertices = generateCircleVertices(0.0f, 0.0f, 0.3f, 50);

    // Compile shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint transformLoc = glGetUniformLocation(shaderProgram, "transform");
    GLuint colorLoc = glGetUniformLocation(shaderProgram, "color");

    // Setup VAO/VBO for rotating triangle
    GLuint rotatingTriangleVAO, rotatingTriangleVBO;
    glGenVertexArrays(1, &rotatingTriangleVAO);
    glGenBuffers(1, &rotatingTriangleVBO);

    glBindVertexArray(rotatingTriangleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, rotatingTriangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rotatingTriangleVertices), rotatingTriangleVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Setup VAO/VBO for circle
    GLuint circleVAO, circleVBO;
    glGenVertexArrays(1, &circleVAO);
    glGenBuffers(1, &circleVBO);

    glBindVertexArray(circleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
    glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(GLfloat), circleVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Setup VAO/VBO for shape transitioning between triangle and square
    GLuint shapeVAO, shapeVBO;
    glGenVertexArrays(1, &shapeVAO);
    glGenBuffers(1, &shapeVBO);

    glBindVertexArray(shapeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, shapeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(interpolatedVertices), interpolatedVertices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    float transitionStartTime = 5.0f; // Start transition after 5 seconds
    float transitionDuration = 2.0f;  // Transition lasts 2 seconds

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        float time = (float)glfwGetTime();
        float bgRed = (sin(time * 0.5f) + 1.0f) / 2.0f;
        float bgGreen = (cos(time * 0.3f) + 1.0f) / 2.0f;
        float bgBlue = (sin(time * 0.7f) + 1.0f) / 2.0f;

        glClearColor(bgRed, bgGreen, bgBlue, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Render rotating triangle
        float rotationMatrix[16];
        createRotationMatrix(rotationMatrix, time * 50.0f);
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, rotationMatrix);
        glUniform4f(colorLoc, 1.0f, 0.3f, 0.5f, 1.0f);
        glBindVertexArray(rotatingTriangleVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Compute transition factor `t` for smooth shape transformation
        float t = (time - transitionStartTime) / transitionDuration;
        t = (t < 0.0f) ? 0.0f : (t > 1.0f ? 1.0f : t); // Clamp `t` between 0 and 1

        // Interpolate vertices between triangle and square
        interpolateVertices(triangleVertices, squareVertices, interpolatedVertices, t, 4);

        // Update VBO with interpolated vertices
        glBindBuffer(GL_ARRAY_BUFFER, shapeVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(interpolatedVertices), interpolatedVertices);

        // Move shape horizontally
        float xOffset = sin(time) * 2.3f;
        float translationMatrix[16];
        createTranslationMatrix(translationMatrix, xOffset, 0.5f, 0.0f);
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, translationMatrix);

        // Render transitioning shape with color
        glUniform4f(colorLoc, 0.5f, 0.7f, 1.0f, 1.0f);
        glBindVertexArray(shapeVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, (t < 1.0f) ? 3 : 4); // Draw triangle or square based on t

        // Render circle moving between (0, 10) and (0, -10)
        float circleYOffset = 2*sin(time) * 15.0f; // Oscillate between 10 and -10
        createTranslationMatrix(translationMatrix, 0.0f, circleYOffset, 0.0f);
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, translationMatrix);
        glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f); // White color for circle
        glBindVertexArray(circleVAO);
        glDrawArrays(GL_LINE_LOOP, 0, circleVertices.size() / 3);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &shapeVAO);
    glDeleteBuffers(1, &shapeVBO);
    glDeleteVertexArrays(1, &circleVAO);
    glDeleteBuffers(1, &circleVBO);
    glDeleteVertexArrays(1, &rotatingTriangleVAO);
    glDeleteBuffers(1, &rotatingTriangleVBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
