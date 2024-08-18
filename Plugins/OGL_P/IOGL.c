#include <IOGL.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

typedef struct OGL_Resource
{
    OGL_Resource_Desc desc;
    GLuint data[4];
} OGL_Resource;

int OGL_Init(const char *name, int width, int height, void** vg) {
    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Error: Failed to initialize GLFW\n");
        return -1;
    }

    // Set OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 3
                                                    //.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  //3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create GLFW window
    GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);
    if (!window) {
        fprintf(stderr, "Error: Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Load OpenGL functions using glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Error: Failed to initialize GLAD\n");
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

void OGL_StartFrame(void* ctx, float color[4])
{
    glfwMakeContextCurrent((GLFWwindow*)ctx);
    glClearColor(color[0], color[1], color[2], color[3]);
    glClear(GL_COLOR_BUFFER_BIT);
}

void OGL_EndFrame(void* ctx)
{
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
    glfwSwapBuffers((GLFWwindow*)ctx);
}

int OGL_LoadResource(void* ctx, void * desc, void** out) {
    OGL_Resource_Desc *d = (OGL_Resource_Desc*)desc;
    GLenum shaderType;
    GLuint shader;
    GLint compileStatus;
    char log[512];

    switch (d->type) 
    {
    case OGL_TYPE_VERTEX:
    case OGL_TYPE_PIXEL:
        printf("OGL is creating Shader Resource\n");
        shaderType = (d->type == OGL_TYPE_VERTEX) ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;

        shader = glCreateShader(shaderType);
        if (shader == 0) {
            fprintf(stderr, "Error: Failed to create shader (type %d)\n", d->type);
            return -1;
        }

        glShaderSource(shader, 1, &d->file_or_string, NULL);
        glCompileShader(shader);

        glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
        if (compileStatus != GL_TRUE) {
            glGetShaderInfoLog(shader, sizeof(log), NULL, log);
            fprintf(stderr, "Error: Shader compilation failed (type %d): %s\n", d->type, log);
            glDeleteShader(shader);
            return -1;
        }

        *out = calloc(1,sizeof(OGL_Resource));
        
        if (!*out) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            glDeleteShader(shader);
            return -1;
        }

        ((OGL_Resource*)(*out))->data[0] = shader;
        ((OGL_Resource*)(*out))->desc = *d;
        printf("OGL Shader created Successfully\n");
        return 0;
        break;
    case OGL_TYPE_SHADER_PROGRAM:
        printf("OGL is creating a shader program resource\n");
        if (!d->data || d->size == 0) {
            fprintf(stderr, "Error: No shaders provided for program\n");
            return -1;
        }

        GLuint shaderProgram = glCreateProgram();
        if (shaderProgram == 0) {
            fprintf(stderr, "Error: Failed to create shader program\n");
            return -1;
        }
        
        OGL_Resource** data = (OGL_Resource**)d->data;

        for (size_t i = 0; i < d->size; i++) {
        if (!data[i]) {
            fprintf(stderr, "Error: Shader %zu is NULL\n", i);
            glDeleteProgram(shaderProgram);
            return -1;
        }
        glAttachShader(shaderProgram, data[i]->data[0]);
        }

        //Link all the shaders together into the Shader Program
        glLinkProgram(shaderProgram);

        GLint linkStatus;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            glGetProgramInfoLog(shaderProgram, sizeof(log), NULL, log);
            fprintf(stderr, "Error: Shader program linking failed: %s\n", log);
            glDeleteProgram(shaderProgram);
            return -1;
        }

        *out = (unsigned char*)malloc(sizeof(OGL_Resource));
        if (!*out) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            glDeleteProgram(shaderProgram);
            return -1;
        }

        ((OGL_Resource*)(*out))->data[0] = shaderProgram;
        ((OGL_Resource*)(*out))->desc = *d;
         printf("OGL Shader Program created Successfully\n");
        return 0;
        break;
    default:
        fprintf(stderr, "Error: Unknown resource type %d\n", d->type);
        return -1;
    }
}

void OGL_SetResource(void* ctx, void* rs) {
    OGL_Resource* r = (OGL_Resource*)rs;

    switch (r->desc.type) {
    case OGL_TYPE_VERTEX:
    case OGL_TYPE_PIXEL:
        fprintf(stderr, "Error: Cannot set shader resource directly. Use shader programs.\n");
        break;
    case OGL_TYPE_SHADER_PROGRAM:
        glUseProgram(r->data[0]);
        break;
    default:
        fprintf(stderr, "Error: Unknown resource type %d\n", r->desc.type);
        break;
    }
}

void OGL_UninitResource(void* ctx, void * rs) {
    OGL_Resource* r = (OGL_Resource*)rs;

    switch (r->desc.type) {
    case OGL_TYPE_VERTEX:
    case OGL_TYPE_PIXEL:
        if (r->data[0]) {
            glDeleteShader(r->data[0]);
            r->data[0] = 0;
        }
        break;
    case OGL_TYPE_SHADER_PROGRAM:
        if (r->data[0]) {
            glDeleteProgram(r->data[0]);
            r->data[0] = 0;
        }
        break;
    default:
        fprintf(stderr, "Error: Unknown resource type %d\n", r->desc.type);
        break;
    }
}
