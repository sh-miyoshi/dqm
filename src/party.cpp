#include "include.h"
#include "party.h"

void Party::LoadData(std::string fileName) {
	// TODO(save file����L�����N�^�����[�h����)
	ASSERT(0, "������");
}

void Party::InitData() {
	// debug(�ŏ��̓X���b�V���̂�)
	party.push_back(PartyData("�X���b�V��", 25, 10, 16, 7, 1, 10, 0, CharDataMgr::ID_�X���C��));
	party.push_back(PartyData("���C�X", 21, 6, 18, 5, 1, 7, 0, CharDataMgr::ID_�T�{�e���{�[��));

	for( auto i : party ) {
		i.LoadImage();
	}
}

void Party::DeleteData() {
	for( auto i : party ) {
		i.DeleteImage();
	}
}
