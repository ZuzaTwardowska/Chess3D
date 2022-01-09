#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "camera.h"
#include "Model.h"
#include "Board.h"

#include <iostream>
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1500;
const unsigned int SCR_HEIGHT = 1000;

// camera
Camera camera(glm::vec3(0.0f, 20.0f, 25.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Chess", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    //stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);

    Board board;
    float left = -0.7f;
    float xStep = 2.55f;
    Piece* whitePieces[] = {
        new Pawn(glm::vec3(left, 0.0f, 0.6f),"white"),
        new Pawn(glm::vec3(left+xStep, 0.0f, 0.6f), "white"),
        new Pawn(glm::vec3(left+2*xStep, 0.0f, 0.6f), "white"),
        new Pawn(glm::vec3(left+3*xStep, 0.0f, 0.6f), "white"),
        new Pawn(glm::vec3(left+4*xStep, 0.0f, 0.6f), "white"),
        new Pawn(glm::vec3(left+5*xStep, 0.0f, 0.6f), "white"),
        new Pawn(glm::vec3(left+6*xStep, 0.0f, 0.6f), "white"),
        new Pawn(glm::vec3(left+7*xStep, 0.0f, 0.6f), "white"),
        new Rook(glm::vec3(-17.2f, 0.0f, 17.2f), "white"),
        new Knight(glm::vec3(-0.5f, 0.0f, 0.8f), "white"),
        new Bishop(glm::vec3(-0.15f, 0.0f, 0.8f), "white"),
        new Queen(glm::vec3(0.0f, 0.0f, 0.8f), "white"),
        new King(glm::vec3(0.05f, 0.0f, 0.8f), "white"),
        new Bishop(glm::vec3(7.35f, 0.0f, 0.8f), "white"),
        new Knight(glm::vec3(12.3f, 0.0f, 0.8f), "white"),
        new Rook(glm::vec3(0.8f, 0.0f, 17.2f), "white"),
    };
    Piece* blackPieces[] = {
        new Pawn(glm::vec3(left, 0.0f, -12.25f),"black"),
        new Pawn(glm::vec3(left + xStep, 0.0f, -12.25f), "black"),
        new Pawn(glm::vec3(left + 2 * xStep, 0.0f, -12.25f), "black"),
        new Pawn(glm::vec3(left + 3 * xStep, 0.0f, -12.25f), "black"),
        new Pawn(glm::vec3(left + 4 * xStep, 0.0f, -12.25f), "black"),
        new Pawn(glm::vec3(left + 5 * xStep, 0.0f, -12.25f), "black"),
        new Pawn(glm::vec3(left + 6 * xStep, 0.0f, -12.25f), "black"),
        new Pawn(glm::vec3(left + 7 * xStep, 0.0f, -12.25f), "black"),
        new Rook(glm::vec3(-17.2f, 0.0f, -0.7f), "black"),
        new Knight(glm::vec3(-0.5f, 0.0f, -17.15f), "black"),
        new Bishop(glm::vec3(-0.15f, 0.0f, -17.15f), "black"),
        new Queen(glm::vec3(0.0f, 0.0f, -17.15f), "black"),
        new King(glm::vec3(0.05f, 0.0f, -17.15f), "black"),
        new Bishop(glm::vec3(7.35f, 0.0f, -17.15f), "black"),
        new Knight(glm::vec3(12.3f, 0.0f, -17.15f), "black"),
        new Rook(glm::vec3(0.8f, 0.0f, -0.7f), "black"),
    };



    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // BOARD
        board.use();
        board.set(camera, SCR_WIDTH,SCR_HEIGHT);
        board.Draw();

        // PIECES
        for (int i = 0; i < 16; i++) {
            (*whitePieces[i]).use();
            (*whitePieces[i]).set(camera, SCR_WIDTH, SCR_HEIGHT);
            (*whitePieces[i]).Draw();
            (*blackPieces[i]).use();
            (*blackPieces[i]).set(camera, SCR_WIDTH, SCR_HEIGHT);
            (*blackPieces[i]).Draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    for (int i = 0; i < 16; i++) {
        free(whitePieces[i]);
        free(blackPieces[i]);
    }
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}