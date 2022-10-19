#include "Game_Camera.h"

C_Camera& C_Camera::GetInstance() {
    static C_Camera instance;
    return instance;
}

void C_Camera::Init(const float nearclip , const float farclip ,
    const float fov , const float width , const float height ,
    const XMFLOAT3 eye , const XMFLOAT3 lookat , const XMFLOAT3 up) {
    SetProjection(nearclip , farclip , fov , width , height);
    SetCamera(eye , lookat , up);
}

void C_Camera::Update() {
    CreateCameraMatrix();
    CreateProjectionMatrix();
}

void C_Camera::SetNear(float nearclip) {
    m_near = nearclip;
}

void C_Camera::SetFar(float farclip) {
    m_far = farclip;
}

void C_Camera::SetFov(float fov) {
    m_fov = fov;
}

void C_Camera::SetAspect(float width , float height) {
    m_aspect = width / height;
}

void C_Camera::SetProjection(float nearclip , float farclip , float fov , float width , float height) {
    SetNear(nearclip);
    SetFar(farclip);
    SetFov(fov);
    SetAspect(width , height);
    CreateProjectionMatrix();
}

void C_Camera::SetCamera(const XMFLOAT3& eye , const XMFLOAT3& lookat , const XMFLOAT3& up) {

    SetEye(eye);
    SetLookat(lookat);
    SetUp(up);
    CreateCameraMatrix();
}

void C_Camera::SetEye(const XMFLOAT3& eye) {
    m_Eye = eye;
}

void C_Camera::SetLookat(const XMFLOAT3& lookat) {
    m_Lookat = lookat;
}

void C_Camera::SetUp(const XMFLOAT3& up) {
    m_Up = up;
}

void C_Camera::CreateCameraMatrix() {
    ALIGN16 XMVECTOR Eye = XMVectorSet(m_Eye.x , m_Eye.y , m_Eye.z , 0.0f);
    ALIGN16 XMVECTOR At = XMVectorSet(m_Lookat.x , m_Lookat.y , m_Lookat.z , 0.0f);
    ALIGN16 XMVECTOR Up = XMVectorSet(m_Up.x , m_Up.y , m_Up.z , 0.0f);

    XMMATRIX r_mtx;
    m_viewMtx = XMMatrixLookAtLH(Eye , At , Up);

}

void C_Camera::CreateProjectionMatrix() {

    ALIGN16 XMMATRIX projection;

    m_projectionMtx = XMMatrixPerspectiveFovLH(m_fov , m_aspect , m_near , m_far);

}

const XMMATRIX& C_Camera::GetViewMatrix() {
    return m_viewMtx;
}

const XMMATRIX& C_Camera::GetProjectionMatrix() {
    return m_projectionMtx;
}
