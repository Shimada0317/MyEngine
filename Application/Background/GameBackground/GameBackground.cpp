#include "GameBackground.h"
#include"ModelManager.h"
#include<iomanip>
#include<fstream>

GameBackground::~GameBackground()
{
	for (unique_ptr<Object3d>& new_backgrpund : backgrouhnd_) {
		new_backgrpund.reset();
	}
}

void GameBackground::LoadBackgrounndPopData()
{
	ifstream file;
	file.open("Resources/background.csv");
	assert(file.is_open());

	backgrouhnd_popcommands_ << file.rdbuf();

	file.close();
}

void GameBackground::UpdateBackgroudPopCommands()
{
	//‘Ò‹@ˆ—
	if (waitflag_ == true) {
		waittime_--;
		if (waittime_ <= 0) {
			waitflag_ = false;
		}
		return;
	}

	string line;

	XMVECTOR POSITION = { 0.f,0.f,0.f };
	XMFLOAT3 ROTATION = { 0.f,0.f,0.f };
	const XMFLOAT3 SCALE{ 6.f,10.f,7.f };

	bool POSSkip = false;
	bool ROTSkip = false;

	while (getline(backgrouhnd_popcommands_, line))
	{
		istringstream line_stram(line);

		string word;

		getline(line_stram, word, ',');

		if (word.find("//") == 0) {
			continue;
		}

		if (word.find("POS") == 0) {
			getline(line_stram, word, ',');
			float x = (float)atof(word.c_str());

			getline(line_stram, word, ',');
			float y = (float)atof(word.c_str());

			getline(line_stram, word, ',');
			float z = (float)atof(word.c_str());

			POSITION.m128_f32[0] = x;
			POSITION.m128_f32[1] = y;
			POSITION.m128_f32[2] = z;

			POSSkip = true;
		}
		else if (word.find("ROT") == 0) {
			getline(line_stram, word, ',');
			float x = (float)atof(word.c_str());

			getline(line_stram, word, ',');
			float y = (float)atof(word.c_str());

			getline(line_stram, word, ',');
			float z = (float)atof(word.c_str());

			ROTATION.x = x;
			ROTATION.y = y;
			ROTATION.z = z;

			ROTSkip = true;
		}

		if (POSSkip && ROTSkip) {
			unique_ptr<Object3d> new_background = make_unique<Object3d>();
			new_background = Object3d::Create(ModelManager::GetInstance()->GetModel(kBuils));
			new_background->SetPosition(POSITION);
			new_background->SetRotation(ROTATION);
			new_background->SetScale(SCALE);
			backgrouhnd_.push_back(move(new_background));
			POSSkip = false;
			ROTSkip = false;
		}

		if (word.find("END") == 0) {
			getline(line_stram, word, ',');

			break;
		}
	}
}

void GameBackground::Update()
{
	const XMFLOAT4 BillColor = { 0.8f,0.6f,0.3f,1.0f };
	for (unique_ptr<Object3d>& new_backgrpund : backgrouhnd_) {
		new_backgrpund->Update(BillColor);
	}
}

void GameBackground::Draw()
{
	for (unique_ptr<Object3d>& new_backgrpund : backgrouhnd_) {
		new_backgrpund->Draw();
	}
}
