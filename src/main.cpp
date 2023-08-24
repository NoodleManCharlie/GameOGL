#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include<header files/shaderClass.h>
#include<header files/VAO.h>
#include<header files/VBO.h>
#include<header files/EBO.h>

// Vertice coordinates
GLfloat vertices[] =
{
    //Normal Triangle Vertices
	-0.5f, -0.5f  * float(sqrt(3)) / 3, 0.0f, // Lower left corner
	0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
	0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner

    //Extra Triforce vertices
    -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, //Inner Left
    0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner Right
    0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Inner Down
};

GLuint indices[] =
{
    0, 3, 5, //Lower left triangle
    3, 2, 4, //Lower right triangle
    5, 4, 1 //Upper triangle
};

int main()
{
    //Initialize GLFW
    glfwInit();

    //Hinting GLFW all of our settings so it knows what its working with
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3.3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3.3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Create a window and check if the process failed.
    GLFWwindow* window = glfwCreateWindow(800, 800, "Window is Window", NULL, NULL);
    if(window == NULL)
    {
        std::cout << "Error in createing the GLFW Window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //Giving the background a color
    gladLoadGL();

    glViewport(0, 0, 800, 800);

    Shader shaderProgram("default.vert.txt", "default.frag.txt");


    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkVBO(VBO1, 0);
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
        shaderProgram.Activate();
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
    //Destroying Everything
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}