#pragma once

class Fps{
	float ave;
	unsigned int count, baseCnt;
	int *frame;
public:
	Fps();
	~Fps();

	void Process();
	void Draw(int x, int y);
};