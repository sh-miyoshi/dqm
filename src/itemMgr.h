#pragma once

#include <string>

typedef struct Item {
	int id;
	std::string name;
	std::string description;
	int power;
	bool isTargetSelect;
	bool useInFieldOK;
	bool useInBattleOK;
}Item_t;

class ItemMgr {
public:
	enum ItemID {
		ID_NULL,
		ID_��,
		ID_���@�̐���,
		ID_�����̃G�T,
		ID_���[���̏�,

		ID_MAX,
	};
private:
	bool isLoaded;
	Item_t itemData[ID_MAX];

	ItemMgr():isLoaded(false){}
	~ItemMgr() {}
public:
	static ItemMgr* GetInst() {
		static ItemMgr inst;
		return &inst;
	}

	void LoadData();
	Item_t GetInfo(int id);
};