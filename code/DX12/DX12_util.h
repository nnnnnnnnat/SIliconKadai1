//==============================================================================
/// Filename: DX12_util.h
/// Description: DX12用のUtility群
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

#include <d3d12.h>
#include <wrl/client.h>

#include "../../framework.h"

using namespace Microsoft::WRL;

class DX12Util {
public:
    //-----------------------------------------------------------------------------
    // public method

    //-----------------------------------------------------------------------------
    /// リソース状態変更のための関数
    /// 
    /// \param [out] commandList コマンドリスト
    /// \param [in] resource リソース
    /// \param [in] before リソースステート
    /// \param [in] after リソースステート
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    static void SetResourceBarrier(
        /*[out]*/ ID3D12GraphicsCommandList* commandList ,
        /*[in]*/ ID3D12Resource* resource ,
        /*[in]*/ const D3D12_RESOURCE_STATES before ,
        /*[in]*/ const D3D12_RESOURCE_STATES after);

    //-----------------------------------------------------------------------------
    /// パイプラインステート作成
    /// 
    /// \param [in] pRootSignature ルートシグネチャ
    /// \param [in] pBinaryVS 頂点シェーダーのバイナリ
    /// \param [in] vsSize 頂点シェーダーのサイズ
    /// \param [in] pBinaryPS ピクセルシェーダーのバイナリ
    /// \param [in] psSize ピクセルシェーダーのサイズ
    /// \param [in] descInputElements エレメントデスク
    /// \param [in] numInputElements エレメントデスクのサイズ
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    static D3D12_GRAPHICS_PIPELINE_STATE_DESC CreateGraphicsPipelineStateDesc(
        /*[in]*/ ID3D12RootSignature* pRootSignature ,
        /*[in]*/ const void* pBinaryVS ,
        /*[in]*/ const int vsSize ,
        /*[in]*/ const void* pBinaryPS ,
        /*[in]*/ const int psSize ,
        /*[in]*/ const D3D12_INPUT_ELEMENT_DESC* descInputElements ,
        /*[in]*/ const int numInputElements
    );

    //-----------------------------------------------------------------------------
    /// 頂点バッファ設定
    /// 
    /// \param [in] device デバイス
    /// \param [in] bufferSize 頂点バッファのサイズ
    /// \param [in] type ヒープテスト
    /// \param [in] states リソースステート
    /// 
    /// \return ComPtr<ID3D12Resource>
    //-----------------------------------------------------------------------------
    static ComPtr<ID3D12Resource> CreateVertexBuffer(
        /*[in]*/ ID3D12Device* device ,
        /*[in]*/ const int bufferSize ,
        /*[in]*/ const D3D12_HEAP_TYPE type ,
        /*[in]*/ const D3D12_RESOURCE_STATES states
    );

    //-----------------------------------------------------------------------------
    /// インデックスバッファ設定
    /// 
    /// \param [in] device デバイス
    /// \param [in] bufferSize インデックスバッファのサイズ
    /// \param [in] type ヒープテスト
    /// \param [in] states リソースステート
    /// 
    /// \return ComPtr<ID3D12Resource>
    //-----------------------------------------------------------------------------
    static ComPtr<ID3D12Resource> CreateIndexBuffer(
        /*[in]*/ ID3D12Device* device ,
        /*[in]*/ const int bufferSize ,
        /*[in]*/ const D3D12_HEAP_TYPE type ,
        /*[in]*/ const D3D12_RESOURCE_STATES states
    );

    //-----------------------------------------------------------------------------
    /// コンスタントバッファ設定
    /// 
    /// \param [in] device デバイス
    /// \param [in] bufferSize コンスタントバッファのサイズ
    /// 
    /// \return ComPtr<ID3D12Resource>
    //-----------------------------------------------------------------------------
    static ComPtr<ID3D12Resource> CreateConstantBuffer(
        /*[in]*/ ID3D12Device* device ,
        /*[in]*/ const int bufferSize
    );

    //-----------------------------------------------------------------------------
    /// デプスバッファ設定
    /// 
    /// \param [in] device デバイス
    /// \param [in] bufferSize コンスタントバッファのサイズ
    /// 
    /// \return ComPtr<ID3D12Resource>
    //-----------------------------------------------------------------------------
    static ComPtr<ID3D12Resource> CreateDepthBuffer(
        /*[in]*/ ID3D12Device* device ,
        /*[in]*/ const int width ,
        /*[in]*/ const int height);

    //-----------------------------------------------------------------------------
    /// テクスチャ作成
    /// 
    /// \param [in] textureWidth 横のサイズ
    /// \param [in] textureHeight 縦のサイズ
    /// \param [in] pixelSize ピクセルサイズ
    /// 
    /// \return ComPtr<ID3D12Resource>
    //-----------------------------------------------------------------------------
    static const std::vector<UINT8> CreateTexture(
        /*[in]*/ const UINT textureWidth ,
        /*[in]*/ const UINT textureHeight ,
        /*[in]*/ const UINT pixelSize);

    //-----------------------------------------------------------------------------
};
