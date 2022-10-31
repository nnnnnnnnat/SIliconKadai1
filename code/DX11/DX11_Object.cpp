//==============================================================================
/// Filename: DX11_Cube.cpp
/// Description: �{�b�N�X�\������N���X
/// Copyright (C) Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#include "DX11_Object.h"
#include "DX11_Graphics.h"

bool DX11Object::Init(GameDevice* _pDevice) {

    HRESULT hr;
    InitCube();
    m_pDevice = dynamic_cast<DX11Graphics*>( _pDevice )->GetDXDevice();
    m_pDeviceContext = dynamic_cast<DX11Graphics*>( _pDevice )->GetDeviceContext();

    //-----------------------------
    // ���_�o�b�t�@�쐬
    D3D11_BUFFER_DESC vbDesc = {};
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // �f�o�C�X�Ƀo�C���h����Ƃ��̎��(���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�A�萔�o�b�t�@�Ȃ�)
    vbDesc.ByteWidth = sizeof(GetCubeVertex()); // �쐬����o�b�t�@�̃o�C�g�T�C�Y
    vbDesc.MiscFlags = 0; // ���̑��̃t���O
    vbDesc.StructureByteStride = 0;	// �\�����o�b�t�@�̏ꍇ�A���̍\���̂̃T�C�Y
    vbDesc.Usage = D3D11_USAGE_DEFAULT;	// �쐬����o�b�t�@�̎g�p�@
    vbDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA initData = { &GetCubeVertex()[0] , sizeof(GetCubeVertex()) , 0 }; // �������ރf�[�^
    // ���_�o�b�t�@�̍쐬
    hr = m_pDevice->CreateBuffer(&vbDesc , &initData , &m_pVertexBuffer);
    if (FAILED(hr)) { // if
        MessageBoxA(nullptr , "CreateBuffer" , "" , MB_OK);
        return false;
    }

    // ���_�V�F�[�_�[��ǂݍ��݁��R���p�C��
    ComPtr<ID3DBlob> compiledVS;

    hr = D3DCompileFromFile(L"shader/SpriteShader.hlsl" ,
        nullptr ,
        nullptr ,
        "VS" ,
        "vs_5_0" ,
        0 ,
        0 ,
        &compiledVS ,
        nullptr);
    if (FAILED(hr)) { // if
        MessageBoxA(nullptr , "vs" , "" , MB_OK);
        return false;
    }

    // �s�N�Z���V�F�[�_�[��ǂݍ��݁��R���p�C��
    ComPtr<ID3DBlob> compiledPS;

    hr = D3DCompileFromFile(L"shader/SpriteShader.hlsl" ,
        nullptr ,
        nullptr ,
        "PS" ,
        "ps_5_0" ,
        0 ,
        0 ,
        &compiledPS ,
        nullptr);
    if (FAILED(hr)) { // if
        MessageBoxA(nullptr , "ps" , "" , MB_OK);
        return false;
    }

    // ���_�V�F�[�_�[�쐬
    hr = m_pDevice->CreateVertexShader(compiledVS->GetBufferPointer() ,
        compiledVS->GetBufferSize() ,
        nullptr ,
        &m_pVertexShader);
    if (FAILED(hr)) { // if
        MessageBoxA(nullptr , "vx create" , "" , MB_OK);
        return false;
    }

    // �s�N�Z���V�F�[�_�[�쐬
    hr = m_pDevice->CreatePixelShader(compiledPS->GetBufferPointer() ,
        compiledPS->GetBufferSize() ,
        nullptr ,
        &m_pPixelShader);
    if (FAILED(hr)) { // if
        MessageBoxA(nullptr , "ps create" , "" , MB_OK);
        return false;
    }

    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION" , 0 , DXGI_FORMAT_R32G32B32_FLOAT , 0 , D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 } ,
        { "COLOR" , 0 , DXGI_FORMAT_R32G32B32A32_FLOAT , 0 , D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 } ,
        { "TEXCOORD" , 0 , DXGI_FORMAT_R32G32_FLOAT , 0 , D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 }
    };

    hr = m_pDevice->CreateInputLayout(
        layout ,
        ARRAYSIZE(layout) ,
        compiledVS->GetBufferPointer() ,
        compiledVS->GetBufferSize() ,
        &m_pInputLayout);

    // ���_�C���v�b�g���C�A�E�g�쐬
    if (FAILED(hr)) { // if
        MessageBoxA(nullptr , "CreateInputLayout" , "" , MB_OK);
        return false;
    }

    // �e�N�X�`���쐬
    Microsoft::WRL::ComPtr<ID3D11Texture2D> D3DTexture;
    D3D11_TEXTURE2D_DESC td;
    td.Width = TEXTURE_SIZE_X;
    td.Height = TEXTURE_SIZE_Y;
    td.MipLevels = 1;
    td.ArraySize = 1;
    td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    td.SampleDesc.Count = 1;
    td.SampleDesc.Quality = 0;
    td.Usage = D3D11_USAGE_DYNAMIC;
    td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    td.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    td.MiscFlags = 0;

    hr = m_pDevice->CreateTexture2D(&td , nullptr , &D3DTexture);
    if (FAILED(hr)) { // if
        MessageBoxA(nullptr , "CreateTexture2D" , "" , MB_OK);
        return false;
    }

    //�e�N�X�`�������ւ�
    D3D11_MAPPED_SUBRESOURCE msr;
    m_pDeviceContext->Map(D3DTexture.Get() , 0 , D3D11_MAP_WRITE_DISCARD , 0 , &msr);

    byte srcData[TEXTURE_SIZE_X * TEXTURE_SIZE_Y * 4] = { 0 };// �r�b�g�}�b�v�����ŏ�����
    enum {
        R ,
        G ,
        B ,
        A ,
    };
    for (int y = 0; y < TEXTURE_SIZE_Y; y++) { // for
        for (int x = 0; x < TEXTURE_SIZE_X; x++) { // for
            srcData[y * TEXTURE_SIZE_Y * 4 + x * 4 + R] = GetTexturePixelColor(x , y , R);
            srcData[y * TEXTURE_SIZE_Y * 4 + x * 4 + G] = GetTexturePixelColor(x , y , G);
            srcData[y * TEXTURE_SIZE_Y * 4 + x * 4 + B] = GetTexturePixelColor(x , y , B);
            srcData[y * TEXTURE_SIZE_Y * 4 + x * 4 + A] = GetTexturePixelColor(x , y , A);
        }
    }
    memcpy(msr.pData , srcData , sizeof(srcData));

    m_pDeviceContext->Unmap(D3DTexture.Get() , 0);

    //�V�F�[�_���\�[�X�r���[�̍쐬
    D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
    srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srv.Texture2D.MipLevels = 1;
    hr = m_pDevice->CreateShaderResourceView(D3DTexture.Get() , &srv , &m_pShaderResourceView);
    if (FAILED(hr)) { // if
        MessageBoxA(nullptr , "CreateShaderResourceView" , "" , MB_OK);
        return false;
    }

    return true;
}

