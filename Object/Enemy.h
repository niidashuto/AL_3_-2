#pragma once
#include "Affin.h"
#include "DebugText.h"
#include "EnemyBullet.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
#include <list>
#include <memory>

// 自機クラスの前方宣言
class Player;
class GameScene;

enum class Phase {
	None,     // 停止
	Approach, // 接近
	Leave,    // 離脱
};

class Enemy {
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

	// 自キャラ
	Player* player_ = nullptr;

	GameScene* gameScene_ = nullptr;

	//デスフラグ
	bool isDead_ = false;

	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	Phase phase_ = Phase::None;
	// 発射タイマー
	int32_t fireTimer_ = 0;

  public:
	// 発射間隔
	static const int kFireInterval = 60;
	// 弾リストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets_() { return bullets_; }
	float r = 2;

  public:
	void Initialize(Model* model,Vector3 trans);
	void Update();
	void Draw(ViewProjection viewProjection);
	void SetPlayer(Player* player);

	/// <summary>
	/// 行列の計算
	/// </summary>
	void MatUpdate(WorldTransform& worldTransfom);

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	/// <summary>
	/// 接近フェーズ初期化
	/// </summary>
	void ApproachInitialize();

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	Vector3 GetWorldPosition();

	/// <summary>
	/// 衝突を検知したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision();

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	bool IsDead() const { return isDead_; }
};
