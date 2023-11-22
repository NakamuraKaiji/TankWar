#include "pch.h"
#include "Graphics.h"

std::unique_ptr<Graphics> Graphics::m_graphics = nullptr;

// DirectX Graphics�N���X�̃C���X�^���X���擾
Graphics* const Graphics::GetInstance()
{
	if (m_graphics == nullptr)
	{
		// DirectX Graphics�N���X�̃C���X�^���X�𐶐�
		m_graphics.reset(new Graphics());
	}
	// DirectX Graphics�N���X�̃C���X�^���X��Ԃ�
	return m_graphics.get();
}

// �R���X�g���N�^
Graphics::Graphics()
	:
	m_deviceResources(nullptr),			                // �f�o�C�X���\�[�X
	m_commonStates(nullptr),				            // �R�����X�e�[�g
	m_spriteBatch(nullptr),					            // �X�v���C�g�o�b�`
	m_spriteFont(nullptr),						        // �X�v���C�g�t�H���g
	m_basicEffect(nullptr),					            // �x�[�V�b�N�G�t�F�N�g
	m_primitiveBatch(nullptr),				            // �v���~�e�B�u�o�b�`
	m_rasterrizerState(nullptr),				        // ���X�^���C�U�[�X�e�[�g
	m_effectFactory(nullptr),				            // �G�t�F�N�g�t�@�N�g��
	m_inputLayout(nullptr),					            // ���̓��C�A�E�g
	m_screenW(0),								        // �X�N���[����
	m_screenH(0),									    // �X�N���[����
	m_view{},											// �r���[�s��
	m_projection{},								        // �ˉe�s��
	m_device(nullptr),							        // �f�o�C�X
	m_context(nullptr)							        // �f�o�C�X�R���e�L�X�g
{
	// DeviceResources�N���X�̃C���X�^���X�𐶐�
	//m_deviceResources = std::make_unique<DX::DeviceResources>();
}

// �f�X�g���N�^
Graphics::~Graphics()
{
}

// ������
void Graphics::Initialize()
{
	// �X�N���[���T�C�Y��ݒ�
	//SetScreenSize(width, height);
	// �f�o�C�X���\�[�X��ݒ�
	//m_deviceResources = deviceResources;
	// �f�o�C�X���擾
	m_device = m_deviceResources->GetD3DDevice();
	// �f�o�C�X�R���e�L�X�g���擾
	m_context = m_deviceResources->GetD3DDeviceContext();

	// �R�����X�e�[�g�𐶐�
	//m_commonStates = std::make_unique<DirectX::CommonStates>(m_device);
	// �X�v���C�g�o�b�`�𐶐�
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_context);
	// �x�[�V�b�N�G�t�F�N�g�𐶐�
	m_basicEffect = std::make_unique<DirectX::BasicEffect>(m_device);
	// �X�v���C�g�t�H���g�𐶐�
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(m_device, L"resources\\font\\SegoeUI_18.spritefont");
	// �v���~�e�B�u�o�b�`�𐶐�
	m_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(m_context);
	// ���̓��C�A�E�g�𐶐�
	m_basicEffect->SetVertexColorEnabled(true);
	// �e�N�X�`���𖳌�
	m_basicEffect->SetTextureEnabled(false);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	// ���̓��C�A�E�g�𐶐�
	m_device->CreateInputLayout(
		DirectX::VertexPositionColor::InputElements,
		DirectX::VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_inputLayout.ReleaseAndGetAddressOf()
	);
	CD3D11_RASTERIZER_DESC rasterizerStateDesc(
		D3D11_FILL_SOLID, D3D11_CULL_NONE, FALSE,
		D3D11_DEFAULT_DEPTH_BIAS, D3D11_DEFAULT_DEPTH_BIAS_CLAMP,
		D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS, TRUE, FALSE, FALSE, TRUE
	);
	// ���X�^���C�Y�X�e�[�g�𐶐�
	m_device->CreateRasterizerState(&rasterizerStateDesc, m_rasterrizerState.ReleaseAndGetAddressOf());
	// �G�t�F�N�g�t�@�N�g���𐶐�
	m_effectFactory = std::make_unique<DirectX::EffectFactory>(m_device);
	// ���\�[�X�f�B���N�g����ݒ�
	m_effectFactory->SetDirectory(L"resources/cmo");
}

// �������`��
void Graphics::DrawString(const float& x, const float& y, const wchar_t* str)
{
	// �������`��
	m_spriteFont->DrawString(m_spriteBatch.get(), str, DirectX::SimpleMath::Vector2(x, y));
}

