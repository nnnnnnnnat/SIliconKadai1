#pragma once

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

class OpenGL {
public:

    bool Init(HWND hwnd);
    bool Update();
    bool Draw();
    void BeforeRender();
    void AfterRender();
    void Release();

private:

    struct Position {
        float x;
        float y;
        float z;
    };

    struct Color {
        float r;
        float g;
        float b;
    };

    struct Vertex_GL {
        Position postion;
        Color color;
    };

    HDC m_hdc;
    Vertex_GL m_vertex[24];

};