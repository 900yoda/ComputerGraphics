#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;

int main() {
    //Initialize GLFW
    if(!glfwInit()) {
        std::cout << "Initialization Failed" << std::endl;
        glfwTerminate();
        return 1;
    }

    //setup GLFW window properties
    // OpenGL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //means don't use deprecated code, no backwards compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
    if(!mainWindow) {
        std::cout << "GLFW window creation failed" << std::endl;
        glfwTerminate();
        return 1;
    }

    //get buffer size information
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // set the context for glew to use
    glfwMakeContextCurrent(mainWindow);

    // allow modern extention features
    glewExperimental = GL_TRUE;

    if(glewInit() != GLEW_OK) {
        std::cout << "GLEW Initialization Failed" << std::endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    //setup the viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    // loop until the window closes
    while(!glfwWindowShouldClose(mainWindow)) {
        // get and handle user input events
        glfwPollEvents();

        // clear window
        glClearColor(1.0f,0,0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(mainWindow);
    }
    return 0;
}