void DX11Object::Update(GameDevice* _pDevice) {
    RotateMatrix();
}

void DX11Object::Draw() {

    HRESULT hr;

    D3D11_BUFFER_DESC constantBufferDesc;
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.ByteWidth = sizeof(ConstantBuffer);
    constantBufferDesc.CPUAccessFlags = 0;
    constantBufferDesc.MiscFlags = 0;
    constantBufferDesc.StructureByteStride = 0;
    constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    ID3D11Buffer* cb;

    hr = m_pDevice->CreateBuffer(&constantBufferDesc , nullptr , &cb);

    if (FAILED(hr)) { // if
        MessageBoxA(nullptr , "CreateBuffer" , "" , MB_OK);
    }

    // ���[���h�ϊ��s��
    DirectX::XMMATRIX worldMatlix = DirectX::XMMatrixIdentity();
    DirectX::XMFLOAT4X4 mtx = GetMatrix();
    worldMatlix = DirectX::XMLoadFloat4x4(&mtx);

    // �r���[�ϊ��s��
    DirectX::XMMATRIX viewMatrix = GameCamera::GetInstance().GetViewMatrix();

    // �v���W�F�N�V�����ϊ��s��
    DirectX::XMMATRIX projectionMatrix =
        GameCamera::GetInstance().GetProjectionMatrix();

    // �萔�o�b�t�@�ݒ�
    ConstantBuffer constBuffer;

    DirectX::XMStoreFloat4x4(&constBuffer.m_world ,
        DirectX::XMMatrixTranspose(worldMatlix));
    DirectX::XMStoreFloat4x4(&constBuffer.m_view ,
        DirectX::XMMatrixTranspose(viewMatrix));
    DirectX::XMStoreFloat4x4(&constBuffer.m_projection ,
        DirectX::XMMatrixTranspose(projectionMatrix));

    if (cb) {
        m_pDeviceContext->UpdateSubresource(cb , 0 , nullptr , &constBuffer , 0 , 0);
    }

    // ���_�o�b�t�@��`��Ŏg����悤�ɃZ�b�g����
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_pDeviceContext->IASetVertexBuffers(0 ,
        1 ,
        m_pVertexBuffer.GetAddressOf() ,
        &stride ,
        &offset);

    //�g�|���W�[ �Z�b�g
    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // �萔�o�b�t�@ �Z�b�g
    m_pDeviceContext->VSSetConstantBuffers(0 , 1 , &cb);

    // �V�F�[�_�[ �Z�b�g
    m_pDeviceContext->VSSetShader(m_pVertexShader.Get() , 0 , 0);
    m_pDeviceContext->PSSetShader(m_pPixelShader.Get() , 0 , 0);

    // �C���v�b�g���C�A�E�g �Z�b�g
    m_pDeviceContext->IASetInputLayout(m_pInputLayout.Get());

    // �V�F�[�_�[���\�[�X�r���[ �Z�b�g
    m_pDeviceContext->PSSetShaderResources(0 , 1 , m_pShaderResourceView.GetAddressOf());

    // �r���[�|�[�g�ݒ�
    D3D11_VIEWPORT m_viewport = { 0.0f ,
        0.0f ,
        (float)SCREEN_WIDTH ,
        (float)SCREEN_HEIGHT ,
        0.0f ,
        1.0f };
    m_pDeviceContext->RSSetViewports(1 , &m_viewport);

    // �萔�o�b�t�@ ���
    cb->Release();

    // �`��
    m_pDeviceContext->Draw((unsigned int)GetCubeVertex().size() , 0);
}

void DX11Object::Release() {

}

