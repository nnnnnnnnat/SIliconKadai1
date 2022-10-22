//==============================================================================
/// Filename: Game_Camera.h
/// Description: カメラに関するクラス
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================


#pragma once

#include <directXMath.h>

#include "../Utilities.h"

using namespace DirectX;

class GameCamera : NonCopyable {
public:
    //-----------------------------------------------------------------------------
    // public method

    //-----------------------------------------------------------------------------
    /// インスタンス取得
    /// 
    /// \return GameCamera&
    //-----------------------------------------------------------------------------
    static GameCamera& GetInstance();

    //-----------------------------------------------------------------------------
    /// 初期化
    /// 
    /// nearclip [in] 描画されるオブジェクトの最短距離 
    /// farclip [in] 描画されるオブジェクトの最長距離 
    /// fov [in] 視野角 
    /// width [in] スクリーンの横の大きさ 
    /// height [in] スクリーンの縦の大きさ 
    /// eye [in] カメラ座標 
    /// lookat [in] 注視点 
    /// up [in] 上向きベクトル 
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void Init(
        const float nearclip ,
        const float farclip ,
        const float fov ,
        const float width ,
        const float height ,
        const XMFLOAT4& eye ,
        const XMFLOAT4& lookat ,
        const XMFLOAT4& up);

    //-----------------------------------------------------------------------------
    /// 更新処理
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void Update();

    //-----------------------------------------------------------------------------
    /// 描画されるオブジェクトの最短距離 格納
    /// 
    /// _near [in] 格納する値
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void SetNear(
        /*[in]*/ const float _near);

    //-----------------------------------------------------------------------------
    /// 描画されるオブジェクトの最長距離 格納
    /// 
    /// _near [in] 格納する値
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void SetFar(
        /*[in]*/ const float _far);

    //-----------------------------------------------------------------------------
    /// 視野角 格納
    /// 
    /// _fov [in] 格納する値
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void SetFov(
        /*[in]*/ const float _fov);

    //-----------------------------------------------------------------------------
    /// 描画されるオブジェクトの最短距離 格納
    /// 
    /// width [in] スクリーンの横の大きさ
    /// height [in] スクリーンの縦の大きさ
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void SetAspect(
        /*[in]*/ const float width ,
        /*[in]*/ const float height);

    //-----------------------------------------------------------------------------
    /// プロジェクション変換行列に使用する変数をまとめて格納
    /// 
    /// nearclip [in] 描画されるオブジェクトの最短距離
    /// farclip [in] 描画されるオブジェクトの最長距離
    /// fov [in] 視野角
    /// width [in] スクリーンの横の大きさ
    /// height [in] スクリーンの縦の大きさ
    /// 
    /// \return void
    //-----------------------------------------------------------------------------    
    void SetProjection(
        /*[in]*/ const float nearclip ,
        /*[in]*/ const float farclip ,
        /*[in]*/ const float fov ,
        /*[in]*/ const float width ,
        /*[in]*/ const float height);

    //-----------------------------------------------------------------------------
    /// カメラ行列に使用する変数をまとめて格納
    /// 
    /// eye [in] カメラ座標
    /// lookat [in] 注視点
    /// up [in] 上向きベクトル
    /// 
    /// \return void
    //-----------------------------------------------------------------------------    
    void SetCamera(
        /*[in]*/ const XMFLOAT4& eye ,
        /*[in]*/ const XMFLOAT4& lookat ,
        /*[in]*/ const XMFLOAT4& up);

    //-----------------------------------------------------------------------------
    /// カメラ座標 格納
    ///
    /// eye [in] カメラ座標
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void SetEye(
        /*[in]*/ const XMFLOAT4& eye);

    //-----------------------------------------------------------------------------
    /// カメラ座標 格納
    ///
    /// lookat [in] 注視点
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void SetLookat(
        /*[in]*/ const XMFLOAT4& lookat);

    //-----------------------------------------------------------------------------
    /// カメラ座標 格納
    ///
    /// up [in] 上向きベクトル
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void SetUp(
        /*[in]*/ const XMFLOAT4& up);

    //-----------------------------------------------------------------------------
    /// ビュー変換行列 取得
    /// 
    /// \return const XMMATRIX&
    //-----------------------------------------------------------------------------
    const XMMATRIX GetViewMatrix();

    //-----------------------------------------------------------------------------
    /// プロジェクション変換行列 取得
    /// 
    /// \return const XMMATRIX&
    //-----------------------------------------------------------------------------
    const XMMATRIX GetProjectionMatrix();

    //-----------------------------------------------------------------------------
    /// カメラ座標 取得
    /// 
    /// \return XMFLOAT4
    //-----------------------------------------------------------------------------
    const XMFLOAT4 GetPos();

    //-----------------------------------------------------------------------------
    /// 注視点 取得
    /// 
    /// \return XMFLOAT4
    //-----------------------------------------------------------------------------
    const XMFLOAT4 GetLookAt();

    //-----------------------------------------------------------------------------
    /// カメラの上向きベクトル 取得
    /// 
    /// \return XMFLOAT4
    //-----------------------------------------------------------------------------
    const XMFLOAT4 GetUpVector();

    //-----------------------------------------------------------------------------
    /// 描画されるオブジェクトの最短距離 取得
    /// 
    /// \return float
    //-----------------------------------------------------------------------------
    const float GetNearClip();

    //-----------------------------------------------------------------------------
    /// 描画されるオブジェクトの最長距離 取得
    /// 
    /// \return float
    //-----------------------------------------------------------------------------
    const float GetFarClip();

    //-----------------------------------------------------------------------------
    /// 画面のアスペクト比 取得
    /// 
    /// \return float
    //-----------------------------------------------------------------------------
    const float GetAspect();

    //-----------------------------------------------------------------------------
    /// 視野角 取得
    /// 
    /// \return float
    //-----------------------------------------------------------------------------
    const float GetFov();


    //-----------------------------------------------------------------------------
private:
    //-----------------------------------------------------------------------------
    // private method

    //-----------------------------------------------------------------------------
    /// ビュー変換行列 計算
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void CreateCameraMatrix();

    //-----------------------------------------------------------------------------
    /// プロジェクション変換行列 計算
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void CreateProjectionMatrix();

    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // private variable

    XMMATRIX m_viewMtx = XMMATRIX();
    XMMATRIX m_projectionMtx = XMMATRIX();

    XMFLOAT4 m_eye = XMFLOAT4();
    XMFLOAT4 m_lookat = XMFLOAT4();
    XMFLOAT4 m_up = XMFLOAT4();

    float m_near = 0;
    float m_far = 0;
    float m_aspect = 0;
    float m_fov = 0;

    ///<
    /// m_viewMtx ビュー変換行列
    /// m_projectionMtx ビュー変換行列
    /// 
    /// m_Eye カメラ座標
    /// m_Lookat 注視点
    /// m_Up 上向きベクトル
    /// 
    /// m_near 描画されるオブジェクトの最短距離
    /// m_far 描画されるオブジェクトの最長距離
    /// m_aspect 画面のアスペクト比
    /// m_fov 視野角
    ///<
    //-----------------------------------------------------------------------------
};
