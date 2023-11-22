//*********************************************************************
//			�T�v	�F���\�[�X�N���X
// 
//			�����	�F6��14��
// 
//			�����	�FKaiji Nakamura
//*********************************************************************
#pragma once
#ifndef RESOURCES_DEFINED
#define RESOURCES_DEFINED

class Resources
{
public:
	// �ԑ̃��f�����擾����
	std::shared_ptr<DirectX::Model> GetTankBody() { return m_tankBody; }
	// �C�g���f�����擾����
	std::shared_ptr<DirectX::Model> GetTankTurret() { return m_tankTurret; }
	// �G�̎ԑ̃��f�����擾����
	std::shared_ptr<DirectX::Model> GetEnemyTankBody() { return m_enemyTankBody; }
	// �G�̖C�g���f�����擾����
	std::shared_ptr<DirectX::Model> GetEnemyTankTurret() { return m_enemyTankTurret; }
	// �X�J�C�h�[�����f�����擾����
	std::shared_ptr<DirectX::Model> GetSkydome() { return m_skydome;}
	// �C�e���擾����
	std::shared_ptr<DirectX::Model> GetBullet() { return m_bullet; }

public:
	// Resouces�N���X�̃C���X�^���X���擾����
	static Resources* const GetInstance();
	// ���\�[�X�����[�h����
	void LoadResource();
private:
	Resources() 
		: m_tankBody{}
		, m_tankTurret{}
		, m_enemyTankBody{}
		, m_enemyTankTurret{}
		, m_skydome{}
		, m_bullet{}
	{}
	// ����͋��e���Ȃ�
	void operator=(const Resources& object) = delete;
	// �R�s�[�R���X�g���N�^�͋��e���Ȃ�
	Resources(const Resources& object) = delete;

private:
	// Resources�N���X�̃C���X�^���X�ւ̃|�C���^
	static std::unique_ptr<Resources> m_resources;
	// �O���t�B�b�N�X
	Graphics* m_graphics = Graphics::GetInstance();
	// �f�o�C�X
	ID3D11Device* m_device = m_graphics->GetDeviceResources()->GetD3DDevice();

	// �ԑ̃��f��
	std::shared_ptr <DirectX::Model> m_tankBody;
	// �C�g���f��
	std::shared_ptr<DirectX::Model> m_tankTurret;
	// �G�̎ԑ̃��f��
	std::shared_ptr<DirectX::Model> m_enemyTankBody;
	// �G�̖C�g���f��
	std::shared_ptr<DirectX::Model> m_enemyTankTurret;
	// �X�J�C�h�[�����f��
	std::shared_ptr<DirectX::Model> m_skydome;
	// �C�e���f��
	std::shared_ptr<DirectX::Model> m_bullet;

};

#endif // RESOURCES_DEFINED
