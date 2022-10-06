#include <vector>
#include <random>

#include <DirectXMath.h>

#include "DX11_Polygon.h"
#include "DX11_Graphics.h"
#include "DX11_Shader.h"
#include "DX11_Settransform.h"

bool DX11Polygon::Initialize(
    const float _width ,
    const float _height ,
    DirectX::XMFLOAT4 _color) {

    m_vertex[0] = { { -_width , +_height , 0.0f } , { 0.0f , 0.0f } , _color };
    m_vertex[1] = { { +_width , +_height , 0.0f } , { 0.0f , 1.0f } , _color };
    m_vertex[2] = { { -_width , -_height , 0.0f } , { 1.0f , 0.0f } , _color };
    m_vertex[3] = { { +_width , -_height , 0.0f } , { 1.0f , 1.0f } , _color };

    ID3D11Device* dev;
    ID3D11DeviceContext* devcontext;
    HRESULT hr;

    dev = DX11Graphics::GetInstance().GetDXDevice();
    devcontext = DX11Graphics::GetInstance().GetDeviceContext();

    //-----------------------------
    // ���_�o�b�t�@�쐬
    // �E��L�ō�����R�̒��_�͂��̂܂܂ł͕`��Ɏg�p�ł��Ȃ��񂷁c
    // �E�r�f�I���������Ɂu���_�o�b�t�@�v�Ƃ����`�ō��K�v������܂��I
    // �E����͌����������āA���̏�ō���āA�g���āA�����̂Ă܂��B
    //-----------------------------
    // �쐬����o�b�t�@�̎d�l�����߂�
    // �E����͒��_�o�b�t�@�ɂ��邼�I���Ċ����̐ݒ�
    D3D11_BUFFER_DESC vbDesc = {};
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// �f�o�C�X�Ƀo�C���h����Ƃ��̎��(���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�A�萔�o�b�t�@�Ȃ�)
    vbDesc.ByteWidth = sizeof(m_vertex);					// �쐬����o�b�t�@�̃o�C�g�T�C�Y
    vbDesc.MiscFlags = 0;							// ���̑��̃t���O
    vbDesc.StructureByteStride = 0;					// �\�����o�b�t�@�̏ꍇ�A���̍\���̂̃T�C�Y
    vbDesc.Usage = D3D11_USAGE_DEFAULT;				// �쐬����o�b�t�@�̎g�p�@
    vbDesc.CPUAccessFlags = 0;

    // ��̎d�l��n���Ē��_�o�b�t�@������Ă��炤
    // �������f�o�C�X����ɂ��肢����
    D3D11_SUBRESOURCE_DATA initData = { &m_vertex[0] , sizeof(m_vertex) , 0 };	// �������ރf�[�^
    // ���_�o�b�t�@�̍쐬
    dev->CreateBuffer(&vbDesc , &initData , &mp_vertexBuffer);

    //=====================================================
    // �V�F�[�_�[�̍쐬
    //=====================================================
    // ���_�V�F�[�_�[��ǂݍ��݁��R���p�C��
    ComPtr<ID3DBlob> compiledVS;
    if (FAILED(D3DCompileFromFile(VS_FILENAME , nullptr , nullptr , "VS" , "vs_5_0" , 0 , 0 , &compiledVS , nullptr))) {
        MessageBox(nullptr , "vs" , "" , MB_OK);
        return false;
    }
    // �s�N�Z���V�F�[�_�[��ǂݍ��݁��R���p�C��
    ComPtr<ID3DBlob> compiledPS;
    if (FAILED(D3DCompileFromFile(PS_FILENAME , nullptr , nullptr , "PS" , "ps_5_0" , 0 , 0 , &compiledPS , nullptr))) {
        MessageBox(nullptr , "ps" , "" , MB_OK);
        return false;
    }

    // ���_�V�F�[�_�[�쐬
    if (FAILED(dev->CreateVertexShader(compiledVS->GetBufferPointer() ,
        compiledVS->GetBufferSize() ,
        nullptr ,
        &mp_vertexShader))) {
        MessageBox(nullptr , "vx create" , "" , MB_OK);
        return false;
    }
    // �s�N�Z���V�F�[�_�[�쐬
    if (FAILED(dev->CreatePixelShader(compiledPS->GetBufferPointer() ,
        compiledPS->GetBufferSize() ,
        nullptr ,
        &mp_pixelShader))) {
        MessageBox(nullptr , "ps create" , "" , MB_OK);
        return false;
    }

    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION" , 0 , DXGI_FORMAT_R32G32B32_FLOAT , 0 , D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 } ,
        { "TEXCOORD" , 0 , DXGI_FORMAT_R32G32_FLOAT , 0 , D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 } ,
        { "COLOR" , 0 , DXGI_FORMAT_R32G32B32A32_FLOAT , 0 , D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 }
    };

    hr = dev->CreateInputLayout(
        layout ,
        ARRAYSIZE(layout) ,
        compiledVS->GetBufferPointer() ,
        compiledVS->GetBufferSize() ,
        &mp_inputLayout);

    // ���_�C���v�b�g���C�A�E�g�쐬
    if (FAILED(hr)) {
        MessageBox(nullptr , "input" , "" , MB_OK);
        return false;
    }

    //--------------------------���ύX��--------------------------
    const int iPixSize = 32;//�c���s�N�Z����

    //�e�N�X�`���̍쐬
    ComPtr<ID3D11Texture2D> D3DTexture;
    D3D11_TEXTURE2D_DESC td;
    td.Width = iPixSize;
    td.Height = iPixSize;
    td.MipLevels = 1;
    td.ArraySize = 1;
    td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    td.SampleDesc.Count = 1;
    td.SampleDesc.Quality = 0;
    td.Usage = D3D11_USAGE_DYNAMIC;
    td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    td.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    td.MiscFlags = 0;
    hr = dev->CreateTexture2D(&td , nullptr , &D3DTexture);
    if (FAILED(hr)) {
        MessageBox(nullptr , "CreateTexture2D" , "" , MB_OK);
        return false;
    }

    //�e�N�X�`�������ւ�
    D3D11_MAPPED_SUBRESOURCE msr;
    devcontext->Map(D3DTexture.Get() , 0 , D3D11_MAP_WRITE_DISCARD , 0 , &msr);

    //��������
    std::random_device seed;
    std::mt19937 random(seed());
    std::uniform_int_distribution<> number(0 , 255);//0�`255�܂ł̃����_���Ȑ�

    byte srcData[iPixSize * iPixSize * 4] = { 0 };//�r�b�g�}�b�v�����ŏ�����
    for (int i = 0; i < iPixSize * iPixSize * 4; i += 4) {
        srcData[i] = number(random);//Red
        srcData[i + 1] = number(random);//Green
        srcData[i + 2] = number(random);//Blue
    }
    memcpy(msr.pData , srcData , sizeof(srcData));
    //--------------------------���ύX��--------------------------
    devcontext->Unmap(D3DTexture.Get() , 0);

    //�V�F�[�_���\�[�X�r���[�̍쐬
    D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
    srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srv.Texture2D.MipLevels = 1;
    hr = dev->CreateShaderResourceView(D3DTexture.Get() , &srv , &mp_srv);
    if (FAILED(hr)) {
        MessageBox(nullptr , "CreateShaderResourceView" , "" , MB_OK);
        return false;
    }
    return true;
}

