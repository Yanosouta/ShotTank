#include "manager.h"
#include "sprite.h"
#include "CreateTexture.h"


void Sprite::Init(int x, int y, int Width, int Height, const char* TextureName)
{

	VERTEX_3D vertex[4];

	vertex[0].Position = DirectX::XMFLOAT3((float)x, (float)y, 0.0f);
	vertex[0].Normal = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = DirectX::XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = DirectX::XMFLOAT3((float)(x + Width), (float)y, 0.0f);
	vertex[1].Normal = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = DirectX::XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = DirectX::XMFLOAT3((float)x, (float)(y + Height), 0.0f);
	vertex[2].Normal = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = DirectX::XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = DirectX::XMFLOAT3((float)(x + Width), (float)(y + Height), 0.0f);
	vertex[3].Normal = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = DirectX::XMFLOAT2(1.0f, 1.0f);


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	// �e�N�X�`���ǂݍ���
	//CreateTextureFromFile(Renderer::GetDevice(), TextureName, &m_Texture);
	//assert(m_Texture);
	//m_Material.Diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//m_Material.TextureEnable = TRUE;
	if (TextureName)
	{
		CreateTextureFromFile(Renderer::GetDevice(), TextureName, &m_Texture);
		assert(m_Texture);
		m_Material.TextureEnable = TRUE;
	}
	else
	{
		m_Texture = nullptr;
		m_Material.TextureEnable = FALSE;
	}
	m_Material.Diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

}



void Sprite::Uninit()
{
	m_VertexBuffer->Release();
	if (m_Texture)
	{
		m_Texture->Release();
	}
}



void Sprite::Draw()
{
	// �}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	Renderer::SetMaterial(m_Material);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);

}


