#include<cassert>
#include "imgui.h"
#include"imconfig.h"
#include"Action.h"
#include"ModelManager.h"
#include"Mouse.h"
#include"Player.h"
#include"SpriteManager.h"
#include"WinApp.h"
//�e��
const int MaxRemainingBullet = 9;
//�c�̃X�N���[���̔����̃T�C�Y
const float screenhalfheight_ = WinApp::window_height / 2;
//���̃X�N���[���̔����̃T�C�Y
const float screenhalfwidth_ = WinApp::window_width / 2;
//�f�X�g���N�^
Player::~Player()
{
	gun_.reset();
	body_.reset();
	player_ui_.reset();
	shot_se_.reset();
	reload_se_.reset();
}
//����������
void Player::Initalize(Camera* camera)
{
	//HUD�̃T�C�Y
	const XMFLOAT2 SpriteSiz = { 64.0f,64.0f };
	//�F
	const XMFLOAT4 Color{ 1.f,1.f,1.f,1.f };
	//�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(camera);
	//�X�v���C�g�̓ǂݍ���
	sprite_reticle_.reset(Sprite::SpriteCreate(Name::kReticle, reticle_pos2d_, Color, anchorpoint_));

	bullet_ui_ = make_unique<BulletUI>();
	bullet_ui_->Create(remaining_, ui_bulletpos, ui_reloadpos_);

	player_ui_ = make_unique<PlayerUI>();
	player_ui_->Create();
	//�I�u�W�F�N�g�̓ǂݍ���
	gun_ = Object3d::Create(ModelManager::GetInstance()->GetModel(10));
	body_ = Object3d::Create(ModelManager::GetInstance()->GetModel(10));
	body_->SetParent(camera);
	//�p�[�e�B�N���̐���
	part_red_.reset(ParticleManager::Create(camera));
	part_green_.reset(ParticleManager::Create(camera));
	part_smoke_.reset(ParticleManager::Create(camera));
	//���̐���
	shot_se_ = make_unique<Audio>();
	reload_se_ = make_unique<Audio>();
	shot_se_->Initialize();
	reload_se_->Initialize();

};
//�X�e�[�^�X�Z�b�g
void Player::StatusSet(Camera* camera, XMFLOAT3 eyerot)
{
	//�{�̂̃��[���h���W�擾
	body_mat_ = body_->GetMatrix();
	body_worldpos_ = { -10.0f,0.0f,-20.0f };
	body_worldpos_ = XMVector3Transform(body_worldpos_, body_mat_);
	//���e�B�N���̈ʒu����p�x�̎Z�o
	change_rot_ = eyerot;
	reticle_rot_.y = (reticle_pos2d_.x - screenhalfwidth_) / 10 + change_rot_.y;
	//�{�̂̃p�[�c�̃X�e�[�^�X�Z�b�g
	body_->SetRotation(body_rot_);
	body_->SetPosition(body_pos_);
	body_->SetScale(body_scl_);
	body_->SetParent(camera);
	//�e�̃��[���h���W�擾�ƃX�e�[�^�X�Z�b�g
	gun_mat_ = gun_->GetMatrix();
	gun_worldpos_ = XMVector3Transform(gun_pos_, gun_mat_);
	gun_->SetRotation(gun_rot_);
	gun_->SetScale(gun_scl_);
	gun_->SetParent(camera);
	XMMATRIX GunNotParentMatrix = gun_->GetNotParentWorld();
	gun_notparentpos_ = XMVector3Transform(gun_pos_, GunNotParentMatrix);
	gun_->SetPosition(gun_pos_);
	//UI�̃|�W�V�����Z�b�g
	bullet_ui_->Set();
}
//�I�u�W�F�N�g�Ȃǂ̍X�V����
void Player::AllUpdate()
{
	//��
	const XMFLOAT4 ColorRed{ 1.f,0.f,0.f,0.f };
	//��
	const XMFLOAT4 ColorGreen{ 0.f,0.5f,0.f,0.f };
	//�D�F
	const XMFLOAT4 ColorSmoke{ 0.1f,0.1f,0.1f,0.f };
	//�v���C���[�{�̂̍X�V
	body_->Update();
	//�e�̍X�V
	gun_->Update();
	//�Ԃ��p�[�e�B�N���̍X�V����
	part_red_->Update(ColorRed);
	//�΂̃p�[�e�B�N���̍X�V����
	part_green_->Update(ColorGreen);
	//���̍X�V����
	part_smoke_->Update(ColorSmoke);
}
//�X�V����
void Player::Update(Camera* camera, Phase patern, XMFLOAT3 eyerot, int gamestate_, int state_)
{
	//�}�E�X����
	MouseContoroll();
	//�ҋ@��Ԃ̏���
	WaitProcess();
	//���W���]�A�X�P�[���Ȃǂ̃X�e�[�^�X�̃Z�b�g
	StatusSet(camera, eyerot);
	//�S�Ă̍X�V����
	AllUpdate();
	if (gamestate_ == state_) { return; }
	//���C�̏���
	GunShotProcess(patern);
	//UI
	UIMotionProcess();
	//�����[�h�̏���
	ReloadProcess();
}
//�ҋ@��Ԃ̏���
void Player::WaitProcess()
{
	//�X�e�[�^�X���ҋ@��Ԃ̎�
	if (player_state_ == WAIT) {
		//���Z����l
		const int dividevaluY = 10;
		const int dividevaluX = 50;
		//�}�E�X���W����p�x�̎擾
		gun_rot_.y = (reticle_pos2d_.x - screenhalfwidth_) / dividevaluY;
		gun_rot_.x = (reticle_pos2d_.y - screenhalfheight_) / dividevaluX;
		bullet_shotflag_ = false;
	}
}
//�p�[�e�B�N���`��
void Player::ParticleDraw(ID3D12GraphicsCommandList* cmdeList)
{
	//�}�Y���t���b�V���̕`��
	ParticleManager::PreDraw(cmdeList);
	part_smoke_->Draw();
	part_red_->Draw();
	part_green_->Draw();
	ParticleManager::PostDraw();
}
//�X�v���C�g�`��
void Player::SpriteDraw()
{
	if (!mouse_stopflag_) {
		bullet_ui_->Draw();
	}
	sprite_reticle_->Draw();
	player_ui_->Draw(hp_);
}
//�I�u�W�F�N�g�`��
void Player::ObjDraw()
{
	//Hp��0�ȏ�ł����
	if (hp_ >= 0) {
		gun_->Draw();
	}
}

