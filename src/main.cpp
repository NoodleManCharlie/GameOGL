#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include<header files/shaderClass.h>
#include<header files/VAO.h>
#include<header files/VBO.h>
#include<header files/EBO.h>

// Vertice coordinates
GLfloat vertices[] =
{//      Coordinates                Colors           Texture Vertices
	-0.5f,  -0.5f, 0.0f,      1.0f, 0.0f,  0.0f,        0.0f, 0.0f,     // Lower left corner
	-0.5f,   0.5f, 0.0f,      0.8f, 1.0f,  0.0f,        0.0f, 1.0f,     // Upper left corner
	 0.5f,   0.5f, 0.0f,      0.0f, 0.0f,  1.0f,        1.0f, 1.0f,     // Upper right corner
     0.5f,  -0.5f, 0.0f,      1.0f, 1.0f,  1.0f,        1.0f, 0.0f,     // Lower right corner

};

GLuint indices[] =
{
    0, 2, 1, //Upper Square
    0, 3, 2  //Lower Square
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


    // Texture
    int widthImg, heightImg, numColCh;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load("Textures/pop_cat.png", &widthImg, &heightImg, &numColCh, 0);

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);


    //Telling Open GL how to process when scaling image
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //Telling Open GL how to process when repeating the image
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0");
    shaderProgram.Activate();
    glUniform1i(tex0Uni, 0);

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
        shaderProgram.Activate();
        glUniform1f(uniID, 0.25f);
        glBindTexture(GL_TEXTURE_2D, texture);
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
    //Destroying Everything
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    glDeleteTextures(1, &texture);
    shaderProgram.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}