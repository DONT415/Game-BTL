#include "MainPower.h"

MainPower::MainPower()
{
	number_ = 0;
}

MainPower::~MainPower()
{

}

void MainPower::AddPos(const int& xPos)
{
	position_list_.push_back(xPos);
}

void MainPower::Show(SDL_Renderer* screen)
{
	for (int i = 0; i < position_list_.size(); i++)
	{
		rect_.x = position_list_.at(i);
		rect_.y = 0;
		Render(screen);
	}
}

void MainPower::Init(SDL_Renderer* screen)
{
	LoadImg("img//Main_Power.png", screen);
	number_ = 3;
	if (position_list_.size() > 0)
	{
		position_list_.clear();
	}
	AddPos(20);
	AddPos(60);
	AddPos(100);
}

void MainPower::Decrease()
{
	number_--;
	position_list_.pop_back();
}
