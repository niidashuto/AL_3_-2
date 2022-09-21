#include "RailCamera.h"
#include <cassert>

/// <summary>
/// ������
/// </summary>
void RailCamera::Initialize(Vector3 wTrans, Vector3 rad) {
	//���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();
	worldTransform_.translation_ = wTrans;
	worldTransform_.rotation_ = rad;
	

	//viewProjection_.farZ = 1000;
	//�r���[�v���W�F�N�V�����̏�����
	viewProjection_.Initialize();

	// �V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
}

/// <summary>
/// �X�V
/// </summary>
void RailCamera::Update() {

	Move();
	Rotate();
	worldTransform_.matWorld_ = Affin::matWorld(
	  worldTransform_.translation_, worldTransform_.rotation_, worldTransform_.scale_);

	viewProjection_.eye = worldTransform_.translation_;
	// ���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);
	forward =Affin::VecMat(forward, worldTransform_.matWorld_);
	// ���_����O���ɓK���ȋ����i�񂾈ʒu�������_
	forward += viewProjection_.eye;
	viewProjection_.target = forward;
	// ���[���h����x�N�g��
	Vector3 up(0, 1, 0);
	// ���[���J�����̉�]�𔽉f�i���[���J�����̏���x�N�g���j
	viewProjection_.up = Affin::VecMat(up, worldTransform_.matWorld_);
	// �r���[�v���W�F�N�V�����̍X�V
	viewProjection_.UpdateMatrix();
	
}

/// <summary>
/// �ړ�
/// </summary>
void RailCamera::Move() {
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = {0, 0, 0};

	// ���s�ړ�
	{ // X����
		if (input_->PushKey(DIK_RIGHT)) {
			move.z = 0.1f;
		} else if (input_->PushKey(DIK_LEFT)) {
			move.z = -0.1f;
		}
		// Y����
		if (input_->PushKey(DIK_UP)) {
			move.y = 0.1f;
		} else if (input_->PushKey(DIK_DOWN)) {
			move.y = -0.1f;
		}
	}

	worldTransform_.translation_ += move;
}

/// <summary>
/// ��]
/// </summary>
void RailCamera::Rotate() {
	Vector3 rot = {0, 0, 0};
	if (input_->PushKey(DIK_8)) {
		rot.y = 0.1f;
	}
	if (input_->PushKey(DIK_9)) {
		rot.y = -0.1f;
	}
	worldTransform_.rotation_.y += rot.y;
}

/// <summary>
/// ���[���h���W���擾
/// </summary>
Vector3 RailCamera::GetWorldPosition() {
	//
	Vector3 worldPos;
	//
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}