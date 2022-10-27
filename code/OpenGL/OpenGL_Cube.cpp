#include "OpenGL_Cube.h"

#include <stdlib.h>

bool OpenGLCube::Init(GameDevice* _pDevice) {
    InitCube(1.0f);


    unsigned char tex[TEXTURE_SIZE_X * TEXTURE_SIZE_Y * 4] = {};

    for (int y = 0; y < TEXTURE_SIZE_Y; y++) {
        for (int x = 0; x < TEXTURE_SIZE_X; x++) {
            tex[y * TEXTURE_SIZE_Y * 4 + x * 4 + 0] = GetColorMap(x , y , 0);
            tex[y * TEXTURE_SIZE_Y * 4 + x * 4 + 1] = GetColorMap(x , y , 1);
            tex[y * TEXTURE_SIZE_Y * 4 + x * 4 + 2] = GetColorMap(x , y , 2);
            tex[y * TEXTURE_SIZE_Y * 4 + x * 4 + 3] = 255;
        }
    }

    glTexImage2D(
        GL_TEXTURE_2D ,
        0 , // mipmap
        GL_RGBA ,
        TEXTURE_SIZE_X , // width
        TEXTURE_SIZE_Y , // height
        0 , // border
        GL_RGBA ,
        GL_UNSIGNED_BYTE ,
        tex
    );

    return true;
}

void OpenGLCube::Update(GameDevice* _pDevice) {
    RotateMatrix();
}

void OpenGLCube::Draw() {

    // カラー、頂点

    glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_NEAREST); // 拡大時近傍
    glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_NEAREST); // 縮小時近傍

    glTexEnvf(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);

    glEnable(GL_TEXTURE_2D);

    glRotatef(GetRotateAngle(0) , 1.0f , 0.0f , 0.0f);
    glRotatef(GetRotateAngle(1) , 0.0f , 1.0f , 0.0f);
    glRotatef(GetRotateAngle(2) , 0.0f , 0.0f , 1.0f);

    for (int i = 0; i < 6; i++) {
        glBegin(GL_TRIANGLE_STRIP);

        for (int j = 0; j < 6; j++) {
            int no = i * 6 + j;
            glTexCoord2f(
                GetCubeVertex()[no].m_tex.x ,
                GetCubeVertex()[no].m_tex.y);
            glVertex3f(
                GetCubeVertex()[no].m_pos.x ,
                GetCubeVertex()[no].m_pos.y ,
                GetCubeVertex()[no].m_pos.z);
        }
        glEnd();
    }
}

void OpenGLCube::Release() {

}