// �`��v���~�e�B�u���J�n
void Graphics::DrawPrimitiveBegin(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	m_context->OMSetBlendState(m_commonStates->Opaque(), nullptr, 0xFFFFFFFF);
	m_context->OMSetDepthStencilState(m_commonStates->DepthNone(), 0);
	m_context->RSSetState(m_commonStates->CullNone());
	//m_context->RSSetState(m_rasterrizeState.Get());

	// �r���[�s���ݒ�
	m_basicEffect->SetView(view);
	// �v���W�F�N�V�����s���ݒ�
	m_basicEffect->SetProjection(projection);
	// ���[���h�s���ݒ�
	m_basicEffect->SetWorld(DirectX::SimpleMath::Matrix::Identity);

	// ���_�J���[��L��
	m_basicEffect->SetVertexColorEnabled(true);
	// �e�N�X�`����L��
	m_basicEffect->SetTextureEnabled(false);
	// ���̓��C�A�E�g��ݒ�
	m_basicEffect->Apply(m_context);
	// ���̓��C�A�E�g��ݒ�
	m_context->IASetInputLayout(m_inputLayout.Get());
	// �v���~�e�B�u�o�b�`���J�n
	m_primitiveBatch->Begin();
}

// �`��v���~�e�B�u���I��
void Graphics::DrawPrimitiveEnd()
{
	// �v���~�e�B�u�o�b�`���I��
	m_primitiveBatch->End();
}

// ������`��(XZ����)
void Graphics::DrawLine(const DirectX::SimpleMath::Vector2& position, const DirectX::SimpleMath::Vector2& vector, const DirectX::FXMVECTOR& color)
{
	// ���_�J���[��ݒ�
	DirectX::VertexPositionColor vertex[2] =
	{
		{ DirectX::SimpleMath::Vector3(position.x, 0.0f, position.y), color },
		{ DirectX::SimpleMath::Vector3(position.x + vector.x, 0.0f, position.y + vector.y), color }
	};
	// ������`��
	m_primitiveBatch->DrawLine(vertex[0], vertex[1]);
}

// ������`��
void Graphics::DrawLine(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& vector, const DirectX::FXMVECTOR& color)
{
	// ���_�J���[��ݒ�
	DirectX::VertexPositionColor vertex[2] =
	{
		{ DirectX::SimpleMath::Vector3(position.x, position.y, position.z), color },
		{ DirectX::SimpleMath::Vector3(position.x + vector.x, position.y + vector.y, position.z + vector.z), color }
	};
	// ������`��
	m_primitiveBatch->DrawLine(vertex[0], vertex[1]);
}

// �x�N�g����`��(XZ����)
void Graphics::DrawVector(const DirectX::SimpleMath::Vector2& position, const DirectX::SimpleMath::Vector2& vector, const DirectX::FXMVECTOR& color)
{
	using namespace DirectX::SimpleMath;
	const float cosTheta = cosf(DirectX::XMConvertToRadians(20.0f));
	const float sinTheta = sinf(DirectX::XMConvertToRadians(20.0f));

	// ���̃x�N�g���̃T�C�Y��ݒ�
	Vector2 arrow = -vector;
	// ���K��
	arrow.Normalize();
	// ���̃T�C�Y��ݒ�
	arrow *= 3.0f;
	// �E�� X: (xcos��- ysin��)  Y: (xsin��+ ycos��)
	Vector2 arrowR = Vector2(arrow.x * cosTheta - arrow.y * sinTheta, arrow.x * sinTheta + arrow.y * cosTheta);
	// ���� X: (xcos��- ysin��)  Y: (xsin��+ ycos��)
	Vector2 arrowL = Vector2(arrow.x * cosTheta + arrow.y * sinTheta, -arrow.x * sinTheta + arrow.y * cosTheta);
	// ������`��
	DrawLine(position, vector, color);
	// �E���`��
	DrawLine(position + vector, arrowR, color);
	// �����`��
	DrawLine(position + vector, arrowL, color);
}

// �x�N�g����`��
void Graphics::DrawVector(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& vector, const DirectX::FXMVECTOR& color)
{
	using namespace DirectX::SimpleMath;
	const float cosTheta = cosf(DirectX::XMConvertToRadians(20.0f));
	const float sinTheta = sinf(DirectX::XMConvertToRadians(20.0f));

	// ���̃x�N�g���̃T�C�Y��ݒ�
	Vector3 arrow = -vector;
	// ���K��
	arrow.Normalize();
	// ���̃T�C�Y��ݒ�
	arrow *= 3.0f;
	// �E�� X: (xcos��- zsin��)  Z: (xsin��+ zcos��)
	Vector3 arrowR = Vector3(arrow.x * cosTheta - arrow.z * sinTheta, arrow.y, arrow.x * sinTheta + arrow.z * cosTheta);
	// ���� X: (xcos��- zsin��)  Z: (xsin��+ zcos��)
	Vector3 arrowL = Vector3(arrow.x * cosTheta + arrow.z * sinTheta, arrow.y, -arrow.x * sinTheta + arrow.z * cosTheta);
	// ������`��
	DrawLine(position, vector, color);
	// �E���`��
	DrawLine(position + vector, arrowR, color);
	// �����`��
	DrawLine(position + vector, arrowL, color);
}

