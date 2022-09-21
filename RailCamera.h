#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Affin.h"
#include "Input.h"

/// <summary>
/// ���[���J����
/// </summary>
class RailCamera {
  private:
	  //���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	// �r���[�v���W�F�N�V����
	ViewProjection viewProjection_;

	DebugText* debugText_= nullptr;
	Input* input_ = nullptr;

  public:
	/// <summary>
	/// �`��
	/// </summary>
	void Initialize(Vector3 wTrans , Vector3 rad);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

	/// <summary>
	/// ��]
	/// </summary>
	void Rotate();

	/// <summary>
	/// ���[���h���W���擾
	/// </summary>
	Vector3 GetWorldPosition();

	/// <summary>
	/// ���[���h���W���擾
	/// </summary>
	WorldTransform* GetWorldTransform() { return &worldTransform_; }

	/// <returns>�r���[�v���W�F�N�V����</returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }
};
