//==============================================================================
/// Filename: OpenGL_Graphics.cpp
/// Description: OpenGLèâä˙âª
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#include "OpenGL_Graphics.h"

OpenGLGraphics& GetInstance() {
    static OpenGLGraphics instance;
    return instance;
}

bool OpenGLGraphics::Init(HWND _hWnd) {

    m_hdc = GetDC(_hWnd);

    PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR) ,
        1 ,
        PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER ,
        PFD_TYPE_RGBA ,
        24 ,
        0 , 0 , 0 , 0 , 0 , 0 ,
        0 ,
        0 ,
        0 ,
        0 , 0 , 0 , 0 ,
        32 ,
        0 ,
        0 ,
        PFD_MAIN_PLANE ,
        0 ,
        0 , 0 , 0 };

    HGLRC hrc;
    int pixel_format;

    pixel_format = ChoosePixelFormat(m_hdc , &pfd);
    SetPixelFormat(m_hdc , pixel_format , &pfd);

    hrc = wglCreateContext(m_hdc);
    wglMakeCurrent(m_hdc , hrc);

    glPixelStorei(GL_UNPACK_ALIGNMENT , 1);
    glDepthRange(0.0 , 1.0);
    glClearDepth(1.0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glViewport(0 , 0 , SCREEN_WIDTH , SCREEN_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(-10.0 / 200.0 , 10.0 / 200.0 , -5.5 / 200.0 , 5.5 / 200.0 , 0.1 , 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    DirectX::XMFLOAT4 pos = GameCamera::GetInstance().GetPos();
    DirectX::XMFLOAT4 lookat = GameCamera::GetInstance().GetLookAt();
    DirectX::XMFLOAT4 up = GameCamera::GetInstance().GetUpVector();
    gluLookAt(pos.x , pos.y , -pos.z ,
        lookat.x , lookat.y , lookat.z ,
        up.x , up.y , up.z);

    glDisable(GL_STENCIL_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);

    return true;
}

void OpenGLGraphics::Update() {

}

void OpenGLGraphics::BeforeRender() {
    glClearColor(m_backColor[0] ,
        m_backColor[1] ,
        m_backColor[2] ,
        m_backColor[3]);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLGraphics::AfterRender() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_TEXTURE_2D);

    glFlush();
    SwapBuffers(m_hdc);
}

void OpenGLGraphics::Release() {

}