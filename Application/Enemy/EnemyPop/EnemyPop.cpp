#include "EnemyPop.h"
#include<fstream>

void EnemyPop::LoadCsv()
{
	ifstream file;
	file.open("Resources/LoadEnemy.csv");
	assert(file.is_open());

	enemypopcommands_ << file.rdbuf();

	file.close();
}

void EnemyPop::PopEnemy(list<unique_ptr<NormalEnemy>>& normalrobot, list<unique_ptr<ThrowEnemy>>& throwrobot, list<unique_ptr<BossEnemy>>& bossrobot,int phase, Camera* camera)
{
	std::string line;

	XMVECTOR POSITION = { 0.0f,0.0f,0.0f };
	XMVECTOR TRACK = { 0.0f,0.0f,0.0f };
	XMFLOAT3 ROTATION = { 0.0f,0.0f,0.0f };
	int TYPE = 0;

	bool ari = false;
	bool step = false;
	int count = 0;

	bool POPSkip = false;
	bool TRACKSkip = false;
	bool ARIVESkip = false;

	while (getline(enemypopcommands_, line))
	{

		std::istringstream line_stram(line);

		std::string word;

		getline(line_stram, word, ',');

		if (word.find("//") == 0) {
			continue;
		}

		if (word.find("WAVE") == 0) {
			getline(line_stram, word, ',');
			//WAVEの要素
			count = atoi(word.c_str());
		}
		if (phase == count) {
			//角度の取得
			if (word.find("ROTATION") == 0) {

				getline(line_stram, word, ',');
				float x = (float)std::atof(word.c_str());

				getline(line_stram, word, ',');
				float y = (float)std::atof(word.c_str());

				getline(line_stram, word, ',');
				float z = (float)std::atof(word.c_str());

				ROTATION.x = x;
				ROTATION.y = y;
				ROTATION.z = z;

				POPSkip = true;
			}
			//座標取得
			else if (word.find("POSITION") == 0) {

				getline(line_stram, word, ',');
				float x = (float)std::atof(word.c_str());

				getline(line_stram, word, ',');
				float y = (float)std::atof(word.c_str());

				getline(line_stram, word, ',');
				float z = (float)std::atof(word.c_str());

				POSITION.m128_f32[0] = x;
				POSITION.m128_f32[1] = y;
				POSITION.m128_f32[2] = z;

				POPSkip = true;
			}
			//追尾先の座標取得
			else if (word.find("TRACK") == 0) {

				getline(line_stram, word, ',');
				float x = (float)std::atof(word.c_str());

				getline(line_stram, word, ',');
				float y = (float)std::atof(word.c_str());

				getline(line_stram, word, ',');
				float z = (float)std::atof(word.c_str());

				TRACK.m128_f32[0] = x;
				TRACK.m128_f32[1] = y;
				TRACK.m128_f32[2] = z;

				TRACKSkip = true;
			}

			else if (word.find("STEP") == 0) {
				getline(line_stram, word, ',');
				int STEP = atoi(word.c_str());
				if (STEP == 1) {
					step = true;
				}
				else {
					step = false;
				}
			}

			//
			else if (word.find("ARIVE") == 0) {
				getline(line_stram, word, ',');

				//待ち時間
				int Arive = atoi(word.c_str());
				if (Arive == 1) {
					ari = true;
				}
				else {
					ari = false;
				}

				ARIVESkip = true;
			}
			//敵のパターン
			else if (word.find("TYPE") == 0) {
				getline(line_stram, word, ',');
				int type = (int)std::atof(word.c_str());
				TYPE = type;
			}

			if (ARIVESkip == true && POPSkip == true && TRACKSkip == true) {
				if (TYPE == ENEMYPATERN::kNormal) {
					std::unique_ptr<NormalEnemy> newRobot = std::make_unique<NormalEnemy>();
					newRobot->Initialize(ROTATION, POSITION, camera, TRACK);
					normalrobot.push_back(std::move(newRobot));
				}

				else if (TYPE == ENEMYPATERN::kThrow) {
					std::unique_ptr<ThrowEnemy> newRobot = std::make_unique<ThrowEnemy>();
					newRobot->Initialize(ROTATION, POSITION, camera, TRACK);
					throwrobot.push_back(std::move(newRobot));
				}


				else if (TYPE == ENEMYPATERN::kBoss) {
					std::unique_ptr<BossEnemy> boss = std::make_unique<BossEnemy>();
					boss->Initialize(ROTATION, POSITION, camera, TRACK);
					bossrobot.push_back(std::move(boss));
					break;
				}
				POPSkip = false;
				TRACKSkip = false;
				ARIVESkip = false;
			}
		}

		if (phase < count) {
			break;
		}
		if (word.find("END") == 0) {
			getline(line_stram, word, ',');
			break;
		}
	}
}

