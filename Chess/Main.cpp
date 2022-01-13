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
void startSequence();
void runSequence(float currentTime, Piece* whitePieces[], Piece* blackPieces[]);

// settings
const unsigned int SCR_WIDTH = 1500;
const unsigned int SCR_HEIGHT = 1000;

// camera
Camera camera(glm::vec3(0.0f, 20.0f, 25.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool boundToObject = false, afterObject = false;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 reflectors[] = {
    glm::vec3(20.0f, 25.0f, 0.0f),
    glm::vec3(-20.0f, 25.0f, 0.0f), 
    glm::vec3(0.0f, 25.0f, 0.0f),
};
glm::vec3 reflectorDirection = glm::vec3(0.0f, 0.0f, 0.0f);
bool isBlinn = false;
float fogIntensity = 0.0f;

// sequence
float sequenceStartTime = -1.0f;
int currentMove = -1;
glm::vec3 sequenceMoves[] = {
    glm::vec3(9.5f, 0.0f, -4.4f),
    glm::vec3(9.5f, 0.0f, -7.25f),
    glm::vec3(12.0f, 0.0f, -4.4f),
    glm::vec3(12.0f, 0.0f, -4.4f),
    glm::vec3(-7.5f, 0.0f, -9.1f),

};

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
    // stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);

    Board board;
    float left = -0.7f;
    float xStep = 2.55f;
    Piece* whitePieces[] = {
        new Pawn(glm::vec3(left, 0.0f, 0.6f),"white",1,0),
        new Pawn(glm::vec3(left + xStep, 0.0f, 0.6f), "white",1,1),
        new Pawn(glm::vec3(left + 2 * xStep, 0.0f, 0.6f), "white",1,2),
        new Pawn(glm::vec3(left + 3 * xStep, 0.0f, 0.6f), "white",1,3),
        new Pawn(glm::vec3(left + 4 * xStep, 0.0f, 0.6f), "white",1,4),
        new Pawn(glm::vec3(left + 5 * xStep, 0.0f, 0.6f), "white",1,5),
        new Pawn(glm::vec3(left + 6 * xStep, 0.0f, 0.6f), "white",1,6),
        new Pawn(glm::vec3(left + 7 * xStep, 0.0f, 0.6f), "white",1,7),
        new Rook(glm::vec3(-17.2f, 0.0f, 17.2f), "white",0,0),
        new Knight(glm::vec3(-0.5f, 0.0f, 0.8f), "white",0,1),
        new Bishop(glm::vec3(-0.15f, 0.0f, 0.8f), "white",0,2),
        new King(glm::vec3(-2.45f, 0.0f, 0.8f), "white",0,4),
        new Queen(glm::vec3(2.5f, 0.0f, 0.8f), "white",0,3),
        new Bishop(glm::vec3(7.35f, 0.0f, 0.8f), "white",0,5),
        new Knight(glm::vec3(12.3f, 0.0f, 0.8f), "white",0,6),
        new Rook(glm::vec3(0.8f, 0.0f, 17.2f), "white",0,7),
    };
    Piece* blackPieces[] = {
        new Pawn(glm::vec3(left, 0.0f, -12.25f),"black",6,0),
        new Pawn(glm::vec3(left + xStep, 0.0f, -12.25f), "black",6,1),
        new Pawn(glm::vec3(left + 2 * xStep, 0.0f, -12.25f), "black",6,2),
        new Pawn(glm::vec3(left + 3 * xStep, 0.0f, -12.25f), "black",6,3),
        new Pawn(glm::vec3(left + 4 * xStep, 0.0f, -12.25f), "black",6,4),
        new Pawn(glm::vec3(left + 5 * xStep, 0.0f, -12.25f), "black",6,5),
        new Pawn(glm::vec3(left + 6 * xStep, 0.0f, -12.25f), "black",6,6),
        new Pawn(glm::vec3(left + 7 * xStep, 0.0f, -12.25f), "black",6,7),
        new Rook(glm::vec3(-17.2f, 0.0f, -0.7f), "black",7,0),
        new Knight(glm::vec3(-0.5f, 0.0f, -17.15f), "black",7,1),
        new Bishop(glm::vec3(-0.15f, 0.0f, -17.15f), "black",7,2),
        new King(glm::vec3(-2.45f, 0.0f, -17.15f), "black",7,4),
        new Queen(glm::vec3(2.5f, 0.0f, -17.15f), "black",7,3),
        new Bishop(glm::vec3(7.35f, 0.0f, -17.15f), "black",7,5),
        new Knight(glm::vec3(12.3f, 0.0f, -17.15f), "black",7,6),
        new Rook(glm::vec3(0.8f, 0.0f, -0.7f), "black",7,7),
    };

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // MOVE SEQUENCE
        if (currentMove!=-1 && currentMove<5) {
            runSequence(currentFrame, whitePieces, blackPieces);
        }

        // BOARD
        board.use();
        board.set(camera, SCR_WIDTH,SCR_HEIGHT, reflectors, reflectorDirection, isBlinn, fogIntensity);
        board.Draw();

        // PIECES
        for (int i = 0; i < 16; i++) {
            (*whitePieces[i]).use();
            (*whitePieces[i]).set(camera, SCR_WIDTH, SCR_HEIGHT, reflectors, reflectorDirection, isBlinn, fogIntensity);
            (*whitePieces[i]).Draw();
            (*blackPieces[i]).use();
            (*blackPieces[i]).set(camera, SCR_WIDTH, SCR_HEIGHT, reflectors, reflectorDirection, isBlinn, fogIntensity);
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
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        boundToObject = false;
        afterObject = false;
        camera.StandardCamera(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        boundToObject = true;
        afterObject = false;
        camera.BoundToObjectCamera(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        boundToObject = false;
        afterObject = true;
        camera.ObjectCamera(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        isBlinn = true;
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
        isBlinn = false;
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        fogIntensity = 0.5f;
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        fogIntensity = 0.0f;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        startSequence();
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
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void startSequence() {
    if (sequenceStartTime != -1.0f) return;
        currentMove = 0;
        sequenceStartTime = (float)glfwGetTime();
}

void runSequence(float currentTime, Piece* whitePieces[], Piece* blackPieces[]) {
    glm::vec3 newPosition = glm::vec3(-100.0f,-100.f,-100.0f);
    float front = 1.8f;
    switch (currentMove) {
    case 0: {
        if (whitePieces[4]->translateVec[2] > sequenceMoves[0][2]) {
            newPosition = whitePieces[4]->translateVec + (currentTime - sequenceStartTime) * (sequenceMoves[0] - (whitePieces[4]->translateToInitialPos)) / 100.0f;
            whitePieces[4]->Move(newPosition);
            reflectorDirection = newPosition - whitePieces[4]->differenceFromCenter;
        }
        else {
            currentMove = 1; 
            sequenceStartTime = (float)glfwGetTime();
        }
        break;
    }
    case 1: {
        if (blackPieces[4]->translateVec[2] < sequenceMoves[1][2]) {
            newPosition = blackPieces[4]->translateVec + (currentTime - sequenceStartTime) * (sequenceMoves[1] - (blackPieces[4]->translateToInitialPos)) / 100.0f;
            blackPieces[4]->Move(newPosition);
            front = -1.0f;
            reflectorDirection = newPosition - blackPieces[4]->differenceFromCenter;
        }
        else {
            front = -1.0f;
            currentMove = 2;
            sequenceStartTime = (float)glfwGetTime();
            blackPieces[4]->translateToInitialPos = blackPieces[4]->translateVec;
        }
        break;
    }
    case 2: {
        if (whitePieces[5]->translateVec[2] > sequenceMoves[2][2]) {
            newPosition = whitePieces[5]->translateVec + (currentTime - sequenceStartTime)* (sequenceMoves[2] - (whitePieces[5]->translateToInitialPos)) / 100.0f;
            whitePieces[5]->Move(newPosition);
            reflectorDirection = newPosition - whitePieces[5]->differenceFromCenter;
        }
        else {
            currentMove = 3;
            sequenceStartTime = (float)glfwGetTime();
        }
        break;
    }
    case 3: {
        if (blackPieces[4]->translateVec[2] < sequenceMoves[3][2]) {
            front = -1.0f;
            newPosition = blackPieces[4]->translateVec + (currentTime - sequenceStartTime) * (sequenceMoves[3] - (blackPieces[4]->translateToInitialPos)) / 50.0f;
            blackPieces[4]->Move(newPosition);
            whitePieces[5]->KnockDown(-90 + (currentTime - sequenceStartTime) * 25.0f);
            reflectorDirection = newPosition - blackPieces[4]->differenceFromCenter;
        }
        else {
            front = -1.0f;
            sequenceStartTime = (float)glfwGetTime();
            whitePieces[5]->isKnockedDown = true;
            currentMove = 4;
        }
        break;
    }
    case 4: {
        if (whitePieces[12]->translateVec[2] > sequenceMoves[4][2]) {
            newPosition = whitePieces[12]->translateVec + (currentTime - sequenceStartTime) * (sequenceMoves[4] - (whitePieces[12]->translateToInitialPos)) / 100.0f;
            whitePieces[12]->Move(newPosition);
            reflectorDirection = newPosition - whitePieces[12]->differenceFromCenter;
        }
        else {
            currentMove = 5;
        }
        break;
    }
    default:break;
    }
    if (newPosition != glm::vec3(-100.0f, -100.f, -100.0f)) {
        if(boundToObject) camera.SetViewMatrix(glm::lookAt(camera.Position, newPosition, camera.Up));
        if (afterObject) { 
            camera.SetViewMatrix(glm::lookAt(glm::vec3(0.0f, 25.0f, newPosition[2] + front * 25.0f), glm::vec3(0.0f, 0.0f, -front * 15.0f), camera.Up));
        }
        
    }
}