#include <VG/inc/VG.h>
#include <math.h>
#include <stdlib.h>
// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";
int main()
{
    // WINDOW
    VG* vg = VG_Create("new", 300, 300);

    // SHADERS
    VG_Resource_Desc vs_desc = VG_VSHADER_DESC(vertexShaderSource, VG_FALSE, "vs");
    VG_Resource* vs = VG_LoadResource(vg, &vs_desc, 0);


    VG_Resource_Desc fs_desc = VG_FSHADER_DESC(fragmentShaderSource, VG_FALSE, "fs");
    VG_Resource* fs = VG_LoadResource(vg, &fs_desc, 0);


    // SHADER PROGRAM
    VG_Resource* shaders[2];
    shaders[0] = vs;
    shaders[1] = fs;

    VG_Resource_Desc sp = VG_SHADER_PROGRAM_DESC("BasicShaderProgram", shaders, 2);
    VG_Resource* ShaderProgram = VG_LoadResource(vg, &sp, 0);

    //VERTS
	// float vertices[] =
	// {
	// 	-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
	// 	0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
	// 	0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
	// 	-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
	// 	0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner right
	// 	0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Inner down
	// };

    
    
    // SET ALL RESOURCES
    VG_SetResources(vg, 1, ShaderProgram);

    // while (true)
    // {
    //     VG_FrameStartA(vg, VG_COLOR_BLACK);


    //     //TODO


    //     VG_FrameEnd(vg);
    // }
    
}