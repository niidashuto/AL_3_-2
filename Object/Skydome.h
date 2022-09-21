#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "TextureManager.h"


class Skydome {
  private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//テクスチャハンドル
	//uint32_t textureHandle_ = 0u;

	//モデル
	Model* model_ = nullptr;
	const float size = 100.0f;
	Vector3 scale = {size,size,size};
	Vector3 trans = {0.0f, 0.0f, 0.0f};
  public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection);
};
