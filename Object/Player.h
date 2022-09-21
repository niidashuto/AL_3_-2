#pragma once
#include "Affin.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
#include <list>
#include <memory>

#include "PlayerBullet.h"

/// <summary>
/// 自キャラ
/// </summary>
class Player {
  private:
	//	ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// input
	Input* input_ = nullptr;
	// debugText
	DebugText* debugText_ = nullptr;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	WorldTransform cameraWorldTransform_;

  public:
	float r = 2;
	
  public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection);

	/// <summary>
	/// 平行移動
	/// </summary>
	void Move();
	/// <summary>
	/// 回転移動
	/// </summary>
	void Rotate();

	/// <summary>
	/// 行列の計算
	/// </summary>
	void MatUpdate(WorldTransform& worldTransfom);

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	Vector3 GetWorldPosition();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 衝突を検知したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision();

	/// <summary>
	/// 弾の取得
	/// </summary>
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	/// <summary>
	/// 親オブジェクトの設定
	/// </summary>
	void SetParent(WorldTransform* worldTransform) { worldTransform_.parent_ = worldTransform; };
};
