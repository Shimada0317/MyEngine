#include "middle.h"
#include"imgui/imgui.h"
#include<fstream>
middle::~middle()
{
	for (int i = 0; i < 9; i++) {
		delete bull[i];
	}
	for (int i = 0; i < 3; i++) {
		rob[i].reset();
	}
}
//
////�Z�[�u
//
void middle::Initialize()
{
	Sprite::LoadTexture(10, L"Resources/reload.png");
	Sprite::LoadTexture(11, L"Resources/wave.png");
	Sprite::LoadTexture(12, L"Resources/slash.png");
	Sprite::LoadTexture(13, L"Resources/one.png");
	Sprite::LoadTexture(14, L"Resources/two.png");
	Sprite::LoadTexture(15, L"Resources/three.png");
	Sprite::LoadTexture(16, L"Resources/four.png");
	Sprite::LoadTexture(17, L"Resources/five.png");
	Sprite::LoadTexture(18, L"Resources/Hpber.png");
	//camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	//Object3d::SetCamera(camera);
	for (int i = 0; i < 9; i++) {
		bull[i] = new Bullet();
		bull[i]->Initialize();
	}
	for (int i = 0; i < 3; i++) {
		rob[i] = std::make_unique<Robot>();
		rob[i]->Initialize();
		//enemyPos[i] = rob[i]->GetPosition();
		//allpos[i] = { 0.0f + i * 1.0f,0.0f,10.0f };
		all[i] = true;
	}
	player = std::make_unique<Player>();
	player->Initalize();


	////�X�v���C�g�̓ǂݍ���
	for (int i = 0; i < 9; i++) {

		Sprite::LoadTexture(i, L"Resources/bullet.png");
		bulletHUD[i] = Sprite::SpriteCreate(i, { 10.0f,10.0f });
	}

	Reload = Sprite::SpriteCreate(10, { 10.0f,10.0f }, { 1.0f,1.0f,1.0f,1.0f });
	wave = Sprite::SpriteCreate(11, { 10.0f,10.0f });
	slash = Sprite::SpriteCreate(12, { 10.0f,10.0f });
	maxcount = Sprite::SpriteCreate(17, { 10.0f,10.0f });
	HpBer = Sprite::SpriteCreate(18, { 10.0f,10.0f });
	for (int i = 0; i < 5; i++) {
		changecount[i] = Sprite::SpriteCreate(13 + i, { 10.0f,10.0f });
	}
	for (int i = 0; i < 5; i++) {
		LifeCount[i] = Sprite::SpriteCreate(13 + i, { 10.0f,10.0f });
	}
	//LoadEnemyPopData();
	//UpdataEnemyPopCommands();
	oldpatern = patern;

	playerMat = player->GetMat();
	playerHp = player->GetHp();

	enemyPos[0].m128_f32[0] = -5;
	enemyPos[1].m128_f32[0] = 0;
	enemyPos[2].m128_f32[0] = 5;

	enemyPos[0].m128_f32[1] = 0;
	enemyPos[1].m128_f32[1] = 0;
	enemyPos[2].m128_f32[1] = 0;

	enemyPos[0].m128_f32[2] = 10;
	enemyPos[1].m128_f32[2] = 10;
	enemyPos[2].m128_f32[2] = 10;

	for (int i = 0; i < 3; i++) {
		rob[i]->SetPosition({ enemyPos[i] });
	}
}
//
void middle::SetPSR()
{

	playerMat = player->GetMat();

	//HUD�̃|�W�V�����Z�b�g
	for (int i = 0; i < 9; i++) {
		bulletHUD[i]->SetSize({ spSiz });
		bulletHUD[i]->SetPosition({ spPos.x,spPos.y + 32 * i });
	}
	//�����[�h�̕���
	Reload->SetSize({ 128,64 });
	Reload->SetPosition({ 1140,300 });
	//������wave�̕���
	wave->SetSize({ 256,128 });
	wave->SetPosition({ 0,600 });
	//wave�̍ő吔
	maxcount->SetSize({ 80,80 });
	maxcount->SetPosition({ 320, 630 });
	//wave�̍ő�l�Ɛ����̊Ԃ�/������
	slash->SetSize({ 80,80 });
	slash->SetPosition({ 280,630 });
	//�ϓ�����J�E���^�[
	for (int i = 0; i < 5; i++) {
		changecount[i]->SetSize({ 80,80 });
		changecount[i]->SetPosition({ 240,630 });
		LifeCount[i]->SetSize({ 80,80 });
		LifeCount[i]->SetPosition({ 1200,630 });
	}
	//Hp�o�[
	HpBer->SetSize({ 128,64 });
	HpBer->SetPosition({ 1070,650 });
	player->SetHp(playerHp);




}

