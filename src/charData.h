#pragma once

#include <string>
#include <vector>
#include "point.h"

class CharData {
public:
	static const int SKILL_MAX = 8;

	std::string name;
	int hp, hpMax;
	int mp, mpMax;
	int atk, def;
	int lv;
	int spd;
	int image;
	std::string charType;

	int exp;
	int expPtn;
//	std::vector<int> skillData;// TODO(Add ���Z���)

	CharData();
	~CharData();

	void LoadImage();
	void DeleteImage();

	void Draw(CPoint<int> pos);
};

class PartyData:public CharData {
public:
	std::string nickName;
//	std::vector<int> nowSkill;// TODO(Add ���Z���)

	PartyData(
		std::string nickName,
		int hp,
		int mp,
		int atk,
		int def,
		int lv,
		int spd,
		int exp,
		int charID
	);
	~PartyData();

	// todo(Save�֐�)
};

class CharDataMgr {
public:
	// charData.csv�t�@�C���ƍ��킹��K�v����
	enum CharID {
		ID_�X���C��,
		ID_�Ԃ��X���C��,
		ID_�T�{�e���{�[��,
		ID_�Ƃ����ւ�,
		ID_�e�[���C�[�^�[,

		ID_MAX
	};
private:
	bool isLoad;
	CharData charData[ID_MAX];

	CharDataMgr():isLoad(false) {}
	~CharDataMgr() {}
public:
	static CharDataMgr *GetInst() {
		static CharDataMgr inst;
		return &inst;
	}

	void LoadData();
	void DeleteData();

	CharData GetData(int charID);
};