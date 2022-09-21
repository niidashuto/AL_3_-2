#include "Affin.h"
#include "math.h"

/// <summary>
/// �P�ʍs��ݒ�
/// </summary>
Matrix4 Affin::matUnit() {
	Matrix4 matUnit;

	matUnit = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	           0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

	return matUnit;
}

/// <summary>
/// ���s�ړ�
/// </summary>
Matrix4 Affin::matTrans(float moveX, float moveY, float moveZ) {
	Matrix4 matTrans;

	matTrans = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
	            0.0f, 0.0f, 1.0f, 0.0f, moveX, moveY, moveZ, 1.0f};

	return matTrans;
}

/// <summary>
/// ���s�ړ�
/// </summary>
Matrix4 Affin::matTrans(Vector3 move) {
	Matrix4 matTrans;

	matTrans = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f,   1.0f,   0.0f,   0.0f,
	            0.0f, 0.0f, 1.0f, 0.0f, move.x, move.y, move.z, 1.0f};

	return matTrans;
}

/// <summary>
/// �x���@ -> ���W�A��
/// </summary>
float Affin::radConvert(float value) {
	float radValue;

	radValue = value * (PI / 180);

	return radValue;
}

/// <summary>
/// ���W�A�� -> �x���@
/// </summary>
float Affin::degConvert(float value) {
	float degValue;

	degValue = value * (180 / PI);

	return degValue;
}

/// <summary>
/// ��] X����
/// </summary>
Matrix4 Affin::matRotateX(float rotationX) {
	float rotRad = radConvert(rotationX);
	float cosRad = cos(rotRad);
	float sinRad = sin(rotRad);
	Matrix4 matRotX;

	matRotX = {1.0f, 0.0f,    0.0f,   0.0f, 0.0f, cosRad, sinRad, 0.0f,
	           0.0f, -sinRad, cosRad, 0.0f, 0.0f, 0.0f,   0.0f,   1.0f};

	return matRotX;
}

/// <summary>
/// ��] Y����
/// </summary>
Matrix4 Affin::matRotateY(float rotationY) {
	float rotRad = radConvert(rotationY);
	float cosRad = cos(rotRad);
	float sinRad = sin(rotRad);
	Matrix4 matRotY;

	matRotY = {cosRad, 0.0f, -sinRad, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	           sinRad, 0.0f, cosRad,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

	return matRotY;
}

/// <summary>
/// ��] Z����
/// </summary>
Matrix4 Affin::matRotateZ(float rotationZ) {
	float rotRad = radConvert(rotationZ);
	float cosRad = cos(rotRad);
	float sinRad = sin(rotRad);
	Matrix4 matRotZ;

	matRotZ = {cosRad, sinRad, 0.0f, 0.0f, -sinRad, cosRad, 0.0f, 0.0f,
	           0.0f,   0.0f,   1.0f, 0.0f, 0.0f,    0.0f,   0.0f, 1.0f};

	return matRotZ;
}

/// <summary>
/// ��] Z->X->Y
/// </summary>
Matrix4 Affin::matRotation(float rotateX, float rotateY, float rotateZ) {

	Matrix4 x = matRotateX(rotateX);
	Matrix4 y = matRotateY(rotateY);
	Matrix4 z = matRotateZ(rotateZ);
	Matrix4 matRot;

	x *= z;
	y *= x;
	matRot = y;

	return matRot;
}

/// <summary>
/// ��] Z->X->Y
/// </summary>
Matrix4 Affin::matRotation(Vector3 rotate) {

	Matrix4 x = matRotateX(rotate.x);
	Matrix4 y = matRotateY(rotate.y);
	Matrix4 z = matRotateZ(rotate.z);
	Matrix4 matRot;

	x *= z;
	y *= x;
	matRot = y;

	return matRot;
}

/// <summary>
/// �g�k
/// </summary>
Matrix4 Affin::matScale(float scaleX, float scaleY, float scaleZ) {

	Matrix4 matScale = {scaleX, 0.0f, 0.0f,   0.0f, 0.0f, scaleY, 0.0f, 0.0f,
	                    0.0f,   0.0f, scaleZ, 0.0f, 0.0f, 0.0f,   0.0f, 1.0f};

	return matScale;
}

/// <summary>
/// �g�k
/// </summary>
Matrix4 Affin::matScale(Vector3 scale) {

	Matrix4 matScale = {scale.x, 0.0f, 0.0f,    0.0f, 0.0f, scale.y, 0.0f, 0.0f,
	                    0.0f,    0.0f, scale.z, 0.0f, 0.0f, 0.0f,    0.0f, 1.0f};

	return matScale;
}

/// <summary>
/// �g�k*��]*�ړ�
/// </summary>
Matrix4 Affin::matWorld(Vector3 move, Vector3 rotate, Vector3 scale) {
	Matrix4 scaleM = matScale(scale);
	Matrix4 rotateM = matRotation(rotate);
	Matrix4 moveM = matTrans(move);
	Matrix4 matWorld;

	rotateM *= scaleM;
	moveM *= rotateM;
	matWorld = moveM;

	return matWorld;
}

/// <summary>
///	�x�N�g���ƍs��̊|���Z
/// </summary>
Vector3 Affin::VecMat(Vector3 vec, Matrix4 mat) {
	
	float x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0] + 0 * mat.m[3][0];
	
	float y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1] + 0 * mat.m[3][1];
	
	float z = vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2] + 0 * mat.m[3][2];
	
	Vector3 re = {x, y, z};
	
	return re;
}