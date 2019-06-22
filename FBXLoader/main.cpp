#include "FBXModel.h"
#include "Camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;

//FPS
double deltaTime2 = 0;
double lastFrame = 0;

double limitFPS = 1.0/30.0;

// Animation
bool startAnimation = false;
float signal2 = 0.0;

int main(int argc, char** argv) {
    
    FbxVector4 ctrlPoint = FbxVector4(1.0,2.0,3.0);
    std::cout << ctrlPoint[0] << " " << ctrlPoint[1] << " " << ctrlPoint[2] << std::endl;
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    
    // build and compile shaders
    // -------------------------
    Shader ourShader("model_loading_vs.txt", "model_loading_fs.txt");
    
    const char* fileName = "SambaDancing.fbx";
    FBXModel fbxmodel(fileName, ourShader);
    
    int frameNum = fbxmodel.getFrameNum();
    
    ourShader.use();
    
    int frameIndex = 0;
    
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        deltaTime2 += (currentFrame - lastFrame) / limitFPS;
        lastFrame = currentFrame;
        
        //std::cout << deltaTime2 << std::endl;
        
        // input
        // -----
        processInput(window);
        
        
        
        // render
        // ------
        while(deltaTime2>=1.0){
            glm::mat4 model = glm::mat4(1.0);
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
            glm::mat4 view = camera.GetViewMatrix();
            ourShader.setMat4("model", model);
            ourShader.setMat4("projection", projection);
            ourShader.setMat4("view", view);
            ourShader.setVec3("lightDirection", glm::vec3(1.0f, 1.0f, 1.0f));
            ourShader.setFloat("signal", signal2);
            ourShader.setVec3("viewPos", camera.Position);
            fbxmodel.SetGlobalBindInverseMatrices(ourShader);
            if(startAnimation){
                fbxmodel.updateAnimation(ourShader, frameIndex);
                frameIndex++;
                if(frameIndex == frameNum)
                    frameIndex=0;
            }
            deltaTime2--;
        }
        
        fbxmodel.draw();
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
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
    
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        signal2 = 1.0;
        std::cout << "Animation start!" << std::endl;
        startAnimation = true;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
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

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

/*
#include <fbxsdk.h>

int numTabs = 0;

void PrintTabs() {
    for(int i = 0; i < numTabs; i++)
        printf("\t");
}

FbxString GetAttributeTypeName(FbxNodeAttribute::EType type) {
    switch(type) {
        case FbxNodeAttribute::eUnknown: return "unidentified";
        case FbxNodeAttribute::eNull: return "null";
        case FbxNodeAttribute::eMarker: return "marker";
        case FbxNodeAttribute::eSkeleton: return "skeleton";
        case FbxNodeAttribute::eMesh: return "mesh";
        case FbxNodeAttribute::eNurbs: return "nurbs";
        case FbxNodeAttribute::ePatch: return "patch";
        case FbxNodeAttribute::eCamera: return "camera";
        case FbxNodeAttribute::eCameraStereo: return "stereo";
        case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
        case FbxNodeAttribute::eLight: return "light";
        case FbxNodeAttribute::eOpticalReference: return "optical reference";
        case FbxNodeAttribute::eOpticalMarker: return "marker";
        case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
        case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
        case FbxNodeAttribute::eBoundary: return "boundary";
        case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
        case FbxNodeAttribute::eShape: return "shape";
        case FbxNodeAttribute::eLODGroup: return "lodgroup";
        case FbxNodeAttribute::eSubDiv: return "subdiv";
        default: return "unknown";
    }
}

void PrintAttribute(FbxNodeAttribute* pAttribute) {
    if(!pAttribute) return;
    
    FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
    FbxString attrName = pAttribute->GetName();
    PrintTabs();
    // Note: to retrieve the character array of a FbxString, use its Buffer() method.
    printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
}

void PrintNode(FbxNode* pNode) {
    PrintTabs();
    const char* nodeName = pNode->GetName();
    FbxDouble3 translation = pNode->LclTranslation.Get();
    FbxDouble3 rotation = pNode->LclRotation.Get();
    FbxDouble3 scaling = pNode->LclScaling.Get();
    
    // Print the contents of the node.
    printf("<node name='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n",
           nodeName,
           translation[0], translation[1], translation[2],
           rotation[0], rotation[1], rotation[2],
           scaling[0], scaling[1], scaling[2]
           );
    numTabs++;
    
    // Print the node's attributes.
    for(int i = 0; i < pNode->GetNodeAttributeCount(); i++)
        PrintAttribute(pNode->GetNodeAttributeByIndex(i));
    
    // Recursively print the children.
    for(int j = 0; j < pNode->GetChildCount(); j++)
        PrintNode(pNode->GetChild(j));
    
    numTabs--;
    PrintTabs();
    printf("</node>\n");
}

int main(int argc, char** argv) {
    
    // Change the following filename to a suitable filename value.
    const char* lFilename = "Pirate.fbx";
    
    // Initialize the SDK manager. This object handles all our memory management.
    FbxManager* lSdkManager = FbxManager::Create();
    
    // Create the IO settings object.
    FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    lSdkManager->SetIOSettings(ios);
    
    // Create an importer using the SDK manager.
    FbxImporter* lImporter = FbxImporter::Create(lSdkManager,"");
    
    // Use the first argument as the filename for the importer.
    if(!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings())) {
        printf("Call to FbxImporter::Initialize() failed.\n");
        printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
        exit(-1);
    }
    
    // Create a new scene so that it can be populated by the imported file.
    FbxScene* lScene = FbxScene::Create(lSdkManager,"myScene");
    
    // Import the contents of the file into the scene.
    lImporter->Import(lScene);
    
    // The file is imported; so get rid of the importer.
    lImporter->Destroy();
    
    // Print the nodes of the scene and their attributes recursively.
    // Note that we are not printing the root node because it should
    // not contain any attributes.
    FbxNode* lRootNode = lScene->GetRootNode();
    if(lRootNode) {
        for(int i = 0; i < lRootNode->GetChildCount(); i++)
            PrintNode(lRootNode->GetChild(i));
    }
    // Destroy the SDK manager and all the other objects it was handling.
    lSdkManager->Destroy();
    return 0;
}
 */
