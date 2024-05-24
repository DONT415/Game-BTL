#ifndef MainPower_H
#define MainPower_H

#include "CommonFunc.h"
#include "BaseObject.h"

class MainPower : public BaseObject
{
public:
	MainPower();
	~MainPower();

	void SetNum(const int& num) { number_ = num; }
	void AddPos(const int& xPos);
	void Show(SDL_Renderer* screen);
	void Init(SDL_Renderer* screen);
	void Decrease();

private:
	int number_;
	std::vector<int> position_list_;
};

#endif