#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "program.h"
#include "scene.h"
#include "camera.h"
#include "game.h"

class Application
{
public:
    static void start();

private:
    static void display();
    static void initOpenGL();
    static void initGame();

    static void setTitleFps(int fps);
    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void window_size_callback(GLFWwindow* window, int width, int height);
    static void mousepos_callback(GLFWwindow* window, double mouseX, double mouseY);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

    static double lastTime;
    static int nbFrames;
    static int nbFramesLastSecond;
    static GLFWwindow* window;
    static Program program;
    static Program program_shadows;
    static Program program_selection;
    static Program program_skybox;
    static Scene scene;
    static Camera* camera;
    static Game game;
    
    static void renderShadow(int light_index);
    static void renderScene();

    static int window_width, window_height;
    static int framebuffer_width, framebuffer_height;

    // For camera
    static int midWindowX, midWindowY;

    // Selection
    static void processSelection(int xx, int yy);
    static void renderSelection();

    //Skybox
    static void renderSkybox();
    
    static void saveTexture();
    
    static bool skybox_enabled_demo, shadow_enabled_demo, reflection_enabled_demo;
    static bool light_enabled_demo[2];
};
