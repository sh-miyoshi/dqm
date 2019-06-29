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
		ID_薬草,
		ID_魔法の聖水,
		ID_魔物のエサ,
		ID_ルーラの杖,

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