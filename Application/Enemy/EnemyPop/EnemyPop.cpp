#include "EnemyPop.h"
#include"Action.h"
#include"Player.h"
#include<fstream>
#include"ThrowEnemy.h"

#include"Low.h"
#include"NormalEnemy.h"

void EnemyPop::LoadCsv()
{
	ifstream file;
	file.open("Resources/LoadEnemy.csv");
	assert(file.is_open());

	enemypopcommands_ << file.rdbuf();

	file.close();
}

void EnemyPop::PopEnemy(int phase, Camera* camera)
{
	std::string line;

	XMVECTOR POSITION = { 0.0f,0.0f,0.0f };
	XMVECTOR TRACK = { 0.0f,0.0f,0.0f };
	XMFLOAT3 ROTATION = { 0.0f,0.0f,0.0f };
	int TYPE = 0;

	bool ari = false;
	int count = 0;

	bool POPSkip = false;
	bool TRACKSkip = false;
	bool ARIVESkip = false;
	bool TYPESkip = false;

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
				TYPESkip = true;
			}

			if (ARIVESkip == true && POPSkip == true && TRACKSkip == true&& TYPESkip == true) {
				if (TYPE >= BaseEnemy::EnemyType::kNormal&&TYPE<=BaseEnemy::EnemyType::kNormalTank) {
					std::unique_ptr<BaseEnemy> newRobot = std::make_unique<NormalEnemy>(ROTATION, POSITION, TRACK,TYPE);
					newRobot->CreateRobot(camera);
					base_type_.push_back(std::move(newRobot));
				}

				else if (TYPE >= BaseEnemy::EnemyType::kThrowNormal&&TYPE <= BaseEnemy::EnemyType::kThrowBlue) {
					std::unique_ptr<BaseEnemy> newRobot = std::make_unique<ThrowEnemy>(ROTATION, POSITION, TRACK,TYPE);
					newRobot->CreateRobot(camera);
					base_type_.push_back(std::move(newRobot));
				}
				else if (TYPE >= BaseEnemy::EnemyType::kLowNormal&&TYPE<=BaseEnemy::EnemyType::kHighJumpLow) {
					std::unique_ptr<BaseEnemy> newRobot = std::make_unique<Low>(ROTATION, POSITION, TRACK,TYPE);
					newRobot->CreateRobot(camera);
					base_type_.push_back(std::move(newRobot));
				}

				else if (TYPE == BaseEnemy::EnemyType::kBoss) {
					std::unique_ptr<BossEnemy> boss = std::make_unique<BossEnemy>();
					boss->Initialize(ROTATION, POSITION, camera, TRACK);
					boss_.push_back(std::move(boss));
					break;
				}
				


				POPSkip = false;
				TRACKSkip = false;
				ARIVESkip = false;
				TYPESkip = false;
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

void EnemyPop::Update(Player* player)
{
	//敵の更新処理
	for (std::unique_ptr<BaseEnemy>& Enemy : base_type_) {
		Enemy->Update(player);
	}
	
	//ボスの更新処理
	for (std::unique_ptr<BossEnemy>& boss : boss_) {
		boss->Update(player);
	}

}

void EnemyPop::CheckSameTrackPosition()
{
	/*for (std::unique_ptr<NormalEnemy>& FirstEnemy : robot_) {
		for (std::unique_ptr<NormalEnemy>& SecondEnemy : robot_) {
			if (FirstEnemy.get() != SecondEnemy.get()) {
				XMVECTOR FirstTrackPosition = FirstEnemy->GetTrackPos();
				XMVECTOR SecondTrackPosition = SecondEnemy->GetTrackPos();
				bool secondenemyarive = SecondEnemy->IsDead();
				if (Action::GetInstance()->CompletelyTogetherXMVECTOR(FirstTrackPosition, SecondTrackPosition)) {
					otherenemyarive_ = true;
					SecondEnemy->WaitTrack(otherenemyarive_);
				}
				if (secondenemyarive == true) {
					XMVECTOR firstenemytrack = FirstEnemy->CheckTrackPoint();
					XMVECTOR secondenemytrack = SecondEnemy->CheckTrackPoint();
					if (Action::GetInstance()->CompletelyTogetherXMVECTOR(firstenemytrack, secondenemytrack)) {
						otherenemyarive_ = false;
						FirstEnemy->WaitTrack(otherenemyarive_);
					}
				}
			}
		}
	}*/
}

void EnemyPop::EnemyDead()
{
	
	base_type_.remove_if([](std::unique_ptr<BaseEnemy>& robot) {
		return robot->IsDead();
		});
	boss_.remove_if([](std::unique_ptr<BossEnemy>& boss) {
		return boss->IsDead();
		});


}

bool EnemyPop::KilledAllEnemy()
{
	if ( boss_.empty()&&base_type_.empty()) {
		return true;
	}
	return false;
}

void EnemyPop::Draw(DirectXCommon* dxcommon)
{

	for (std::unique_ptr<BossEnemy>& boss : boss_) {
		boss->Draw(dxcommon);
	}

	for (std::unique_ptr<BaseEnemy>& robot : base_type_) {
		robot->Draw(dxcommon);
	}
}

