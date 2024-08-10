#include <IOGL.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
// Function to initialize OpenGL

int OpenGL_Init(const char *name, int width, int height, void** vg) {
    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "FAILED to initialize GLFW\n");
        return -1;
    }

    // Set OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create GLFW window
    GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);
    if (!window) {
        fprintf(stderr, "FAILED to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Load OpenGL functions using glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "FAILED to initialize GLAD\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, width, height);

    // Store context in vg 
    if (vg) {
        *vg = (void*)window;
    }

    return 0;
}

typedef struct OGL_Resource {
    unsigned int api_specific_data[4];
    const char * file_or_string;
    OGL_TYPE type;
} OGL_Resource;

int OpenGL_InitResource(void* ctx, void *resource)
{
    OGL_Resource *res = (OGL_Resource *)resource;

    switch (res->type)
    {
    case OGL_TYPE_VERTEX:
    case OGL_TYPE_PIXEL:
    {
        printf("OGL is creating shader resource\n");
        GLenum shaderType = (res->type == OGL_TYPE_VERTEX) ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;

        // Create the shader
        GLuint shader = glCreateShader(shaderType);
        if (shader == 0) {
            fprintf(stderr, "OGL Failed to create shader.\n");
            return -1;
        }

        glShaderSource(shader, 1, &res->file_or_string, NULL);

        glCompileShader(shader);

        GLint compileStatus;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
        if (compileStatus != GL_TRUE) {
            char log[512];
            glGetShaderInfoLog(shader, sizeof(log), NULL, log);
            fprintf(stderr, "OGL Shader compilation failed: %s\n", log);
            glDeleteShader(shader);
            return -1;
        }
        
        res->api_specific_data[0] = shader;
        break;
    }
    default:
        fprintf(stderr, "OGL Faild to create shader.\n");
        fprintf(stderr, "Unknown resource type.\n");
        return -1;
    }
    printf("OGL created shader resource.\n");
    return 0;
}

void OpenGL_SetResource(void* ctx, void * resource)
{
    OGL_Resource *res = (OGL_Resource *)resource;
    
    switch (res->type) {
    case OGL_TYPE_VERTEX:
    case OGL_TYPE_PIXEL:

        break;
    default:
        fprintf(stderr, "OGL Failed to set resource.\n");
        fprintf(stderr, "Unknown resource type.\n");
        break;
    }

}

void OpenGL_UninitResource(void* ctx, void *resource)
{
    OGL_Resource *res = (OGL_Resource *)resource;

    switch (res->type) {
    case OGL_TYPE_VERTEX:
    case OGL_TYPE_PIXEL:
        if (res->api_specific_data[0]) {
            glDeleteShader(res->api_specific_data[0]);
            res->api_specific_data[0] = 0;
        }
        break;
    default:
        fprintf(stderr, "OGL Failed to uninit resource.\n");
        fprintf(stderr, "Unknown resource type.\n");
        break;
    }
}