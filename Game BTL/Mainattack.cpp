#include "Mainattack..h"

Mainattack::Mainattack()
{
	x_val_ = 0;
	y_val_ = 0;
	is_move_ = false;
	attack_dir = 0;
	attack_type = Attack1;
}

Mainattack::~Mainattack()
{

}

bool Mainattack::LoadAttack(SDL_Renderer* des)
{
	bool ret = false;
	if (attack_type == Attack2)
	{
		ret = LoadImg("img//vu_khí _3.png", des);
	}
	else
	{
		ret = LoadImg("img//vu_khi1.png", des);
	}
	return ret;
}
void Mainattack::HandleMove(const int& x_boder_, const int& y_boder)
{
	if (attack_dir = DIR_RIGHT)
	{
		rect_.x += x_val_;
		if (rect_.x > x_boder_)
		{
			is_move_ = false;
		}
	}
	else if (attack_dir = DIR_LEFT)
	{
		rect_.x -= x_val_;
		if (rect_.x < 0)
		{
			is_move_ = false;
		}
	}
	
}