void Player::SoundEffect()
{
	shot_se_->LoadFile("Resources/Sound/SE/shot.wav", 0.3f);
}
//�}�E�X����
void Player::MouseContoroll()
{
	//���Z����l
	const int addvalue = 15;
	//�}�E�X���W�̎擾
	Mouse::GetInstance()->MouseMoveSprite(reticle_pos2d_);
	//�e����������
	if (recoil_gunflag_) {
		reticle_pos2d_.y += addvalue;
		Mouse::GetInstance()->RecoilMouse(reticle_pos2d_);
	}
	else {
		//�擾�������W�����e�B�N���ɃZ�b�g
		sprite_reticle_->SetPosition(reticle_pos2d_);
	}
}

//�e�̔��ˏ���
void Player::GunShotProcess(Phase paterncount)
{
	//���Z����l
	const int addvalue = 1;
	//�e�̔��ˑO
	if (player_state_ == WAIT && remaining_ < MaxRemainingBullet) {
		if (Mouse::GetInstance()->PushClick(0)) {
			remaining_ += addvalue;
			bullet_ui_->Shot(remaining_);
			recoil_gunflag_ = true;
			player_state_ = SHOT;
			//�}�Y���t���b�V��
			ParticleEfect(paterncount);
		}
	}
	//�X�e�[�^�X��SHOT�ɐ؂�ւ������
	if (player_state_ == SHOT) {
		//�e�����˂��ꂽ
		bullet_shotflag_ = true;
		player_state_ = WAIT;
	}
	//���R�C������
	RecoilProcess();
}

