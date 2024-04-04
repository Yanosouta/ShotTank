// Tank�I�u�W�F�N�g [tank.h]
#pragma once
#include "gameObject.h"
#include "manager.h"
#include "player.h"
#include "pbullet.h"
#include "tankMove.h"
#include "tankShot.h"
#include "tankAvoid.h"
#include "CreateBall.h"
#include "AStarMove.h"
#include <vector>

class Tank : public GameObject
{
private:
	//=======�\����=========
	//ray�̖߂�l
	struct Result
	{
		bool hit;
		DirectX::XMFLOAT3 point;
	};
	//�����肻���Ȓe�̏��
	struct HitBullet
	{
		bool hit;					//��������������
		DirectX::XMFLOAT3 pos;		//���W
		DirectX::XMFLOAT3 velo;		//���x
		float dot;					//���ό���
	};

	//=======�񋓌^=========
	//State
	enum State
	{
		donot,	//�ҋ@
		move,	//�ړ�
		shot,	//����
		avoid,	//������
	};

	//=======�ϐ���`=======
	//-----XMFLOAT3-----
	//�@��
	DirectX::XMFLOAT3 m_Normal[6] = {		
		{-1.0f,  0.0f,  0.0f},	
		{ 1.0f,  0.0f,  0.0f},	
		{ 0.0f, -1.0f,  0.0f},	
		{ 0.0f,  1.0f,  0.0f},	
		{ 0.0f,  0.0f, -1.0f},	
		{ 0.0f,  0.0f,  1.0f},	
	};  
	//-----float-----
	float m_RotY;						//�p�x
	float m_avertDelay;					//�ēx������悤�ɂȂ�܂ł̎���
	float m_avertCount;					//�J�E���g�p
	
	//-----bool-----
	bool m_excFlg;
	
	//�X�e�[�g
	State m_currentState = State::donot;	//���݂̃X�e�[�g���i�[
	
	//�f�o�b�O�p
	bool DebugFlg = false;
	const char* statenamu[4] = {	//���݂̃X�e�[�g��\���p
	"donot",
	"move",
	"shot",
	"avoid"
	};  
	
private:
	//�����擾											
	Scene* scene = Manager::GetScene();					//�V�[��
	Player* player = scene->GetGameObject<Player>();	//�v���C���[
	//�e�X�e�[�g���擾									
	TankShot* tankShot;									//�e������
	TankAvoid* tankAvoid;								//������
	TankMove* tankMove;									//�ړ�(�����_��)
	AStarMove* astar;									//�ړ�(AStar)

	HitBullet m_hitbullet;	//�錾
	
public:
	//=========�֐���`==========
	void Init() override;		//������
	void Update() override;		//�X�V

	//�X�e�[�g
	void DoNot();		//�ҋ@
	void Move();		//�ړ�
	void Shot();		//�e������
	void Avoid();		//������
	//���ςŊp�x�����߂�
	float face(DirectX::XMFLOAT3);
	//�^���N�ƃv���C���[�̊ԂɃI�u�W�F�N�g�����邩����
	void Vision();
	//�G���e�����F�ł��Ă��邩����
	void Possibility();
	//���ό��ʂ�HitBullet�Ɋi�[
	HitBullet Dot(DirectX::XMFLOAT3 vec,DirectX::XMFLOAT3 pos);
	//�X�e�[�g��ς���
	void ChangeState(State newState) { m_currentState = newState; }
	//���W�A������x�ɕϊ�
	float ToDegrees(float rad) { return rad * (180.0f / 3.1415f);}
	//�x���烉�W�A���ɕϊ�
	float ToRadian(float degrees) {return degrees * (3.1415f / 180.0f); }
	DirectX::XMFLOAT3 GetForward()//�O�����x�N�g���擾
	{
		DirectX::XMFLOAT4X4 rot;
		DirectX::XMStoreFloat4x4(&rot,
			DirectX::XMMatrixRotationRollPitchYaw(
				m_Rotation.x, m_Rotation.y, m_Rotation.z));

		DirectX::XMFLOAT3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}
	//�f�o�b�O�p
	void ChangeColor(PBullet* bullet);
};

