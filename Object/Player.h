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
/// ���L����
/// </summary>
class Player {
  private:
	//	���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	// input
	Input* input_ = nullptr;
	// debugText
	DebugText* debugText_ = nullptr;

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	WorldTransform cameraWorldTransform_;

  public:
	float r = 2;
	
  public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection viewProjection);

	/// <summary>
	/// ���s�ړ�
	/// </summary>
	void Move();
	/// <summary>
	/// ��]�ړ�
	/// </summary>
	void Rotate();

	/// <summary>
	/// �s��̌v�Z
	/// </summary>
	void MatUpdate(WorldTransform& worldTransfom);

	/// <summary>
	/// ���[���h���W���擾
	/// </summary>
	Vector3 GetWorldPosition();

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	/// <summary>
	/// �Փ˂����m������Ăяo�����R�[���o�b�N�֐�
	/// </summary>
	void OnCollision();

	/// <summary>
	/// �e�̎擾
	/// </summary>
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	/// <summary>
	/// �e�I�u�W�F�N�g�̐ݒ�
	/// </summary>
	void SetParent(WorldTransform* worldTransform) { worldTransform_.parent_ = worldTransform; };
};
