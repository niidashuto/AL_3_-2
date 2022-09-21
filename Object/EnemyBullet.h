#include "Model.h"
#include "WorldTransform.h"
#pragma once
/// <summary>
/// �G�L�����̒e
/// </summary>
class EnemyBullet {
  private:
	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	// ���f��
	Model* model_ = nullptr;
	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	// ���x
	Vector3 velocity_;

	// ����<frm>
	static const int32_t kLifeTime = 60 * 5;

	// �f�X�^�C�}�[
	int32_t dethTimer_ = kLifeTime;
	// �f�X�t���O
	bool isDead_ = false;

  public:
	bool IsDead() const { return isDead_; }
	float r = 2;

  public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model, const Vector3& position, const Vector3 velocity);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// �Փ˂����m������Ăяo�����R�[���o�b�N�֐�
	/// </summary>
	void OnCollision();

	/// <summary>
	/// ���[���h���W���擾
	/// </summary>
	Vector3 GetWorldPosition();
};
