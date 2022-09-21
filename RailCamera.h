#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Affin.h"
#include "Input.h"

/// <summary>
/// レールカメラ
/// </summary>
class RailCamera {
  private:
	  //ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	DebugText* debugText_= nullptr;
	Input* input_ = nullptr;

  public:
	/// <summary>
	/// 描画
	/// </summary>
	void Initialize(Vector3 wTrans , Vector3 rad);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 回転
	/// </summary>
	void Rotate();

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	Vector3 GetWorldPosition();

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	WorldTransform* GetWorldTransform() { return &worldTransform_; }

	/// <returns>ビュープロジェクション</returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }
};
