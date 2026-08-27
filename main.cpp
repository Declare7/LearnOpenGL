#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "TriangleSection.h"
#include "RectangleSection.h"
#include "TransformSection.h"
#include "CoordinateSection.h"
#include "CameraSection.h"

void showMenu();
SectionBase* createSection(int choice);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void wheel_callback(GLFWwindow* window, double xOffset, double yOffset);
void processInput(GLFWwindow *window);
void render(SectionBase *section);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

using namespace std;

SectionBase *section = nullptr;

int main()
{
    int choice = 0;
    showMenu();
    cin>> choice;
    cin.ignore();

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if(choice == 12 || choice == 13 || choice == 14)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, wheel_callback);
    }

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    section = createSection(choice);
    if(section == nullptr)
    {
        cout<< "Invalid Input."<< endl;
        return -1;
    }

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render(section);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    delete section;
    return 0;
}


void showMenu()
{
    cout<< "----------------------------"<< endl;
    cout<< "1. Triangle"<< endl;
    cout<< "2. Reangle VBO"<< endl;
    cout<< "3. Reangle EBO"<< endl;
    cout<< "4. Triangle Shader"<< endl;
    cout<< "5. Triangle Uniform"<< endl;
    cout<< "6. Triangle Colors"<< endl;
    cout<< "7. Rectangle Texture"<< endl;
    cout<< "8. Rectangle Texture Unit"<< endl;
    cout<< "9. Rectangle Transform"<< endl;
    cout<< "10. Coordinate"<< endl;
    cout<< "11. Coordinate 3D"<< endl;
    cout<< "12. Camera"<< endl;
    cout<< "13. Camera Keyboard"<< endl;
    cout<< "14. Camera Mouse"<< endl;
    cout<< "15. Triangle Double Program"<< endl;

    cout<< "----------------------------"<< endl;
    cout<< "Select To Show:"<< endl;
}

SectionBase* createSection(int choice)
{
    SectionBase *section = nullptr;
    switch(choice)
    {
    case 1:
        section = new TriangleSection();
        break;
    case 2:
        section = new RectangleSection();
        break;
    case 3:
        section = new RectangleSection("EBO");
        break;
    case 4:
        section = new TriangleSection("shader");
        break;
    case 5:
        section = new TriangleSection("uniform");
        break;
    case 6:
        section = new TriangleSection("colors");
        break;
    case 7:
        section = new RectangleSection("texture");
        break;
    case 8:
        section = new RectangleSection("textureUnit");
        break;
    case 9:
        section = new TransformSection();
        break;
    case 10:
        section = new CoordinateSection();
        break;
    case 11:
        section = new CoordinateSection("3D");
        break;
    case 12:
        section = new CameraSection();
        break;
    case 13:
        section = new CameraSection("keyboard");
        break;
    case 14:
        section = new CameraSection("mouse");
        break;
    case 15:
        section = new TriangleSection("doubleProgram");
        break;

    default:

        break;
    }

    if(section)
    {
        section->prepare();
    }

    return section;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(section == nullptr)
    {
        return;
    }

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    section->processInput(window);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    if(section == nullptr)
    {
        return;
    }

    section->processMouse(xPos, yPos);
}

void wheel_callback(GLFWwindow* window, double xOffset, double yOffset)
{
    if(section == nullptr)
    {
        return;
    }

    section->processWheel(xOffset, yOffset);
}

void render(SectionBase *section)
{
    section->render();
}