//void middle::AllUpdata()
//{
//	//�v���C���[�̍X�V
//	player->Updata(bull);
//	rob->Updata(bull);
//
//	//�G�̍X�V
//	for (int i = 0; i < MAXENEMY; i++) {
//
//		enemy[i]->Updata(playerPos, patern, oldpatern, stop[i], playerPos, spown[i]);
//		enemyPos[i] = enemy[i]->GetPosition();
//		
//	}
//	//�e�̍X�V
//	for (int j = 0; j < 9; j++) {
//		bull[j]->Updata();
//	}
//}

void middle::Updata()
{

	SetPSR();


	//�G�����ׂē|�������ɐi��
	if (all[0] == false && all[1] == false && all[2] == false) {
		move = true;
		patern += 1;
	}


	if (move == true) {
		for (int i = 0; i < 3; i++) {
			all[i] = true;
		}
	}

	//���W�̐ݒ�
	SetPSR();



	if (spown == true) {

		SetEnemyPos();
		for (int i = 0; i < 3; i++) {
			rob[i]->SetPosition({ enemyPos[i] });
			rob[i]->SetRotation({ enemyRot[i] });
			rob[i]->SpownEnemy(playerMat, patern);
			if (i == 2) {
				spown = false;
			}
		}
	}

	//�G�̍X�V����
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 3; j++) {
			rob[j]->Updata(bull[i], all[j], playerMat, spown, playerHp);
		}
	}
	player->PlayerMove(move, patern, spown);
	//�v���C���[�̍X�V����
	player->Updata(bull, Remaining);
}

void middle::Draw(DirectXCommon* dxCommon)
{
	for (int j = 0; j < 3; j++) {
		rob[j]->Draw(dxCommon);
	}
	player->ParticleDraw(dxCommon->GetCmdList());
	Object3d::PreDraw(dxCommon->GetCmdList());
	for (int i = 0; i < 9; i++) {
		bull[i]->Draw();
	}
	player->ObjDraw();
	Object3d::PostDraw();

}
//
void middle::SpriteDraw()
{
	for (int i = Remaining; i < 8; i++) {
		bulletHUD[i]->Draw();
	}

	if (Remaining == 8) {
		Reload->Draw();
	}

	if (patern == 0) {
		changecount[0]->Draw();
	}
	else if (patern == 1) {
		changecount[1]->Draw();
	}
	else if (patern == 2) {
		changecount[2]->Draw();
	}
	else if (patern == 3) {
		changecount[3]->Draw();
	}
	else if (patern == 4) {
		changecount[4]->Draw();
	}

	if (playerHp == 1) {
		LifeCount[0]->Draw();
	}
	else if (playerHp == 2) {
		LifeCount[1]->Draw();
	}
	else if (playerHp == 3) {
		LifeCount[2]->Draw();
	}
	else if (playerHp == 4) {
		LifeCount[3]->Draw();
	}
	else if (playerHp == 5) {
		LifeCount[4]->Draw();
	}
	HpBer->Draw();
	maxcount->Draw();

	wave->Draw();
	slash->Draw();
	//bulletHUD[i]->Draw();
	player->SpriteDraw();
}
//
void middle::ImGuiDraw()
{
	player->ImGuiDraw();


}

void middle::Fainalize()
{
	for (int j = 0; j < 3; j++) {
		rob[j]->Finalize();
	}
	player->Finalize();
	//delete[] bulletHUD;
}

