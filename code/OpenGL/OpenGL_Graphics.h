#pragma once

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include <windows.h>
#include	<gl/gl.h>
#include	<gl/glu.h>

struct Position_GL {
    float x;
    float y;
    float z;
};

struct Color_GL {
    float r;
    float g;
    float b;
};

struct Vertex_GL {
    Position_GL postion;
    Color_GL color;
};

class OpenGL {
public:

    static bool Initialize(HWND hwnd);
    static bool Update();
    static bool Draw();
    static bool Finalize();

private:
    static HDC m_hdc;
    static Vertex_GL m_vertex[24];

};