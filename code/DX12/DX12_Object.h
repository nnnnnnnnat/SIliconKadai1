//==============================================================================
/// Filename: DX12_Cube.h
/// Description: DX12用 キューブ作成クラス
/// Copyright (C) Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

#include <DirectXMath.h>
#include <d3dcompiler.h>

#include "DX12_Graphics.h"

#include "../Game/Game_Camera.h"
#include "../Game/Game_Object.h"

template<typename T>
struct ConstantBufferView {
    D3D12_CONSTANT_BUFFER_VIEW_DESC Desc;
    D3D12_CPU_DESCRIPTOR_HANDLE HandleCPU;
    D3D12_GPU_DESCRIPTOR_HANDLE HandleGPU;
    T* pBuffer;

    ///<
    /// Desc        定数バッファの構成設定
    /// HandleCPU   CPUディスクリプタハンドル
    /// HandleGPU   GPUディスクリプタハンドル
    /// pBuffer     バッファ先頭へのポインタ
    ///<
};

struct alignas( 256 ) Transform {
    DirectX::XMMATRIX World;
    DirectX::XMMATRIX View;
    DirectX::XMMATRIX Proj;

    ///<
    /// World   ワールド行列
    /// View    ビュー行列
    /// Proj    プロジェクション行列
    ///<
};

struct Texture {
    ID3D12Resource* pResource;
    D3D12_CPU_DESCRIPTOR_HANDLE HandleCPU;
    D3D12_GPU_DESCRIPTOR_HANDLE HandleGPU;

    ///<
    /// pResource   リソース
    /// HandleCPU   CPUディスクリプタハンドル
    /// HandleGPU   GPUディスクリプタハンドル
    ///<
};

class DX12Object : public GameObject {
public:
    //-----------------------------------------------------------------------------
    // public method

    //-----------------------------------------------------------------------------
    /// 初期化
    /// 
    /// \param  [in]    _pDevice    表示するAPIのポインタ
    /// 
    /// \return bool
    //-----------------------------------------------------------------------------
    bool Init(
        /*[in]*/ GameDevice* _pDevice);

    //-----------------------------------------------------------------------------
    /// 更新処理
    /// 
    /// \param  [in]    _pDevice    表示するAPIのポインタ
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void Update(
        /*[in]*/ GameDevice* _pDevice);

    //-----------------------------------------------------------------------------
    /// 描画
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void Draw();

    //-----------------------------------------------------------------------------
    /// 解放処理
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void Release();

    //-----------------------------------------------------------------------------
private:
    //-----------------------------------------------------------------------------
    // private varable

    ID3D12DescriptorHeap* m_pHeapCBV;
    ID3D12Resource* m_pIndexBuffer;
    ID3D12Resource* m_pVertexBuffer;
    ID3D12Resource* m_pConstantBuffer[FRAME_COUNT];
    ID3D12RootSignature* m_pRootSignature;
    ID3D12PipelineState* m_pPipelineState;

    ID3D12Device* m_pDevice = nullptr;
    ID3D12GraphicsCommandList* m_pCommandList = nullptr;

    uint32_t m_frameIndex;

    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
    D3D12_VIEWPORT m_viewport;
    D3D12_RECT m_scissor;
    ConstantBufferView<Transform> m_constantBufferView[FRAME_COUNT];
    Texture m_texture;

    ///<
    /// m_pHeapCBV              ディスクリプタヒープ
    /// m_pIndexBuffer          インデックスバッファ
    /// m_pVertexBuffer         頂点バッファ
    /// m_pConstantBuffer       定数バッファ
    /// m_pRootSignature        ルートシグネイチャ
    /// m_pPipelineState        パイプラインステート
    /// 
    /// m_pDevice               DirectX12のデバイスのポインタ
    /// m_pCommandList          DirectX12のコマンドリスト
    /// 
    /// m_frameIndex            フレーム番号
    /// 
    /// m_indexBufferView       インデックスバッファビュー
    /// m_vertexBufferView      頂点バッファビュー
    /// m_viewport              ビューポート
    /// m_scissor               シザー矩形
    /// m_constantBufferView    定数バッファビュー
    /// m_texture               テクスチャデータ
    ///<

    //-----------------------------------------------------------------------------

};