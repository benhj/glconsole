// Hacky test

#include "glconsole/GLConsole.hpp"
#include <GLFW/glfw3.h>
#include <memory>

std::shared_ptr<glconsole::GLConsole> testConsolePtr;

void keyboardHandler(GLFWwindow *, int key, int scancode, int action, int mods) 
{ 
    if(testConsolePtr) {
        testConsolePtr->keyHandler(key, scancode, action, mods);
    }
}

int main(int argc, char **argv)
{

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    int windowWidth = 300;
    int windowHeight = 300;
    window = glfwCreateWindow(windowWidth, windowHeight, "GLConsole test", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Path specific to my mac. Will be different on linux.
    std::string fontPath("/Library/Fonts/Courier New.ttf");
    testConsolePtr = 
    std::make_shared<glconsole::GLConsole>(0, 0 , 20, 250, 15,
                                           fontPath);

    glfwSetKeyCallback(window, keyboardHandler);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glClearColor(0, 0, 0, 0.0);

        // Blue text
        glColor3ub(0,0xff,0);
        testConsolePtr->display();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
