#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<header files/shaderClass.h>
#include<header files/VAO.h>
#include<header files/VBO.h>
#include<header files/EBO.h>
#include<header files/Texture.h>

const unsigned int width = 800;
const unsigned int height = 800;

// Vertice coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
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
    GLFWwindow* window = glfwCreateWindow(width, height, "Window is Window", NULL, NULL);
    if(window == NULL)
    {
        std::cout << "Error in createing the GLFW Window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //Giving the background a color
    gladLoadGL();

    glViewport(0, 0, width, height);

    Shader shaderProgram("Shaders/default.vert.txt", "Shaders/default.frag.txt");


    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

    //Texture (Refer to Texture.cpp and Texture.h)
    Texture popCat("Textures/brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    popCat.texUnit(shaderProgram, "tex0", 0);

    float rotation = 0.0f;
    double prevTime = glfwGetTime();

    glEnable(GL_DEPTH_TEST);

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaderProgram.Activate();


        double currentTime = glfwGetTime();
        if (currentTime - prevTime >= 1 /60)
        {
            rotation += 0.5f;
            prevTime = currentTime;
        }

        //Defining Matrixes
        glm::mat4 modelMat = glm::mat4(1.0f);
        glm::mat4 viewMat = glm::mat4(1.0f);
        glm::mat4 projectMat = glm::mat4(1.0f);

        modelMat = glm::rotate(modelMat, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        viewMat = glm::translate(viewMat, glm::vec3(0.0f, -0.5f, -2.0f));
        projectMat = glm::perspective(glm::radians(45.0f), (float)(width / height), 0.1f, 100.0f);

        //Uniform for Model Matrix
        int modelLoc = glGetUniformLocation(shaderProgram.ID, "modelMat");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat));
        //Uniform for View Matrix
        int viewLoc = glGetUniformLocation(shaderProgram.ID, "viewMat");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMat));
        //Uniform for Project Matrix
        int projectLoc = glGetUniformLocation(shaderProgram.ID, "projectMat");
        glUniformMatrix4fv(projectLoc, 1, GL_FALSE, glm::value_ptr(projectMat));


        glUniform1f(uniID, 0.1f);
        popCat.Bind();
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
    //Destroying Everything
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    popCat.Delete();
    shaderProgram.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}