void middle::SetEnemyPos()
{
	if (patern == 1) {
		enemyPos[0].m128_f32[0] = -5;
		enemyPos[1].m128_f32[0] = 0;
		enemyPos[2].m128_f32[0] = 5;

		enemyPos[0].m128_f32[1] = 0;
		enemyPos[1].m128_f32[1] = 0;
		enemyPos[2].m128_f32[1] = 0;

		enemyPos[0].m128_f32[2] = 35;
		enemyPos[1].m128_f32[2] = 35;
		enemyPos[2].m128_f32[2] = 35;

	}

	else if (patern == 2) {
		enemyPos[0].m128_f32[0] = -5;
		enemyPos[1].m128_f32[0] = 0;
		enemyPos[2].m128_f32[0] = 5;

		enemyPos[0].m128_f32[1] = 0;
		enemyPos[1].m128_f32[1] = 0;
		enemyPos[2].m128_f32[1] = 0;

		enemyPos[0].m128_f32[2] = 55;
		enemyPos[1].m128_f32[2] = 55;
		enemyPos[2].m128_f32[2] = 55;

	}

	else if (patern == 3) {
		enemyPos[0].m128_f32[0] = 40;
		enemyPos[1].m128_f32[0] = 40;
		enemyPos[2].m128_f32[0] = 40;

		enemyPos[0].m128_f32[1] = 0;
		enemyPos[1].m128_f32[1] = 0;
		enemyPos[2].m128_f32[1] = 0;

		enemyPos[0].m128_f32[2] = 50;
		enemyPos[1].m128_f32[2] = 45;
		enemyPos[2].m128_f32[2] = 40;


		enemyRot[0].y = 90;
		enemyRot[1].y = 90;
		enemyRot[2].y = 90;
	}

	else if (patern == 4) {
		enemyPos[0].m128_f32[0] = 55;
		enemyPos[1].m128_f32[0] = 55;
		enemyPos[2].m128_f32[0] = 55;

		enemyPos[0].m128_f32[1] = 0;
		enemyPos[1].m128_f32[1] = 0;
		enemyPos[2].m128_f32[1] = 0;

		enemyPos[0].m128_f32[2] = 50;
		enemyPos[1].m128_f32[2] = 45;
		enemyPos[2].m128_f32[2] = 40;
	}

	else if (patern == 5)
	{
		enemyPos[0].m128_f32[0] = 46;
		enemyPos[1].m128_f32[0] = 41;
		enemyPos[2].m128_f32[0] = 36;

		enemyPos[0].m128_f32[1] = 0;
		enemyPos[1].m128_f32[1] = 0;
		enemyPos[2].m128_f32[1] = 0;

		enemyPos[0].m128_f32[2] = 5;
		enemyPos[1].m128_f32[2] = 5;
		enemyPos[2].m128_f32[2] = 5;

		enemyRot[0].y = 180;
		enemyRot[1].y = 180;
		enemyRot[2].y = 180;
	}

	else if (patern == 6)
	{
		enemyPos[0].m128_f32[0] = 46;
		enemyPos[1].m128_f32[0] = 41;
		enemyPos[2].m128_f32[0] = 36;

		enemyPos[0].m128_f32[1] = 0;
		enemyPos[1].m128_f32[1] = 0;
		enemyPos[2].m128_f32[1] = 0;

		enemyPos[0].m128_f32[2] = -15;
		enemyPos[1].m128_f32[2] = -15;
		enemyPos[2].m128_f32[2] = -15;
	}

	else if (patern == 7)
	{
		enemyPos[0].m128_f32[0] = 0;
		enemyPos[1].m128_f32[0] = 0;
		enemyPos[2].m128_f32[0] = 0;

		enemyPos[0].m128_f32[1] = 0;
		enemyPos[1].m128_f32[1] = 0;
		enemyPos[2].m128_f32[1] = 0;

		enemyPos[0].m128_f32[2] = 3;
		enemyPos[1].m128_f32[2] = -2;
		enemyPos[2].m128_f32[2] = -7;

		enemyRot[0].y = 270;
		enemyRot[1].y = 270;
		enemyRot[2].y = 270;
	}
}


//void middle::LoadEnemyPopData()
//{
//	//�t�@�C���I�[�v��
//	std::ifstream file;
//	file.open("Resources/enemyPop.csv");
//	assert(file.is_open());
//	//�t�@�C�����e�𕶎���X�g���[���ɃR�s�[
//	enemyPopCommands << file.rdbuf();
//
//	//�t�@�C���N���[�Y
//	file.close();
//}
//
//void middle::UpdataEnemyPopCommands()
//{
//	//�ҋ@����
//	if (waitF == true) {
//		waitTimer--;
//		if (waitTimer <= 0) {
//			waitF = false;
//		}
//		return;
//	}
//
//	//1�s���̕����������ϐ�
//	std::string line;
//
//	//�R�}���h���s���[�v
//	while (getline(enemyPopCommands, line))
//	{
//		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
//		std::istringstream line_stream(line);
//
//		std::string word;
//		//,��؂�ōs�̐擪��������擾
//		getline(line_stream, word, ',');
//
//		//"//"����n�܂�s�̓R�����g
//		if (word.find("//") == 0) {
//			//�R�����g�s���΂�
//			continue;
//		}
//		if (word.find("POP") == 0) {
//			//x���W
//			getline(line_stream, word, ',');
//			float x = (float)std::atof(word.c_str());
//
//			//y���W
//			getline(line_stream, word, ',');
//			float y = (float)std::atof(word.c_str());
//
//			//x���W
//			getline(line_stream, word, ',');
//			float z = (float)std::atof(word.c_str());
//
//
//			//�G�𔭐�������
//			enemyPos[0].m128_f32[0] = x;
//			enemyPos[0].m128_f32[1] = y;
//			enemyPos[0].m128_f32[2] = z;
//			XMVECTOR o{ x,y,z };
//			tst = make_unique<Enemy>();
//			tst->SetPosition(o);
//		}
//
//		//WAIT�R�}���h
//		else if (word.find("WAIT") == 0) {
//			getline(line_stream, word, ',');
//
//			//�҂�����
//			int32_t waitTime = atoi(word.c_str());
//
//			//�ҋ@����
//			waitF = true;
//			waitTimer = waitTime;
//
//			//�R�}���h�𔲂���
//			break;
//		}
//	}
//}
