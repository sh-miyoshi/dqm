#include "include.h"
#include "party.h"

void Party::LoadData(std::string fileName) {
	// TODO(save fileからキャラクタをロードする)
	ASSERT(0, "未実装");
}

void Party::InitData() {
	// debug(最初はスラッシュのみ)
	party.push_back(PartyData("スラッシュ", 25, 10, 16, 7, 1, 10, 0, CharDataMgr::ID_スライム));
	party.push_back(PartyData("ライス", 21, 6, 18, 5, 1, 7, 0, CharDataMgr::ID_サボテンボール));

	for( auto i : party ) {
		i.LoadImage();
	}
}

void Party::DeleteData() {
	for( auto i : party ) {
		i.DeleteImage();
	}
}
