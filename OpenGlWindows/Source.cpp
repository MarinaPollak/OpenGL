#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//The first thing we need to do is write the vertex shader in the shader language GLSL(OpenGL
//	Shading Language) and then compile this shader so we can use it in our application.
//specify coordinate for our vertexes
//this character array will be used to set the position of the triangle
const char* vertexShaderSource = "#version 330 core\n" //specify the version of the shader
"layout(location = 0) in vec3 aPos;\n" //specify the input vertex attribute
"void main()\n" //main function for the vertex shader
"{\n" //start of the function
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"//set the position of the vertex,vec.x, vec.y, vec.z and vec.w, where vec is a vector,vec.w component is not used as a position
//in space(we’re dealing with 3D, not 4D) but is used for something called perspective
//division.
"}\0";//end of the function

//specify the color of the triangle, the color is an orange color
//this character array will be used to set the color of the triangle
const char* fragmentShaderSource = "#version 330 core\n" //specify the version of the shader
"out vec4 FragColor;\n"//specify the output of the fragment shader
"void main()\n"//main function for the fragment shader
"{\n"//start of the function
"	FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"//set the color of the triangle
"}\n\0";//end of the function



int main()
{
	// Initialize GLFW for window creationz
	glfwInit();
	// Tell GLFW that we want to use OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // setting major version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // setting minor version
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // setting profile
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
		GL_TRUE); // setting forward compatibility for Mac

	//specify the vertices of the triangle
	GLfloat vertices[] = {
		0.5f, 0.5f, 0.0f, // top right
		0.4f, -0.4f, 0.1f, // bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.4f, 0.4f, 0.0f // top left
	};

	
	
	unsigned int indices[] = { // note that we start from 0!
		0, 1, 3, // first triangle
		1, 2, 3 // second triangle
	};

	



	GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL", nullptr, nullptr); // Create a window
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}


	//tell it that we do like to use the window that we created
	glfwMakeContextCurrent(window); //Make the window's context current

	//start using colors by telling GLAD to load configuration for OpenGL
	gladLoadGL(); //Load GLAD so it configures OpenGL


	glViewport(0, 0, 800, 800); //Create a viewport of the size of the window

	


	///Create a vertex buffer object (VBO) to store the vertices of the triangle
	//GLuint is used to declare the variable vertexShader,
	//which represents the vertex shader object.
	//The vertex shader object is responsible for processing vertex data in the OpenGL pipeline.

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); //Create a vertex shader

	//Next we attach the shader source code to the shader object and compile the shader:
	//The glShaderSource function is used to attach the shader source code to the shader object.
	//The glShaderSource function takes the shader object to compile to as its first argument.
	/*The second argument specifies how many strings we’re passing as source code, which is only one.*/
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr); //Attach the vertex shader to the source code
	glCompileShader(vertexShader); //Compile the vertex shader


	/*check if compilation was successful after the call to
		glCompileShader*/
	int success;

	/*check if compilation was successful with glGetShaderiv.*/
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{    
		/*compilation failed, we should retrieve the error message with glGetShaderInfoLog*/
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); //Create a fragment shader
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr); //Attach the fragment shader to the source code
	glCompileShader(fragmentShader); //Compile the fragment shader


	/*Both the shaders are now compiled and the only thing left to do is link both shader objects into a
		shader program that we can use for rendering.*/
	//Create a shader program to link the vertex and fragment shader together
	GLuint shaderProgram = glCreateProgram(); //Create a shader program


	/*The glCreateProgram function creates a program and returns the ID reference to the newly
		created program object.Now we need to attach the previously compiled shaders to the program
		object and then link them with glLinkProgram*/
	//attach the vertex and fragment shader to the shader program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	//wrap up the shader program
	glLinkProgram(shaderProgram);

	

	/*check if compilation was successful with glGetShaderiv.*/
	glGetProgramiv(vertexShader, GL_LINK_STATUS, &success);
	if (!success)
	{
		/*compilation failed, we should retrieve the error message with glGetShaderInfoLog*/
		char info_log[512];
		glGetProgramInfoLog(vertexShader, 512, nullptr, info_log);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
	}


	///delete shaders once we are done with them
	glDeleteShader(vertexShader); //Delete the vertex shader
	glDeleteShader(fragmentShader); //Delete the fragment shader


	//create a vertex buffer object to store the vertices of the triangle
	GLuint VAO, VBO; //Create a vertex buffer object and vertex array object, VBO and VAO
	glGenVertexArrays(1, &VAO); //Generate a vertex array object
	glGenBuffers(1, &VBO); //Generate a buffer
	glBindVertexArray(VAO); //Bind the vertex array object


	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Bind the buffer

	//copy the vertices to the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//copy the vertices to the buffer
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);


	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices, GL_STATIC_DRAW);


	/*When using glDrawElements we’re going to draw
		using indices provided in the element buffer object currently bound :*/

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr)); //Specify the vertex attributes
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbind the buffer
	glBindVertexArray(0); //Unbind the vertex array object
	//tell openGl how to interpret the vertex data


	

	//tell OpenGL to clear the color buffer  and set the color to red
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f); //Set the color of the window to red
	glClear(GL_COLOR_BUFFER_BIT); //Clear the color buffer

	glfwSwapBuffers(window); //Swap the buffers to fromt buffer since it where are colors are displayed

	//to see out color


	
	//we have to tell to openGl to keep the window open
	/*The glfwWindowShouldClose function checks at the start of each loop iteration if GLFW
		has been instructed to close.If so, the function returns true and the render loop stops running,
		after which we can close the application.*/
	while (!glfwWindowShouldClose(window)) //While the window should not close
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true); //Close the window if the escape key is pressed
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f); //GlClearColor is a state setting function, setting color
		glClear(GL_COLOR_BUFFER_BIT); //glClear is a state using function, using the color set by glClearColor

		//activate the shader programm
		/*The processing cores run small programs on the GPU for each step of the
			pipeline.These small programs are called shaders.*/
		glUseProgram(shaderProgram); //Use the shader program
		glBindVertexArray(VAO); //Bind the vertex array object
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//glDrawArrays(GL_TRIANGLES, 0, 3); //Draw the triangle with 0 offset and 3 vertices
		glfwSwapBuffers(window); /*The glfwSwapBuffers
            will swap the color buffer(a large 2D buffer that contains color values for each pixel in GLFW’s
			window) that is used to render to during this render iteration and show it as output to the screen*/

		glfwPollEvents(); //The glfwPollEvents function checks if any events are
		//triggered(like keyboard input or mouse movement events), updates the window state, and calls the
		//	corresponding functions(which we can register via callback methods).
	}

	//1 is passed to the function to specify the number of vertex array objects to delete
	glDeleteVertexArrays(1, &VAO); //Delete the vertex array object
	glDeleteBuffers(1, &VBO); //Delete the buffer
	glDeleteProgram(shaderProgram); //Delete the shader program


	//Once we are done with a window itself we want to delete it
	glfwDestroyWindow(window); // Destroy the window

	glfwTerminate(); /*As soon as we exit the render loop we would like to properly clean / delete all of GLFW’s resources
		that were allocated.We can do this via the glfwTerminate function that we call at the end of
		the main function.*/
}

