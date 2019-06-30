#pragma once

#include <string>
#include <vector>
#include "charData.h"

class Party {
	std::vector<int> itemIDs;
	std::string playerName;
	std::vector<PartyData> party;
	int money;

	Party():money(0){}
	~Party() {}
public:
	static const int PARTY_MAX_NUM = 3;

	static Party *GetInst() {
		static Party inst;
		return &inst;
	}

	void InitData();
	void LoadData(std::string fileName);
	void DeleteData();

	std::string GetPlayerName()const { return playerName; }
	std::vector<PartyData> GetParty()const { return party; }
	int GetMoney()const { return money; }
	std::vector<int> GetItemIDs()const { return itemIDs; }
};