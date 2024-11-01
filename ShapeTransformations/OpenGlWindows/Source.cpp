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
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f); // z-coordinate (0 for 2D shape)
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

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL Shapes Tranforming", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL();

    glViewport(0, 0, 800, 800);

    // Define vertices for two triangles
    GLfloat vertices[] = {
        // First triangle
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,

        // Second triangle
        0.0f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    // Generate circle vertices (e.g., radius 0.3, 50 segments for smoothness)
    std::vector<GLfloat> circleVertices = generateCircleVertices(0.0f, 0.0f, 0.3f, 50);

    GLuint circleVAO, circleVBO;
    glGenVertexArrays(1, &circleVAO);
    glGenBuffers(1, &circleVBO);

    glBindVertexArray(circleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
    glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(GLfloat), circleVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


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

    // Setup buffers
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    GLuint transformLoc = glGetUniformLocation(shaderProgram, "transform");
    GLuint colorLoc = glGetUniformLocation(shaderProgram, "color");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Calculate background color based on time
        float time = (float)glfwGetTime();
        float bgRed = (sin(time * 0.5f) + 1.0f) / 2.0f;
        float bgGreen = (cos(time * 0.3f) + 1.0f) / 2.0f;
        float bgBlue = (sin(time * 0.7f) + 1.0f) / 2.0f;

        // Set dynamic background color
        glClearColor(bgRed, bgGreen, bgBlue, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use shader program for shapes
        glUseProgram(shaderProgram);

        // Render first triangle
        float rotationMatrix[16];
        float translationMatrix[16];
        float finalMatrix[16];

        createRotationMatrix(rotationMatrix, time * 50.0f);
        createTranslationMatrix(translationMatrix, -0.5f, 0.0f, 0.0f);
        multiplyMatrices(translationMatrix, rotationMatrix, finalMatrix);

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, finalMatrix);
        glUniform4f(colorLoc, (sin(time * 2.0f) + 1.0f) / 2.0f, 0.3f, 0.5f, 1.0f);
        glBindVertexArray(VAO);  // Triangle VAO
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Render second triangle with a different transformation
        createRotationMatrix(rotationMatrix, -time * 50.0f);
        createTranslationMatrix(translationMatrix, 0.5f, 0.0f, 0.0f);
        multiplyMatrices(translationMatrix, rotationMatrix, finalMatrix);

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, finalMatrix);
        glUniform4f(colorLoc, 0.3f, (sin(time * 2.0f) + 1.0f) / 2.0f, 0.8f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 3, 3);

        // Render circle outline
        glBindVertexArray(circleVAO);  // Circle VAO
        glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);  // Circle color (e.g., white)
        glDrawArrays(GL_LINE_LOOP, 0, circleVertices.size() / 3);

        // Unbind VAO
        glBindVertexArray(0);

        // Swap buffers to display the rendered frame
        glfwSwapBuffers(window);
    }



    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
