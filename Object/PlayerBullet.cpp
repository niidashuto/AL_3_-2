#include "PlayerBullet.h"
#include <cassert>
#include "Affin.h"


/// <summary>
/// 初期化
/// </summary>
void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3 velocity) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("ddddog.png");

	// ワールドトランスフォーム
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	worldTransform_.TransferMatrix();

	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
}

/// <summary>
/// 更新
/// </summary>
void PlayerBullet::Update() {

	// 座標を移動させる (1フレーム分の移動量を足しこむ)
	worldTransform_.translation_ += velocity_;

	// 行列の更新
	worldTransform_.matWorld_ = Affin::matWorld(
	  worldTransform_.translation_, worldTransform_.rotation_, worldTransform_.scale_);
	worldTransform_.TransferMatrix();

	// 時間経過でデス
	if (--dethTimer_ <= 0) {
		isDead_ = true;
	}
}

/// <summary>
/// 描画
/// </summary>
void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

/// <summary>
/// 衝突を検知したら呼び出されるコールバック関数
/// </summary>
void PlayerBullet::OnCollision() {
	// デスデスデース	
		isDead_ = true;	
}

/// <summary>
/// ワールド座標を取得
/// </summary>
Vector3 PlayerBullet::GetWorldPosition() {
	//
	Vector3 worldPos;
	//
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}