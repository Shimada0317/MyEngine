#include "middle.h"
#include"imgui/imgui.h"
#include<fstream>
//
////セーブ
//
void middle::Initialize()
{

	//camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	//Object3d::SetCamera(camera);
	for (int i = 0; i < 9; i++) {
		bull[i] = new Bullet();
		bull[i]->Initialize();
	}
	for (int i = 0; i < 3; i++) {
		rob[i] = std::make_unique<Robot>();
		rob[i]->Initialize();
		allpos[i] = { 0.0f + i * 1.0f,0.0f,10.0f };
		all[i] = true;
	}
	player = std::make_unique<Player>();
	player->Initalize();


	////スプライトの読み込み
	for (int i = 0; i < 9; i++) {

		Sprite::LoadTexture(i, L"Resources/bullet.png");
		bulletHUD[i] = Sprite::SpriteCreate(i, { 10.0f,10.0f });
	}

	Sprite::LoadTexture(10, L"Resources/reload.png");
	Reload = Sprite::SpriteCreate(10, { 10.0f,10.0f }, { 1.0f,1.0f,1.0f,1.0f });

	Sprite::LoadTexture(11, L"Resources/wave.png");
	wave = Sprite::SpriteCreate(11, { 10.0f,10.0f });

	Sprite::LoadTexture(17, L"Resources/five.png");
	maxcount = Sprite::SpriteCreate(17, { 10.0f,10.0f });

	Sprite::LoadTexture(12, L"Resources/slash.png");
	slash = Sprite::SpriteCreate(12, { 10.0f,10.0f });

	Sprite::LoadTexture(13, L"Resources/one.png");
	Sprite::LoadTexture(14, L"Resources/two.png");
	Sprite::LoadTexture(15, L"Resources/three.png");
	Sprite::LoadTexture(16, L"Resources/four.png");
	for (int i = 0; i < 5; i++) {
		changecount[i] = Sprite::SpriteCreate(13 + i, { 10.0f,10.0f });
	}
	//LoadEnemyPopData();
	//UpdateEnemyPopCommands();
	oldpatern = patern;
}
//
void middle::SetPSR()
{
	for (int i = 0; i < 3; i++) {
		rob[i]->SetPosition(allpos[i]);
	}


	//HUDのポジションセット
	for (int i = 0; i < 9; i++) {
		bulletHUD[i]->SetSize({ spSiz });
		bulletHUD[i]->SetPosition({ spPos.x,spPos.y + 32 * i });
	}
	//リロードの文字
	Reload->SetSize({ 128,64 });
	Reload->SetPosition({ 1140,300 });
	//左下のwaveの文字
	wave->SetSize({ 256,128 });
	wave->SetPosition({ 0,600 });
	//waveの最大数
	maxcount->SetSize({ 80,80 });
	maxcount->SetPosition({ 320, 630 });
	//waveの最大値と数字の間の/←これ
	slash->SetSize({ 80,80 });
	slash->SetPosition({ 280,630 });
	//変動するカウンター
	for (int i = 0; i < 5; i++) {
		changecount[i]->SetSize({ 80,80 });
		changecount[i]->SetPosition({ 240,630 });
	}

	//プレイヤーのポジションセット
	if (moveTime == false) {
		playerPos = player->GetPosition();
	}
	else {
		player->SetPosition(playerPos);
	}

}

//void middle::AllUpdate()
//{
//	//プレイヤーの更新
//	player->Update(bull);
//	rob->Update(bull);
//
//	//敵の更新
//	for (int i = 0; i < MAXENEMY; i++) {
//
//		enemy[i]->Update(playerPos, patern, oldpatern, stop[i], playerPos, spown[i]);
//		enemyPos[i] = enemy[i]->GetPosition();
//		
//	}
//	//弾の更新
//	for (int j = 0; j < 9; j++) {
//		bull[j]->Update();
//	}
//}

