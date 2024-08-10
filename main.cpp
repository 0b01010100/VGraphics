#include <VG/inc/VG.h>
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
    VG_Win l = {0};
    l.height = 300;
    l.width = 300;
    l.name = "d";
    VG* w = VG_ctor(l);

    VG_Resource vs;
    vs.file_or_string = vertexShaderSource;
    vs.type = VG_TYPE_VERTEX;

    VG_Resource fs;
    fs.file_or_string = fragmentShaderSource;
    fs.type = VG_TYPE_PIXEL;
    VG_ctor_Resources
    (
        w, 2, 
        VG_Resource_Param(vs), 
        VG_Resource_Param(fs)
    );
    VG_dtor_Resources(w, 2, 
        VG_Resource_Param(vs), 
        VG_Resource_Param(fs)
    );
    VG_Window_Loop(w);
}