#include "Model.h"
#include "WorldTransform.h"
#pragma once
/// <summary>
/// 敵キャラの弾
/// </summary>
class EnemyBullet {
  private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 速度
	Vector3 velocity_;

	// 寿命<frm>
	static const int32_t kLifeTime = 60 * 5;

	// デスタイマー
	int32_t dethTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;

  public:
	bool IsDead() const { return isDead_; }
	float r = 2;

  public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, const Vector3& position, const Vector3 velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 衝突を検知したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision();

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	Vector3 GetWorldPosition();
};
