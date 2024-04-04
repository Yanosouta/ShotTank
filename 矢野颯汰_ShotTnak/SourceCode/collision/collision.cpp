#include "collision.h"

Collision::Vertices Collision::GetVertices()
{
	Vertices vtx = {};
	DirectX::XMFLOAT3 stride[] = {
		DirectX::XMFLOAT3(-1, 1, 1),
		DirectX::XMFLOAT3( 1, 1, 1),
		DirectX::XMFLOAT3(-1, 1,-1),
		DirectX::XMFLOAT3( 1, 1,-1),
		DirectX::XMFLOAT3(-1,-1, 1),
		DirectX::XMFLOAT3( 1,-1, 1),
		DirectX::XMFLOAT3(-1,-1,-1),
		DirectX::XMFLOAT3( 1,-1,-1)
	};

	DirectX::XMFLOAT3 size = m_GameObject->GetScale();
	DirectX::XMFLOAT3 rot = m_GameObject->GetRotation();

	DirectX::XMVECTOR vCenter = DirectX::XMLoadFloat3(&m_GameObject->GetPosition());/*�u���b�N�̒��S���W*/
	DirectX::XMFLOAT3 half = DirectX::XMFLOAT3(size.x , size.y , size.z );
	DirectX::XMMATRIX mRot;/*�u���b�N�̉�]�s��*/
	DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(rot.x);
	DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(rot.y);
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(rot.z);
	mRot = Rx * Ry * Rz;
	for (int i = 0; i < 8; i++)
	{//�e���̒��S���狗�����v�Z
		DirectX::XMVECTOR pos = DirectX::XMVectorSet(half.x * stride[i].x, half.y * stride[i].y, half.z * stride[i].z, 0.0f);
		
		//�u���b�N�̔�������]�ɍ��킹�Ĉړ�������
		pos = DirectX::XMVector3TransformCoord(pos, mRot);
		//�u���b�N����]�����Ă���A���S���W�܂ňړ�������
		DirectX::XMStoreFloat3(&vtx.pos[i], DirectX::XMVectorAdd(pos, vCenter));
	}
	return vtx;
}
