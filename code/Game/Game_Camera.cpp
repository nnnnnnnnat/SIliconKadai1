//==============================================================================
/// Filename: Game_Camera.cpp
/// Description: ÉJÉÅÉâÇ…ä÷Ç∑ÇÈÉNÉâÉX
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#include "Game_Camera.h"

GameCamera& GameCamera::GetInstance() {
    static GameCamera instance;
    return instance;
}

void GameCamera::Init(const float nearclip , const float farclip ,
    const float fov , const float width , const float height ,
    const XMFLOAT4& eye , const XMFLOAT4& lookat , const XMFLOAT4& up) {
    SetProjection(nearclip , farclip , fov , width , height);
    SetCamera(eye , lookat , up);
}

void GameCamera::Update() {
    CreateCameraMatrix();
    CreateProjectionMatrix();
}

void GameCamera::SetNear(float nearclip) {
    m_near = nearclip;
}

void GameCamera::SetFar(float farclip) {
    m_far = farclip;
}

void GameCamera::SetFov(float fov) {
    m_fov = fov;
}

void GameCamera::SetAspect(float width , float height) {
    m_aspect = width / height;
}

void GameCamera::SetProjection(float nearclip , float farclip , float fov , float width , float height) {
    SetNear(nearclip);
    SetFar(farclip);
    SetFov(fov);
    SetAspect(width , height);
    CreateProjectionMatrix();
}

void GameCamera::SetCamera(const XMFLOAT4& eye , const XMFLOAT4& lookat , const XMFLOAT4& up) {

    SetEye(eye);
    SetLookat(lookat);
    SetUp(up);
    CreateCameraMatrix();
}

void GameCamera::SetEye(const XMFLOAT4& eye) {
    m_eye = eye;
}

void GameCamera::SetLookat(const XMFLOAT4& lookat) {
    m_lookat = lookat;
}

void GameCamera::SetUp(const XMFLOAT4& up) {
    m_up = up;
}

void GameCamera::CreateCameraMatrix() {

    ALIGN16 XMVECTOR Eye = XMVectorSet(m_eye.x , m_eye.y , m_eye.z , 0.0f);
    ALIGN16 XMVECTOR At = XMVectorSet(m_lookat.x , m_lookat.y , m_lookat.z , 0.0f);
    ALIGN16 XMVECTOR Up = XMVectorSet(m_up.x , m_up.y , m_up.z , 0.0f);

    m_viewMtx = XMMatrixLookAtLH(Eye , At , Up);

}

void GameCamera::CreateProjectionMatrix() {

    m_projectionMtx = XMMatrixPerspectiveFovLH(m_fov , m_aspect , m_near , m_far);

}

const XMMATRIX GameCamera::GetViewMatrix() {
    return m_viewMtx;
}

const XMMATRIX GameCamera::GetProjectionMatrix() {
    return m_projectionMtx;
}

const XMFLOAT4 GameCamera::GetPos() {
    return m_eye;
}

const XMFLOAT4 GameCamera::GetLookAt() {
    return m_lookat;
}

const XMFLOAT4 GameCamera::GetUpVector() {
    return m_up;
}

const float GameCamera::GetNearClip() {
    return m_near;
}

const float GameCamera::GetFarClip() {
    return m_far;
}

const float GameCamera::GetAspect() {
    return m_aspect;
}

const float GameCamera::GetFov() {
    return m_fov;
}
