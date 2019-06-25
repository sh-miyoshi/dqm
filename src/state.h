#pragma once

#include <stack>

#ifndef NULL
#define NULL (0)
#endif // !NULL

class StateBase {
public:
	virtual ~StateBase() {}

	virtual void Draw() {}
	virtual void Process() = 0;
};

class StateMgr {
	int mark;
	std::stack<StateBase *> stateStack;
public:
	StateMgr():mark(-1){}
	~StateMgr() {
		while( !stateStack.empty() ) {
			StateBase *state = stateStack.top();
			delete state;
			stateStack.pop();
		}
		mark = -1;
	}

	void Draw() {
		if( !stateStack.empty() ) {
			StateBase *now = stateStack.top();
			now->Draw();
		}
	}

	void Process() {
		if( !stateStack.empty() ) {
			StateBase *now = stateStack.top();
			now->Process();
		}
	}

	void PushState(StateBase *next) {
		stateStack.push(next);
	}

	void PopState() {
		if( !stateStack.empty() ) {
			StateBase *now = stateStack.top();
			delete now;
			stateStack.pop();
		}
	}

	void Mark() {
		mark = stateStack.size();
	}

	void BackToMark() {
		if( mark > 0 ) {
			while( (int)stateStack.size() > mark ) {
				StateBase *state = stateStack.top();
				delete state;
				stateStack.pop();
			}
			mark = -1;
		}
	}
};
