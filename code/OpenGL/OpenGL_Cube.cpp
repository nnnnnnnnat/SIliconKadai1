#include "OpenGL_Cube.h"

void OpenGLCube::Init(GameCube* _cube) {
    m_cube = _cube;
}

void OpenGLCube::Update() {
    m_cube->Update();
}

void OpenGLCube::Draw() {

    // カラー、頂点

    float color[6][3] = {
        { 1.0f , 0.0f , 0.0f } ,
        { 0.0f , 1.0f , 0.0f } ,
        { 0.0f , 0.0f , 1.0f } ,
        { 1.0f , 1.0f , 0.0f } ,
        { 1.0f , 0.0f , 1.0f } ,
        { 0.0f , 1.0f , 1.0f }
    };

    for (int i = 0; i < 6; i++) {
        glBegin(GL_TRIANGLE_STRIP);

        for (int j = 0; j < 6; j++) {
            int no = i * 6 + j;
            glColor3f(
                color[i][0] ,
                color[i][1] ,
                color[i][2]);
            glVertex3f(
                m_cube->GetCubevertex()[no].m_pos.x ,
                m_cube->GetCubevertex()[no].m_pos.y ,
                m_cube->GetCubevertex()[no].m_pos.z);
        }

        glEnd();
    }
}