void DX11Polygon::Draw() {

    ID3D11DeviceContext* p_devContext = DX11Graphics::GetInstance().GetDeviceContext();

    // ���_�o�b�t�@��`��Ŏg����悤�ɃZ�b�g����
    UINT stride = sizeof(VerteAx);
    UINT offset = 0;
    p_devContext->IASetVertexBuffers(0 , 1 , mp_vertexBuffer.GetAddressOf() , &stride , &offset);
    // �v���~�e�B�u�E�g�|���W�[���Z�b�g
    p_devContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    //-----------------------------
    // �V�F�[�_�[���Z�b�g
    //-----------------------------
    p_devContext->VSSetShader(mp_vertexShader.Get() , 0 , 0);
    p_devContext->PSSetShader(mp_pixelShader.Get() , 0 , 0);
    p_devContext->IASetInputLayout(mp_inputLayout.Get());
    p_devContext->PSSetShaderResources(0 , 1 , mp_srv.GetAddressOf());

    // ����Ȋ����ŁA�Ђ�����f�o�C�X�R���e�L�X�g�ɏ���n��

    //-----------------------------
    // �`����s
    //-----------------------------
    // �f�o�C�X�R���e�L�X�g����A��L�̃Z�b�g�������e�ŕ`�悵�Ă��������A�Ƃ��肢����
    p_devContext->Draw(4 , 0);

}