void middle::Update()
{

	SetPSR();
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 3; j++) {
			rob[j]->Update(bull[i], all[j]);
			//break;
		}
		//break;
	}
	if (move == false) {
		if (all[0] == false && all[1] == false && all[2] == false) {
			go = true;
			move = true;
		}
	}
	if (go == true) {
		patern += 1;
		if (move == false) {
			for (int i = 0; i < 3; i++) {
				all[i] = true;
			}
		}
		go = false;
	}

	player->Update(bull, Remaining,move);


	//	//敵を倒した時、hitカウントを上げる
	//	if (count == true) {
	//		hit += 1;
	//		count = false;
	//	}
	//
	//	//hitカウントがMAXENEMYになった時、ウェーブを進める
	//	if (hit >= MAXENEMY) {
	//		patern += 1;
	//		cammove = 0.1f;
	//		hit = 0;
	//		moveTime = true;
	//	}
	//	//waveが進むごとにカメラも奥に進む
	//	if (patern != 0) {
	//		playerPos.m128_f32[2] += cammove;
	//		life[1] -= 3;
	//		life[0] -= 3;
	//		if (playerPos.m128_f32[2] >= 10 * patern && patern > oldpatern) {
	//			for (int i = 0; i < MAXENEMY; i++) {
	//				spown[i] = true;
	//			}
	//			moveTime = false;
	//			cammove = 0;
	//			oldpatern = patern;
	//		}
	//	}
	//
	//



	//
	//
	//	SetPSR();
	//
	//	AllUpdate();
}
//
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
	//	for (int j = 0; j < 9; j++) {
	//		bull[j]->Draw();
	//	}
	//	for (int i = 0; i < MAXENEMY; i++) {
	//		enemy[i]->Draw();
	//
	//	}
	//	player->ObjDraw();
	//	//rob->Draw();
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

	maxcount->Draw();

	wave->Draw();
	slash->Draw();
	//bulletHUD[i]->Draw();

}
//
//void middle::ImGuiDraw()
//{
//	player->ImGuiDraw();
//	float r = Remaining;
//	float p = patern;
//	float a = ans;
//	float t = ReloadTime;
//	float ow = oldpatern;
//
//	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
//	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.0f, 0.1f, 0.0f));
//	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
//	ImGui::Begin("mouth");
//	ImGui::Checkbox("shot", &shot[0]);
//	ImGui::Checkbox("stop", &stop[0]);
//	ImGui::Checkbox("stop", &stop[1]);
//	ImGui::SliderFloat("ReloadTimer", &t, -100.0f, 100.0f);
//	ImGui::SliderFloat("Remaining", &r, -100.0f, 100.0f);
//	ImGui::SliderFloat("Remaining", &a, -100.0f, 100.0f);
//	if (ImGui::TreeNode("playerSts")) {
//
//		ImGui::SliderFloat("Rot.x", &playerRot.x, -100.0f, 100.0f);
//		ImGui::SliderFloat("Rot.y", &playerRot.y, -100.0f, 100.0f);
//		ImGui::SliderFloat("Rot.z", &playerRot.z, -100.0f, 100.0f);
//
//		ImGui::TreePop();
//	}
//	if (ImGui::TreeNode("EnePos")) {
//
//		ImGui::SliderFloat("EnePos.x", &enemyPos[0].m128_f32[0], -100.0f, 100.0f);
//		ImGui::SliderFloat("EnePos.y", &enemyPos[0].m128_f32[1], -100.0f, 100.0f);
//		ImGui::SliderFloat("EnePos.z", &enemyPos[0].m128_f32[2], -100.0f, 100.0f);
//		ImGui::SliderFloat("EnePos.x", &enemyPos[1].m128_f32[0], -100.0f, 100.0f);
//		ImGui::SliderFloat("EnePos.y", &enemyPos[1].m128_f32[1], -100.0f, 100.0f);
//		ImGui::SliderFloat("EnePos.z", &enemyPos[1].m128_f32[2], -100.0f, 100.0f);
//		ImGui::TreePop();
//	}
//
//	if (ImGui::TreeNode("bulletHUD")) {
//		ImGui::Checkbox("shot", &shot[0]);
//		ImGui::Checkbox("shot", &shot[1]);
//		ImGui::Checkbox("shot", &shot[2]);
//		ImGui::Checkbox("shot", &shot[3]);
//		ImGui::Checkbox("shot", &shot[4]);
//		ImGui::Checkbox("shot", &shot[5]);
//		ImGui::Checkbox("shot", &shot[6]);
//		ImGui::Checkbox("shot", &shot[7]);
//		ImGui::Checkbox("shot", &shot[8]);
//		ImGui::TreePop();
//	}
//
//	if (ImGui::TreeNode("other")) {
//		ImGui::SliderFloat("speed", &speed[0], -100.0f, 100.0f);
//		ImGui::SliderFloat("Remaining", &r, -100.0f, 100.0f);
//		ImGui::SliderFloat("hit", &hit, -100.0f, 100.0f);
//		ImGui::SliderFloat("patern", &p, -100.0f, 100.0f);
//		ImGui::SliderFloat("oldpatern", &ow, -100.0f, 100.0f);
//		ImGui::Checkbox("count", &count);
//		ImGui::TreePop();
//	}
//
//	ImGui::End();
//	ImGui::PopStyleColor();
//	ImGui::PopStyleColor();
//	enemy[0]->ImGuiDraw();
//	enemy[1]->ImGuiDraw();
//}
//
void middle::Fainalize()
{
	for (int j = 0; j < 3; j++) {
		rob[j]->Finalize();
	}
	player->Finalize();
	//delete[] bulletHUD;
}
//