void Player::UIMotionProcess()
{
	//UI�̌ۓ�
	player_ui_->HartBeat(hp_);
	//�����[�h
	bullet_ui_->ReloadMotion();
	//�����Ă�����䰂̏���
	bullet_ui_->FallingUI();
}

void Player::RecoilProcess()
{
	//�^�C�������Z����l
	const float addrecovery_ = 0.2f;
	//�����̊p�x
	const int bouncerotation_ = 25;
	//recoverytime�̏���l
	const int timelimit_ = 1;
	//���R�C���t���O��true�̎�
	if (recoil_gunflag_) {
		//�^�C�������Z
		recovery_time_ += addrecovery_;
		//�e�̔����ŏ�Ɍ�����
		gun_rot_.x = -bouncerotation_;
		//�^�C��������l�܂ŗ�����
		if (recovery_time_ >= timelimit_) {
			//���������ɖ߂�
			gun_rot_.x = {};
			//�^�C����������
			recovery_time_ = {};
			//�t���O��false�ɖ߂�
			recoil_gunflag_ = false;
		}
	}
}

//�����[�h����
void Player::ReloadProcess()
{
	//�c�e����
	const int emptyremaining_ = 8;
	//��]�̌��Z����l
	const float subrotation_ = 9.5f;
	//�^�C�}�[�̉��Z����l
	const int addtime_ = 1;
	//�^�C�}�[�����Z���邽�߂̒l
	const int divtime_ = 40;
	int anser_ = 0;
	//�X�e�[�^�X���ҋ@��ԂŁA�c�e�����^���ȊO�̎�
	if (player_state_ == WAIT && remaining_ != 0) {
		//�E�N���b�N������
		if (Mouse::GetInstance()->PushClick(1)) {
			//�X�e�[�^�X��RELOAD�ɕύX
			player_state_ = RELOAD;
			reload_se_->LoadFile("Resources/Sound/SE/reload.wav", 0.3f);
			//�}�E�X�𑀍�o���Ȃ���Ԃ�

			mouse_stopflag_ = true;
		}
	}
	//�X�e�[�^�X��RELOAD�ł͂Ȃ��Ƃ�
	if (player_state_ != RELOAD) { return; }
	//�e����]������
	gun_rot_.x -= subrotation_;
	//�c�e����x��\���ɂ���
	remaining_ = emptyremaining_;
	bullet_ui_->SetRemainig(remaining_);
	//�^�C�������Z����
	reload_time_ += addtime_;
	//�������Ă���^�C����40�ŏ��Z
	anser_ = reload_time_ % divtime_;
	//reloaadtime/40�̗]�肪0�ȊO�̎�
	if (anser_ != 0) { return; }
	//�c�e�}�b�N�X��
	remaining_ = {};
	bullet_ui_->Reload(remaining_);
	//�c�e�����^���ɂȂ�����
	if (remaining_ == 0) {
		//�X�e�[�^�X��ҋ@��Ԃɖ߂�
		player_state_ = WAIT;
		//�^�C����������
		reload_time_ = {};
		//������ēx�\��Ԃɂ���
		mouse_stopflag_ = false;
	}
}


