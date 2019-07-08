//
//  Renderer.h
//  FBXLoader
//
//  Created by Jeehoon Hyun on 08/07/2019.
//  Copyright © 2019 Jeehoon Hyun. All rights reserved.
//

#ifndef Renderer_h
#define Renderer_h

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "Shader.h"
#include "FBXModel.h"

class Renderer{
private:
    FBXModel * fbxmodel;
    int frameNum;
    void configureOpenGLSettings();
    void compileShader();
    int frameIndex = 0;
    GLFWwindow * window;
    Shader * ourShader;
    int initialize();
    
public:
    Renderer(const char* fileName);
    void renderLoop();
};

#endif /* Renderer_h */
