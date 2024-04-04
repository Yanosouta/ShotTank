#include "ray.h"
#include "manager.h"
void Ray::Init()
{
	m_Scene = Manager::GetScene();
	m_Box = m_Scene->GetGameObjects<Box>();
}

void Ray::Update()
{
	for (int i = 0; i < 4; i++)
	{
		//m_BoxPos = m_Box[i]->GetPosition();
		//m_BoxScale = m_Box[i]->GetScale();
	}
}

Ray::Result Ray::CheckRayPlane(DirectX::XMFLOAT3 normal, 
								DirectX::XMFLOAT3 pos,
								DirectX::XMFLOAT3 start,
								DirectX::XMFLOAT3 direction, 
								float length)
{
	Result result = {};
	DirectX::XMVECTOR vRayStart = DirectX::XMLoadFloat3(&start);
	DirectX::XMVECTOR vRayN = DirectX::XMLoadFloat3(&direction);
	DirectX::XMVECTOR vPlaneN = DirectX::XMLoadFloat3(&normal);
	DirectX::XMVECTOR vPlanePos = DirectX::XMLoadFloat3(&pos);

	vRayN = DirectX::XMVector3Normalize(vRayN);
	vPlaneN = DirectX::XMVector3Normalize(vPlaneN);

	//---���ʂ���n�_�܂ł̋���
	float p1;
	DirectX::XMVECTOR vToStart = DirectX::XMVectorSubtract(vRayStart, vPlanePos);	//���ʂ��烌�C�̎n�_�܂ł̃x�N�g��

	DirectX::XMVECTOR vTemp = DirectX::XMVector3Dot(vPlaneN, vToStart);	//�ʂ̖@���Ǝn�_�Ɍ������x�N�g���Ƃ̓��ς̌��ʂ��v�Z
	DirectX::XMStoreFloat(&p1, vTemp);									//�v�Z���ʂ�p1�֊i�[(���̋͂��̔����A�\�ʂ̔䗦�Ɏg�p


	//---�I�_���畽�ʂ܂ł̋���
	float p2;
	DirectX::XMVECTOR vRay = DirectX::XMVectorScale(vRayN, length);

	DirectX::XMVECTOR vToEnd = DirectX::XMVectorAdd(vRay, vToStart);	//���ʂ��烌�C�̏I�_�܂ł̃x�N�g��

	DirectX::XMVECTOR vTemp2 = DirectX::XMVector3Dot(vPlaneN, vToEnd);	//�ʂ̖@���ƏI�_�Ɍ������x�N�g���Ƃ̓��ς̌��ʂ��v�Z
	DirectX::XMStoreFloat(&p2, vTemp2);									//�v�Z���ʂ�p2�֊i�[(���̋͂��̔����A���ʂ̔䗦�Ɏg�p


	//---���C���ʂ��ђʂ��Ă��邩
	if (p1 >= 0.0001f && p2 <= -0.0001f)
	{
		result.hit = true;
		float dist;
		//���C�̎n�_����ђʂ��Ă��邩�ʒu�܂ł̋������v�Z
		//���C�̂Ȃ����ɓ��ςɔ䗦���|����(���C�̒��� * (p1 / (p1 + (-p2)))
		dist = length*(p1 / (p1 + (-p2)));

		//�䗦���|�������̂��A���C�̒P�ʃx�N�g���Ɋ|����
		DirectX::XMVECTOR vTemp = DirectX::XMVectorScale(vRayN, dist);

		//���C�̎n�_�ɁA��d���|�����x�N�g�������Z����
		vTemp = DirectX::XMVectorAdd(vRayStart, vTemp);

		//�v�Z���ʂ�result.point�֊i�[
		DirectX::XMStoreFloat3(&result.point, vTemp);
	}
	return result;
}

Ray::Result Ray::CheckPointTraiangle(DirectX::XMFLOAT3 point, Triangle triangle)
{
	DirectX::XMVECTOR vCross[3];	//�O�ς̌v�Z����
	DirectX::XMVECTOR vPoint = DirectX::XMLoadFloat3(&point);			//����point��ϊ�
	DirectX::XMVECTOR vTriStart = DirectX::XMLoadFloat3(&triangle.p[0]);//�ŏ��ɏ������钸�_��ϊ�(triangle.p[0])

	for (int i = 0; i < 3; ++i)
	{
		DirectX::XMVECTOR vTriEnd = DirectX::XMLoadFloat3(&triangle.p[(i + 1) % 3]);	//�ӂ̏I�_��ϊ�(i=2�̎���0���ΏۂȂ̂Œ���
		DirectX::XMVECTOR vToEdge = DirectX::XMVectorSubtract(vTriEnd, vTriStart);	//�ӂ̃x�N�g��
		DirectX::XMVECTOR vToPoint = DirectX::XMVectorSubtract(vPoint, vTriStart);	//�_�Ɍ������x�N�g��
		vCross[i] = DirectX::XMVector3Cross(vToEdge, vToPoint);						//vToEdge��vToPoint�̊O�ς��v�Z
		vCross[i] = DirectX::XMVector3Normalize(vCross[i]);
		vTriStart = vTriEnd;	//���̏I�_�����̎n�_�ɂȂ�
	}

	DirectX::XMVECTOR vDot[3] =
	{
		DirectX::XMVector3Dot(vCross[0],vCross[1]),
		DirectX::XMVector3Dot(vCross[1],vCross[2]),
		DirectX::XMVector3Dot(vCross[0],vCross[2])
	};
	float match[3];

	//---vDot�̌��ʂ�match�ɕϊ�
	for (int i = 0; i < 3; i++)
		DirectX::XMStoreFloat(&match[i], vDot[i]);

	Ray::Result result = { false,DirectX::XMFLOAT3(0.0f,0.0f,0.0f) };
	if ((match[0] >= 0.90f || match[0] == 0) &&
		(match[1] >= 0.90f || match[1] == 0) &&
		(match[2] >= 0.90f || match[2] == 0))
	{
		result.hit = true;
		result.point = point;
	}

	return result;

}
