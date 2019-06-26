#pragma once

class Battle {
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
