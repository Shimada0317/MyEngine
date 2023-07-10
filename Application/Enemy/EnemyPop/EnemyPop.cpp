#include "EnemyPop.h"
#include"Action.h"
#include"Player.h"
#include<fstream>
#include "ThrowRedEnemy.h"
#include"ThrowBlueEnemy.h"
#include"ThrowGreenEnemy.h"
#include"NormalRedEnemy.h"
#include"Normal.h"

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
				if (TYPE == ENEMYPATERN::kNormal) {
					std::unique_ptr<Base> newRobot = std::make_unique<Normal>(ROTATION, POSITION, TRACK);
					newRobot->CreateRobot(camera);
					base_type_.push_back(std::move(newRobot));
				}

				else if (TYPE == ENEMYPATERN::kThrow) {
					std::unique_ptr<BaseThrow> newRobot = std::make_unique<ThrowRedEnemy>(ROTATION, POSITION ,TRACK);
					newRobot->Initialize( camera);
					base_.push_back(std::move(newRobot));
				}

				else if (TYPE == ENEMYPATERN::kBoss) {
					std::unique_ptr<BossEnemy> boss = std::make_unique<BossEnemy>();
					boss->Initialize(ROTATION, POSITION, camera, TRACK);
					boss_.push_back(std::move(boss));
					break;
				}
				else if (TYPE == ENEMYPATERN::kLow) {
					std::unique_ptr<LowEnemy> low = std::make_unique<LowEnemy>();
					low->Initialize(ROTATION, POSITION, camera, TRACK);
					low_.push_back(std::move(low));
				}
				else if (TYPE == ENEMYPATERN::kRocket) {
					std::unique_ptr<RocketEnemy> rocket = std::make_unique<RocketEnemy>();
					rocket->Initialize(ROTATION, POSITION, camera, TRACK);
					rocket_.push_back(std::move(rocket));
				}
				else if (TYPE == ENEMYPATERN::kThrowBlue) {
					std::unique_ptr<BaseThrow> newRobot = std::make_unique<ThrowBlueEnemy>(ROTATION, POSITION, TRACK);
					newRobot->Initialize(camera);
					base_.push_back(std::move(newRobot));
				}
				else if (TYPE == ENEMYPATERN::kThrowGreen) {
					std::unique_ptr<BaseThrow> newRobot = std::make_unique<ThrowGreenEnemy>(ROTATION, POSITION, TRACK);
					newRobot->Initialize(camera);
					base_.push_back(std::move(newRobot));
				}
				else if (TYPE == ENEMYPATERN::kThrowRed) {
					std::unique_ptr<BaseThrow> newRobot = std::make_unique<ThrowRedEnemy>(ROTATION, POSITION, TRACK);
					newRobot->Initialize(camera);
					base_.push_back(std::move(newRobot));
				}
				else if (TYPE == ENEMYPATERN::kNormalRed) {
					std::unique_ptr<BaseEnemy> newRobot = std::make_unique<NormalRedEnemy>(ROTATION, POSITION, TRACK);
					newRobot->Initialize(camera);
					base_enemy_.push_back(std::move(newRobot));
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
	for (std::unique_ptr<Base>& NormalEnemy : base_type_) {
		NormalEnemy->Update(player);
	}
	//浮遊している遠距離的
	for (std::unique_ptr<BaseThrow>& ThrowEnemy : base_) {
		ThrowEnemy->Update(player);
	}
	//浮遊している遠距離的
	for (std::unique_ptr<BaseEnemy>& NormalEnemy : base_enemy_) {
		NormalEnemy->Update(player);
	}
	//ボスの更新処理
	for (std::unique_ptr<BossEnemy>& boss : boss_) {
		boss->Update(player);
	}
	//姿勢の低い敵
	for (std::unique_ptr<LowEnemy>& low : low_) {
		low->Update(player);
	}
	for (std::unique_ptr<RocketEnemy>& rocket : rocket_) {
		rocket->Update(player);
	}
}

void EnemyPop::CheckSameTrackPosition()
{
	for (std::unique_ptr<NormalEnemy>& FirstEnemy : robot_) {
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
	}
}

void EnemyPop::EnemyDead()
{
	base_.remove_if([](std::unique_ptr<BaseThrow>& throwrobot) {
		return throwrobot->IsDead();
		});
	base_type_.remove_if([](std::unique_ptr<Base>& robot) {
		return robot->IsDead();
		});
	boss_.remove_if([](std::unique_ptr<BossEnemy>& boss) {
		return boss->IsDead();
		});
	low_.remove_if([](std::unique_ptr<LowEnemy>& low) {
		return low->IsDead();
		});
	rocket_.remove_if([](std::unique_ptr<RocketEnemy>& rocket) {
		return rocket->IsDead();
		});
	base_enemy_.remove_if([](std::unique_ptr<BaseEnemy>& base) {
		return base->IsDead();
		});
}

bool EnemyPop::KilledAllEnemy()
{
	if (robot_.empty() && base_.empty() && boss_.empty()&&low_.empty()&&rocket_.empty()&&base_enemy_.empty()&&base_type_.empty()) {
		return true;
	}
	return false;
}

void EnemyPop::Draw(DirectXCommon* dxcommon)
{
	for (std::unique_ptr<NormalEnemy>& robot : robot_) {
		robot->Draw(dxcommon);
	}
	for (std::unique_ptr<BaseThrow>& throwrobot : base_) {
		throwrobot->Draw(dxcommon);
	}
	for (std::unique_ptr<BossEnemy>& boss : boss_) {
		boss->Draw(dxcommon);
	}
	for (std::unique_ptr<LowEnemy>& low : low_) {
		low->Draw(dxcommon);
	}
	for (std::unique_ptr<RocketEnemy>& rocket : rocket_) {
		rocket->Draw(dxcommon);
	}
	for (std::unique_ptr<BaseEnemy>& base : base_enemy_) {
		base->Draw(dxcommon);
	}
	for (std::unique_ptr<Base>& robot : base_type_) {
		robot->Draw(dxcommon);
	}
}