// �~��`��(XZ����)
void Graphics::DrawCircle(const DirectX::SimpleMath::Vector2& center, const float& radius, const DirectX::FXMVECTOR& color, const int& split)
{
	using namespace DirectX::SimpleMath;

	// �p�x��������
	float angle = 0.0f;
	// �n�_��錾
	Vector2 position1 = center + Vector2(cosf(angle), sinf(angle)) * radius;
	for (int index = 0; index < split; index++)
	{
		// �n�_��ݒ�
		Vector2 position0 = position1;
		// �p�x���v�Z
		angle += DirectX::XM_2PI / (float)split;
		// �I�_���v�Z
		position1 = center + Vector2(cosf(angle), sinf(angle)) * radius;
		// �~��`��
		DrawLine(position0, position1 - position0, color);
	}
}

// �~��`��
void Graphics::DrawCircle(const DirectX::SimpleMath::Vector3& center, const float& radius, const DirectX::FXMVECTOR& color, const int& split)
{
	using namespace DirectX::SimpleMath;

	// �p�x��������
	float angle = 0.0f;
	// �I�_���v�Z
	Vector3 position1 = center + Vector3(cosf(angle), center.y, sinf(angle)) * radius;
	for (int index = 0; index < split; index++)
	{
		// �n�_��ݒ�
		Vector3 position0 = position1;
		// �p�x���v�Z
		angle += DirectX::XM_2PI / (float)split;
		// �I�_���v�Z
		position1 = center + Vector3(cosf(angle), center.y, sinf(angle)) * radius;
		// �~��`��
		DrawLine(position0, position1 - position0, color);
	}
}

// ��`��`��
void Graphics::DrawQuad(const DirectX::VertexPositionColor& v1, const DirectX::VertexPositionColor& v2, const DirectX::VertexPositionColor& v3, const DirectX::VertexPositionColor& v4)
{
	// ��`��`��
	m_primitiveBatch->DrawQuad(v1, v2, v3, v4);
}

// ��`��`��
void Graphics::DrawQuad(
	const DirectX::FXMVECTOR& point1,
	const DirectX::FXMVECTOR& point2,
	const DirectX::FXMVECTOR& point3,
	const DirectX::FXMVECTOR& point4,
	const DirectX::HXMVECTOR& color
)
{
	DirectX::VertexPositionColor vertexs[5];
	DirectX::XMStoreFloat3(&vertexs[0].position, point1);
	DirectX::XMStoreFloat3(&vertexs[1].position, point2);
	DirectX::XMStoreFloat3(&vertexs[2].position, point3);
	DirectX::XMStoreFloat3(&vertexs[3].position, point4);
	DirectX::XMStoreFloat3(&vertexs[4].position, point1);

	XMStoreFloat4(&vertexs[0].color, color);
	XMStoreFloat4(&vertexs[1].color, color);
	XMStoreFloat4(&vertexs[2].color, color);
	XMStoreFloat4(&vertexs[3].color, color);
	XMStoreFloat4(&vertexs[4].color, color);

	m_primitiveBatch->Draw(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, vertexs, 5);
}

// ���f����`��
void Graphics::DrawModel(const DirectX::Model* model, const DirectX::SimpleMath::Matrix& world, const bool& depthBuffer)
{
	if (depthBuffer)
	{
		// ���f����`��
		model->Draw(m_context, *m_commonStates, world, m_view, m_projection);
	}
	else
	{
		// ���f����`��
		model->Draw(
			m_context,
			*m_commonStates,
			world,
			m_view,
			m_projection,
			false,
			[&]() { 	m_context->OMSetDepthStencilState(m_commonStates->DepthNone(), 0); }
		);
	}
}

// �A�j���[�V�������f����`��
//void Graphics::DrawModel(
//	const DirectX::Model* model,
//	const DX::AnimationSDKMESH* animationSDKMESH,
//	const DirectX::ModelBone::TransformArray* transformArray,
//	const DirectX::SimpleMath::Matrix& world
//)
//{
//	// �{�[���z��̃T�C�Y���擾
//	size_t bones = model->bones.size();
//	// �A�j���[�V�����Ƀ��f���A�{�[�����A�{�[����K�p
//	animationSDKMESH->Apply(*model, bones, transformArray->get());
//	// �R�����X�e�[�g���擾
//	DirectX::CommonStates* commonState = Graphics::GetInstance()->GetCommonStates();
//	// �X�L�����b�V���A�j���[�V������`��
//	model->DrawSkinned(m_context, *commonState, bones, transformArray->get(), world, m_view, m_projection);
//}