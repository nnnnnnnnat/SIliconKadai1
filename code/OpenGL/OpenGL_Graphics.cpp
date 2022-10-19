#include "OpenGL_Graphics.h"

HDC OpenGL::m_hdc;
Vertex_GL OpenGL::m_vertex[24];

bool OpenGL::Initialize(HWND hwnd) {
    m_hdc = GetDC(hwnd);

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
    glClearColor(0.0f , 0.0f , 0.0f , 0.4f);
    glDepthRange(0.0 , 1.0);
    glClearDepth(1.0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glViewport(0 , 0 , 1920 / 2 , 1080 / 2);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(-10.0 / 200.0 , 10.0 / 200.0 , -5.5 / 200.0 , 5.5 / 200.0 , 0.1 , 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(5.0 , 5.0 , 5.0 , 0.0 , 0.0 , -10.0 , 0.0 , 1.0 , 0.0);

    glDisable(GL_STENCIL_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);

    for (int i = 0; i < 24; i++) {
        m_vertex[i].color = { 1.0f , 1.0f , 1.0f };
    }

    // 手前
    m_vertex[0].postion = { 1.0f , 1.0f , -9.0f };
    m_vertex[1].postion = { 1.0f , -1.0f , -9.0f };
    m_vertex[2].postion = { -1.0f , -1.0f , -9.0f };
    m_vertex[3].postion = { -1.0f , 1.0f , -9.0f };
    // 後ろ
    m_vertex[4].postion = { -1.0f , 1.0f , -11.0f };
    m_vertex[5].postion = { -1.0f , -1.0f , -11.0f };
    m_vertex[6].postion = { 1.0f , -1.0f , -11.0f };
    m_vertex[7].postion = { 1.0f , 1.0f , -11.0f };
    // 右
    m_vertex[8].postion = { 1.0f , 1.0f , -11.0f };
    m_vertex[9].postion = { 1.0f , -1.0f , -11.0f };
    m_vertex[10].postion = { 1.0f , -1.0f , -9.0f };
    m_vertex[11].postion = { 1.0f , 1.0f , -9.0f };
    // 左
    m_vertex[12].postion = { -1.0f , 1.0f , -9.0f };
    m_vertex[13].postion = { -1.0f , -1.0f , -9.0f };
    m_vertex[14].postion = { -1.0f , -1.0f , -11.0f };
    m_vertex[15].postion = { -1.0f , 1.0f , -11.0f };
    // 上
    m_vertex[16].postion = { 1.0f , 1.0f , -11.0f };
    m_vertex[17].postion = { 1.0f , 1.0f , -9.0f };
    m_vertex[18].postion = { -1.0f , 1.0f , -9.0f };
    m_vertex[19].postion = { -1.0f , 1.0f , -11.0f };
    // 下
    m_vertex[20].postion = { 1.0f , -1.0f , -9.0f };
    m_vertex[21].postion = { 1.0f , -1.0f , -11.0f };
    m_vertex[22].postion = { -1.0f , -1.0f , -11.0f };
    m_vertex[23].postion = { -1.0f , -1.0f , -9.0f };


    return true;
}

bool OpenGL::Update() {
    return false;
}

bool OpenGL::Draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // カラー、頂点

    for (int i = 0; i < 24; i += 4) {
        glBegin(GL_TRIANGLE_FAN);

        for (int j = 0; j < 4; j++) {
            glColor3f(m_vertex[i + j].color.r , m_vertex[i + j].color.g , m_vertex[i + j].color.b);
            glVertex3f(m_vertex[i + j].postion.x , m_vertex[i + j].postion.y , m_vertex[i + j].postion.z);
        }

        glEnd();
    }

    glBegin(GL_TRIANGLE_FAN);


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_TEXTURE_2D);

    glFlush();
    SwapBuffers(m_hdc);

    return false;
}

bool OpenGL::Finalize() {
    return false;
}