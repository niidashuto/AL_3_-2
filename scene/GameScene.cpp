#include "GameScene.h"
#include "AxisIndicator.h"
#include "Enemy.h"
#include "PLayer.h"
#include "TextureManager.h"
#include <cassert>
#include <cmath>
#include <fstream>
#include <random>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete debugCamera_;
	//自キャラの解放
	delete player_;
	delete model_;
	delete modelSkydome_;
}

void GameScene::Initialize(GameScene* gameScene) {
	gameScene_ = gameScene;
	debugCamera_ = new DebugCamera(1280, 720);
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//自キャラの生成
	skydome_ = std::make_unique<Skydome>();
	railCamera_ = std::make_unique<RailCamera>();
	player_ = new Player();
	model_ = Model::Create();
	//自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	railCamera_->Initialize(Vector3{0.0f, 0.0f, -50.0f}, Vector3{0.0f, 0.0f, 0.0f});

	player_->SetParent(railCamera_->GetWorldTransform());

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	skydome_->Initialize(modelSkydome_);

	LoadEnemyPopData();
}

void GameScene::Update() {

#ifdef _DEBUG
	  if (input_->TriggerKey(DIK_P) && isDebugcameraActive_ == false) {
		isDebugcameraActive_ = true;
	}
	else if (input_->TriggerKey(DIK_P) && isDebugcameraActive_ == true) {
		isDebugcameraActive_ = false;
	}

#endif // _DEBUG

	// カメラの処理
	if (isDebugcameraActive_ == true) {
		//デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
		viewProjection_.TransferMatrix();
	}


	//デスフラグの立った弾を削除
	enemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });
	//デスフラグの立った弾を削除
	enemy_.remove_if([](std::unique_ptr<Enemy>& enemy) { return enemy->IsDead(); });
	//自キャラの更新
	player_->Update();

	UpdateEnemyPopCommands();

	//弾更新
	for (std::unique_ptr<Enemy>& enemy : enemy_) {
		//敵キャラの更新
		enemy->Update();
	}
	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		bullet->Update();
	}

	//衝突判定
	CheckAllCollisions();

	railCamera_->Update();

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	// 3Dモデル描画
	skydome_->Draw(railCamera_->GetViewProjection());
	//自キャラの描画
	player_->Draw(railCamera_->GetViewProjection());
	//弾描画
	for (std::unique_ptr<Enemy>& enemy : enemy_) {
		enemy->Draw(railCamera_->GetViewProjection());
	}
	//弾描画
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		bullet->Draw(railCamera_->GetViewProjection());
	}
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
void GameScene::CheckAllCollisions() {
	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	//自キャラの座標
	posA = player_->GetWorldPosition();

	//自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		//敵弾の座標
		posB = bullet->GetWorldPosition();

		const float AR = 1;
		const float BR = 1;

		float A = pow((posB.x - posA.x), 2) + pow((posB.y - posA.y), 2) + pow((posB.z - posA.z), 2);
		float B = pow((AR + BR), 2);

		if (A <= B) {
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	//自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<Enemy>& enemy : enemy_) {
		posA = enemy->GetWorldPosition();
		//自弾と敵キャラ全ての当たり判定
		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
			//自弾の座標
			posB = bullet->GetWorldPosition();

			const float AR = 1;
			const float BR = 1;

			float A =
			  pow((posB.x - posA.x), 2) + pow((posB.y - posA.y), 2) + pow((posB.z - posA.z), 2);
			float B = pow((AR + BR), 2);

			if (A <= B) {
				//自キャラの衝突時コールバックを呼び出す
				enemy->OnCollision();
				//敵弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
			}
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	//自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<EnemyBullet>& enemybullet : enemyBullets_) {
		//自弾と敵キャラ全ての当たり判定
		for (const std::unique_ptr<PlayerBullet>& playerbullet : playerBullets) {
			//自弾の座標
			posA = playerbullet->GetWorldPosition();
			//自弾の座標
			posB = enemybullet->GetWorldPosition();

			const float AR = 1;
			const float BR = 1;

			float A =
			  pow((posB.x - posA.x), 2) + pow((posB.y - posA.y), 2) + pow((posB.z - posA.z), 2);
			float B = pow((AR + BR), 2);

			if (A <= B) {
				//自キャラの衝突時コールバックを呼び出す
				enemybullet->OnCollision();
				//敵弾の衝突時コールバックを呼び出す
				playerbullet->OnCollision();
			}
		}
	}
#pragma endregion
}

void GameScene::AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet) {
	enemyBullets_.push_back(std::move(enemyBullet));
}
void GameScene::Fire(Vector3 trans) {
	assert(player_);

	std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
	enemy->Initialize(model_, trans);
	enemy->SetPlayer(player_);
	enemy->SetGameScene(gameScene_);
	enemy_.push_back(std::move(enemy));
}
void GameScene::LoadEnemyPopData() {
	//ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();
}
void GameScene::UpdateEnemyPopCommands() {

	//待機処理
	if (enemyPop) {
		enemyPopTime--;
		if (enemyPopTime <= 0) {
			//待機完了
			enemyPop = false;
		}
		return;
	}

	// 1行分の文字列を入れる
	std::string line;

	//コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');
		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			//コメント行を飛ばす
			continue;
		}
		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			//敵を発生させる
			Fire(Vector3(x, y, z));
		} else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機開始
			enemyPop = true;

			enemyPopTime = waitTime;
			//コマンドループを抜ける
			break;
		}
	}
}