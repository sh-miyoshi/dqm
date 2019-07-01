#include "include.h"
#include "party.h"
#include "itemMgr.h"

void Party::LoadData(std::string fileName) {
	// TODO(save fileからキャラクタをロードする)
	ASSERT(0, "未実装");
}

void Party::InitData() {
	playerName = "プレイヤー";// TODO(userに決めさせる)

	// debug(最初はスラッシュのみ)
	party.push_back(PartyData("スラッシュ", 25, 10, 16, 7, 1, 10, 0, CharDataMgr::ID_スライム));
	party.push_back(PartyData("ライス", 21, 6, 18, 5, 1, 7, 0, CharDataMgr::ID_サボテンボール));

	for( auto i : party ) {
		i.LoadImage();
	}

	// debug(デバッグ用に初期アイテムを持たせる)
	itemIDs.push_back(ItemMgr::ID_薬草);
	itemIDs.push_back(ItemMgr::ID_薬草);
	itemIDs.push_back(ItemMgr::ID_魔法の聖水);
}

void Party::DeleteData() {
	for( auto i : party ) {
		i.DeleteImage();
	}
}

bool Party::Recover(int targetNo, bool targetIsHP, int point, int rndMax) {
	ASSERT(0 <= targetNo && targetNo < (int)party.size(), "targetNoが異常値です");
	return party[targetNo].Recover(targetIsHP, point, rndMax);
}

bool Party::AddItem(int itemID) {
	ASSERT(0 <= itemID && itemID < ItemMgr::ID_MAX, ToString("追加しようとしているItemIDが異常値です: itemID: %d",itemID));

	// TODO(道具の所持数の最大値を決める)
	itemIDs.push_back(itemID);
	return true;
}

bool Party::DeleteItem(int itemNo) {
	if( 0 <= itemNo && itemNo < ( int ) itemIDs.size() ) {
		// TODO(大事なもの)
		itemIDs.erase(itemIDs.begin() + itemNo);
		return true;
	}
	return false;
}
