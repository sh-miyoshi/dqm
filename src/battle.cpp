#include "include.h"
#include "battle.h"

Battle::Battle()
{
}

Battle::~Battle()
{
}

void Battle::Draw(){
	DrawString(def::FMX / 2 - 40, def::FMY / 2 - 20, "í“¬’†", WHITE);
	DrawString(def::FMX / 2 - 40, def::FMY / 2 + 40, "Œˆ’è: Ÿ—˜", WHITE);
	DrawString(def::FMX / 2 - 40, def::FMY / 2 + 60, "ƒLƒƒƒ“ƒZƒ‹: ”s–k", WHITE);
}

Battle::RtnCode Battle::Process(){
	if (CKey::GetInst()->CheckKey(eKEY_ENTER) == 1) {
		return eRTN_WIN;
	}
	else if (CKey::GetInst()->CheckKey(eKEY_CANCEL) == 1) {
		return eRTN_LOSE;
	}
	return eRTN_CONTINUE;
}
