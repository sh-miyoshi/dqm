#include "include.h"
#include "party.h"
#include "itemMgr.h"

void Party::LoadData(std::string fileName) {
	// TODO(save file����L�����N�^�����[�h����)
	ASSERT(0, "������");
}

void Party::InitData() {
	playerName = "�v���C���[";// TODO(user�Ɍ��߂�����)

	// debug(�ŏ��̓X���b�V���̂�)
	party.push_back(PartyData("�X���b�V��", 25, 10, 16, 7, 1, 10, 0, CharDataMgr::ID_�X���C��));
	party.push_back(PartyData("���C�X", 21, 6, 18, 5, 1, 7, 0, CharDataMgr::ID_�T�{�e���{�[��));

	for( auto i : party ) {
		i.LoadImage();
	}

	// debug(�f�o�b�O�p�ɏ����A�C�e������������)
	itemIDs.push_back(ItemMgr::ID_��);
	itemIDs.push_back(ItemMgr::ID_��);
	itemIDs.push_back(ItemMgr::ID_���@�̐���);
}

void Party::DeleteData() {
	for( auto i : party ) {
		i.DeleteImage();
	}
}

bool Party::Recover(int targetNo, bool targetIsHP, int point, int rndMax) {
	ASSERT(0 <= targetNo && targetNo < (int)party.size(), "targetNo���ُ�l�ł�");
	return party[targetNo].Recover(targetIsHP, point, rndMax);
}

bool Party::AddItem(int itemID) {
	ASSERT(0 <= itemID && itemID < ItemMgr::ID_MAX, ToString("�ǉ����悤�Ƃ��Ă���ItemID���ُ�l�ł�: itemID: %d",itemID));

	// TODO(����̏������̍ő�l�����߂�)
	itemIDs.push_back(itemID);
	return true;
}

bool Party::DeleteItem(int itemNo) {
	if( 0 <= itemNo && itemNo < ( int ) itemIDs.size() ) {
		// TODO(�厖�Ȃ���)
		itemIDs.erase(itemIDs.begin() + itemNo);
		return true;
	}
	return false;
}
