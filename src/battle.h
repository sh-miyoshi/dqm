#pragma once

#include <vector>
#include "charData.h"
#include "state.h"
#include "winMgr.h"

class Battle {
	std::vector<PartyData> party;
	StateMgr stateMgr;
	WinMgr winMgr;
public:
	enum RtnCode {
		eRTN_CONTINUE,
		eRTN_WIN,
		eRTN_LOSE,
	};

	Battle();
	~Battle();

	void Draw();
	RtnCode Process();
};
