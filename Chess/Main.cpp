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
#include "Light.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void setToShader(Board board, Piece* whitePieces[], Piece* blackPieces[], Light* light[], Shader s);
void startSequence();
void runSequence(float currentTime, Piece* whitePieces[], Piece* blackPieces[], Light* light[]);

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
    glm::vec3(15.0f, 10.0f, 0.0f),
    glm::vec3(-15.0f, 10.0f, 0.0f), 
    glm::vec3(0.0f, 10.0f, 0.0f),
};
glm::vec3 reflectorDirection = glm::vec3(0.0f, 0.0f, 0.0f);
bool isBlinn = false;
float fogIntensity = 0.0f;
bool isPhong = true;
bool isDay = false;
bool isFog = false;
float lightIntensity = 0.2f;

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

    glEnable(GL_DEPTH_TEST);
    Shader phongShader = Shader("phongShader.vs", "phongShader.fs");
    Shader gourardShader = Shader("gourardShader.vs", "gourardShader.fs");
    phongShader.setDeafult();
    gourardShader.setDeafult();
    Board board(phongShader);
    Light* light[] = {
        new Light(glm::vec3(15.0f, 10.0f, 0.0f), glm::radians(215.0f), phongShader),
        new Light(glm::vec3(-15.0f, 10.0f, 0.0f), glm::radians(-35.0f), phongShader),
        new Light(glm::vec3(0.0f, 10.0f, 0.0f), glm::radians(-90.0f), phongShader),
    };
    float left = -0.7f;
    float xStep = 2.55f;
    Piece* whitePieces[] = {
        new Pawn(glm::vec3(left, 0.0f, 0.6f),"white", phongShader),
        new Pawn(glm::vec3(left + xStep, 0.0f, 0.6f), "white", phongShader),
        new Pawn(glm::vec3(left + 2 * xStep, 0.0f, 0.6f), "white", phongShader),
        new Pawn(glm::vec3(left + 3 * xStep, 0.0f, 0.6f), "white", phongShader),
        new Pawn(glm::vec3(left + 4 * xStep, 0.0f, 0.6f), "white", phongShader),
        new Pawn(glm::vec3(left + 5 * xStep, 0.0f, 0.6f), "white", phongShader),
        new Pawn(glm::vec3(left + 6 * xStep, 0.0f, 0.6f), "white", phongShader),
        new Pawn(glm::vec3(left + 7 * xStep, 0.0f, 0.6f), "white", phongShader),
        new Rook(glm::vec3(-17.2f, 0.0f, 17.2f), "white", phongShader),
        new Knight(glm::vec3(-0.5f, 0.0f, 0.8f), "white", phongShader),
        new Bishop(glm::vec3(-0.15f, 0.0f, 0.8f), "white", phongShader),
        new King(glm::vec3(-2.45f, 0.0f, 0.8f), "white", phongShader),
        new Queen(glm::vec3(2.5f, 0.0f, 0.8f), "white", phongShader),
        new Bishop(glm::vec3(7.35f, 0.0f, 0.8f), "white", phongShader),
        new Knight(glm::vec3(12.3f, 0.0f, 0.8f), "white", phongShader),
        new Rook(glm::vec3(0.8f, 0.0f, 17.2f), "white", phongShader),
    };
    Piece* blackPieces[] = {
        new Pawn(glm::vec3(left, 0.0f, -12.25f),"black", phongShader),
        new Pawn(glm::vec3(left + xStep, 0.0f, -12.25f), "black", phongShader),
        new Pawn(glm::vec3(left + 2 * xStep, 0.0f, -12.25f), "black", phongShader),
        new Pawn(glm::vec3(left + 3 * xStep, 0.0f, -12.25f), "black", phongShader),
        new Pawn(glm::vec3(left + 4 * xStep, 0.0f, -12.25f), "black", phongShader),
        new Pawn(glm::vec3(left + 5 * xStep, 0.0f, -12.25f), "black", phongShader),
        new Pawn(glm::vec3(left + 6 * xStep, 0.0f, -12.25f), "black", phongShader),
        new Pawn(glm::vec3(left + 7 * xStep, 0.0f, -12.25f), "black", phongShader),
        new Rook(glm::vec3(-17.2f, 0.0f, -0.7f), "black", phongShader),
        new Knight(glm::vec3(-0.5f, 0.0f, -17.15f), "black", phongShader),
        new Bishop(glm::vec3(-0.15f, 0.0f, -17.15f), "black", phongShader),
        new King(glm::vec3(-2.45f, 0.0f, -17.15f), "black", phongShader),
        new Queen(glm::vec3(2.5f, 0.0f, -17.15f), "black", phongShader),
        new Bishop(glm::vec3(7.35f, 0.0f, -17.15f), "black", phongShader),
        new Knight(glm::vec3(12.3f, 0.0f, -17.15f), "black", phongShader),
        new Rook(glm::vec3(0.8f, 0.0f, -0.7f), "black", phongShader),
    };

    
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        if(isPhong) setToShader(board, whitePieces, blackPieces, light, phongShader);
        else setToShader(board, whitePieces, blackPieces, light, gourardShader);

        if (isDay && lightIntensity<0.6f) {
            lightIntensity += 0.005f;
        }
        if (!isDay && lightIntensity > 0.2f) {
            lightIntensity -= 0.005f;
        }
        if (isFog && fogIntensity < 0.3f) {
            fogIntensity += 0.005f;
        }
        if (!isFog && fogIntensity > 0.0f) {
            fogIntensity -= 0.005f;
        }

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // MOVE SEQUENCE
        if (currentMove!=-1 && currentMove<5) {
            runSequence(currentFrame, whitePieces, blackPieces, light);
        }

        // BOARD
        board.use();
        board.set(camera, SCR_WIDTH,SCR_HEIGHT, reflectors, reflectorDirection, isBlinn, fogIntensity, lightIntensity);
        board.Draw();
        for (int i = 0; i < 3; i++) {
            light[i]->use();
            light[i]->set(camera, SCR_WIDTH, SCR_HEIGHT, reflectors, reflectorDirection, isBlinn, fogIntensity, lightIntensity);
            light[i]->Draw();
        }
        

        // PIECES
        for (int i = 0; i < 16; i++) {
            (*whitePieces[i]).use();
            (*whitePieces[i]).set(camera, SCR_WIDTH, SCR_HEIGHT, reflectors, reflectorDirection, isBlinn, fogIntensity, lightIntensity);
            (*whitePieces[i]).Draw();
            (*blackPieces[i]).use();
            (*blackPieces[i]).set(camera, SCR_WIDTH, SCR_HEIGHT, reflectors, reflectorDirection, isBlinn, fogIntensity, lightIntensity);
            (*blackPieces[i]).Draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    for (int i = 0; i < 16; i++) {
        free(whitePieces[i]);
        free(blackPieces[i]);
    }
    for (int i = 0; i < 3; i++) {
        free(light[i]);
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
        isFog = true;
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        isFog = false;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        isPhong = false;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        isPhong = true;
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        isDay = true;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        isDay = false;

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

void runSequence(float currentTime, Piece* whitePieces[], Piece* blackPieces[], Light* light[]) {
    glm::vec3 newPosition = glm::vec3(-100.0f,-100.f,-100.0f);
    float front = 1.8f;
    switch (currentMove) {
    case 0: {
        if (whitePieces[4]->translateVec[2] > sequenceMoves[0][2]) {
            newPosition = whitePieces[4]->translateVec + (currentTime - sequenceStartTime) * (sequenceMoves[0] - (whitePieces[4]->translateToInitialPos)) / 100.0f;
            whitePieces[4]->Move(newPosition);
            reflectorDirection = newPosition - whitePieces[4]->differenceFromCenter;
            light[2]->changeDirection(reflectorDirection);
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
            light[2]->changeDirection(reflectorDirection);
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
            light[2]->changeDirection(reflectorDirection);
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
            light[2]->changeDirection(reflectorDirection);
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
            light[2]->changeDirection(reflectorDirection);
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
            camera.SetViewMatrix(glm::lookAt(glm::vec3(0.0f, 25.0f, newPosition[2] + front * 25.0f), glm::vec3(0.0f, 0.0f, -front * 18.0f), camera.Up));
            camera.Position = glm::vec3(0.0f, 25.0f, newPosition[2] + front * 25.0f);
        }
    }
}

void setToShader(Board board, Piece* whitePieces[], Piece* blackPieces[], Light* light[], Shader s) {
    board.setShader(s);
    for (int i = 0; i < 16; i++) {
        whitePieces[i]->setShader(s);
        blackPieces[i]->setShader(s);
    }
    for (int i = 0; i < 3; i++) {
        light[i]->setShader(s);
    }
}