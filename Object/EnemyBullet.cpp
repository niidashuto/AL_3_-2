#include "EnemyBullet.h"
#include "Affin.h"
#include <cassert>

/// <summary>
/// ������
/// </summary>
void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3 velocity) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	// �e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("tama.png");

	// ���[���h�g�����X�t�H�[��
	worldTransform_.Initialize();
	// �����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;

	//worldTransform_.TransferMatrix();

	// �����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;
}

/// <summary>
/// �X�V
/// </summary>
void EnemyBullet::Update() {

	// ���W���ړ������� (1�t���[�����̈ړ��ʂ𑫂�����)
	worldTransform_.translation_ += velocity_;

	// �s��̍X�V
	worldTransform_.matWorld_ = Affin::matWorld(
	  worldTransform_.translation_, worldTransform_.rotation_, worldTransform_.scale_);
	worldTransform_.TransferMatrix();

	// ���Ԍo�߂Ńf�X
	if (--dethTimer_ <= 0) {
		isDead_ = true;
	}
}

/// <summary>
/// �`��
/// </summary>
void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

/// <summary>
/// �Փ˂����m������Ăяo�����R�[���o�b�N�֐�
/// </summary>
void EnemyBullet::OnCollision() {
	// �f�X
	isDead_ = true;
}

/// <summary>
/// ���[���h���W���擾
/// </summary>
Vector3 EnemyBullet::GetWorldPosition() {
	//
	Vector3 worldPos;
	//
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}