//�}�Y���G�t�F�N�g
void Player::ParticleEfect(Phase paterncount)
{

		for (int i = 0; i < 10; i++) {
			float radX = reticle_rot_.y * XM_PI / 180.f;
			float radY = gun_rot_.x * XM_PI / 180.f;
			float sinradX = sinf(radX);
			float cosradX = cosf(radX);

			float sinradY = sinf(radY);
			float cosradY = cosf(radY);
			//���������Ă���Ƃ�
			if (paterncount == LANDINGPOINT_BACK ||
				paterncount == MOVEDPOINT_A ||
				paterncount == MOVEDPOINT_A_LEFT ||
				paterncount == GOALPOINT_BACK) {
				pos.x = gun_worldpos_.m128_f32[0] + sinradX * 3.5f;
				pos.y = gun_worldpos_.m128_f32[1] - sinradY * 1.5f;
				pos.z = gun_worldpos_.m128_f32[2] + 3.0f;
			}
			//�E�������Ă���Ƃ�
			else if (paterncount == MOVEDPOINT_C ||
				paterncount == MOVEDPOINT_C_OBLIQUE) {
				pos.x = gun_worldpos_.m128_f32[0] + 2.3f;
				pos.y = gun_worldpos_.m128_f32[1] - sinradY * 1.5f;
				pos.z = gun_worldpos_.m128_f32[2] + 2.8f * cosradX;
			}
			//�^�������O�������Ă���Ƃ�
			else if (paterncount == LANDINGPOINT_FRONT ||
				paterncount == GOALPOINT) {
				pos.x = gun_worldpos_.m128_f32[0] + sinradX * 3.5f;
				pos.y = gun_worldpos_.m128_f32[1] - sinradY * 1.5f;
				pos.z = gun_worldpos_.m128_f32[2] - 3.0f;
			}
			//���������Ă���Ƃ�
			else if (paterncount == MOVEDPOINT_B) {
				pos.x = gun_worldpos_.m128_f32[0] - 2.3f;
				pos.y = gun_worldpos_.m128_f32[1] - sinradY * 1.5f;
				pos.z = gun_worldpos_.m128_f32[2] + 2.8f * cosradX;
			}
			//�΂߂������Ă���Ƃ�
			else if (paterncount == MOVEDPOINT_C_FRONT) {
				pos.x = gun_worldpos_.m128_f32[0] + 2.3f * sinradX;
				pos.y = gun_worldpos_.m128_f32[1] - sinradY * 1.5f;
				pos.z = gun_worldpos_.m128_f32[2] + 2.8f * cosradX;
			}

			const float rnd_vel = 0.001f;
			XMFLOAT3 vel{};
			vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

			const float smokernd_vel = 0.05f;
			XMFLOAT3 smokevel{};
			smokevel.x = (float)rand() / RAND_MAX * smokernd_vel - smokernd_vel / 2.0f;
			smokevel.y = (float)rand() / RAND_MAX * smokernd_vel - smokernd_vel / 2.0f;
			smokevel.z = (float)rand() / RAND_MAX * smokernd_vel - smokernd_vel / 2.0f;
			XMFLOAT3 acc{};
			acc.y = 0.0;

			XMFLOAT3 Smokeacc{};
			Smokeacc.y += 0.005f;
			part_red_->Add(20, pos, vel, acc, 0.7f, 0.f, 1.f);
			part_green_->Add(20, pos, vel, acc, 0.5f, 0.f, 1.f);
			part_smoke_->Add(50, pos, smokevel, acc, 0.5f, 0.f, 1.f);
		}
		SoundEffect();
}

void Player::SlowlyLargeHUD()
{
	//Reload�̕��������X�ɑ傫������
	Action::GetInstance()->EaseOut(reload_spritesize_.x, 260);
	Action::GetInstance()->EaseOut(reload_spritesize_.y, 190);
	if (reload_spritesize_.x >= 250) {
		revers_flag_ = true;
	}
}
void Player::SlowlySmallHUD()
{
	//Reload�̕��������X�ɏ���������
	Action::GetInstance()->EaseOut(reload_spritesize_.x, 200);
	Action::GetInstance()->EaseOut(reload_spritesize_.y, 130);
	if (reload_spritesize_.x <= 210) {
		revers_flag_ = false;
	}
}
//ImgUi�`��
void Player::ImGuiDraw()
{
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.0f, 0.1f, 0.0f));
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin("Plyer");
	if (ImGui::TreeNode("change")) {
		ImGui::SliderFloat("changerot.x", &change_rot_.x, -100.0f, 100.0f);
		ImGui::SliderFloat("changerot.y", &change_rot_.y, -100.0f, 100.0f);
		ImGui::SliderFloat("changerot.z", &change_rot_.z, -100.0f, 100.0f);
		ImGui::TreePop();
	}

	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}