//
//void middle::LoadEnemyPopData()
//{
//	//ファイルオープン
//	std::ifstream file;
//	file.open("Resources/enemyPop.csv");
//	assert(file.is_open());
//	//ファイル内容を文字列ストリームにコピー
//	enemyPopCommands << file.rdbuf();
//
//	//ファイルクローズ
//	file.close();
//}
//
//void middle::UpdateEnemyPopCommands()
//{
//	//待機処理
//	if (waitF == true) {
//		waitTimer--;
//		if (waitTimer <= 0) {
//			waitF = false;
//		}
//		return;
//	}
//
//	//1行分の文字列を入れる変数
//	std::string line;
//
//	//コマンド実行ループ
//	while (getline(enemyPopCommands, line))
//	{
//		//1行分の文字列をストリームに変換して解析しやすくする
//		std::istringstream line_stream(line);
//
//		std::string word;
//		//,区切りで行の先頭文字列を取得
//		getline(line_stream, word, ',');
//
//		//"//"から始まる行はコメント
//		if (word.find("//") == 0) {
//			//コメント行を飛ばす
//			continue;
//		}
//		if (word.find("POP") == 0) {
//			//x座標
//			getline(line_stream, word, ',');
//			float x = (float)std::atof(word.c_str());
//
//			//y座標
//			getline(line_stream, word, ',');
//			float y = (float)std::atof(word.c_str());
//
//			//x座標
//			getline(line_stream, word, ',');
//			float z = (float)std::atof(word.c_str());
//
//
//			//敵を発生させる
//			enemyPos[0].m128_f32[0] = x;
//			enemyPos[0].m128_f32[1] = y;
//			enemyPos[0].m128_f32[2] = z;
//			XMVECTOR o{ x,y,z };
//			tst = make_unique<Enemy>();
//			tst->SetPosition(o);
//		}
//
//		//WAITコマンド
//		else if (word.find("WAIT") == 0) {
//			getline(line_stream, word, ',');
//
//			//待ち時間
//			int32_t waitTime = atoi(word.c_str());
//
//			//待機時間
//			waitF = true;
//			waitTimer = waitTime;
//
//			//コマンドを抜ける
//			break;
//		}
//	}
//}
