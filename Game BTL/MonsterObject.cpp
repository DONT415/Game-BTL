#include "MonsterObject.h"

MonsterObject::MonsterObject()
{
    width_frame_ = 0;
    height_frame_ = 0;
    x_val_ = 0;
    y_val_ = 0;
    x_pos_ = 0;
    y_pos_ = 0;
    on_ground_ = false;
    come_back_time_ = 0;
    frame_ = 0;
}

MonsterObject::~MonsterObject()
{
    
}

SDL_Rect MonsterObject::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = rect_.w / 6;
    rect.h = rect_.h;
    return rect;
}

bool MonsterObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg(path, screen);
    if (ret)
    {
        width_frame_ = rect_.w / Monster_FRAMES;
        height_frame_ = Monster_SPRITE_SIZE;
    }
    return ret;
}

void MonsterObject::Setclips()
{
    if (width_frame_ > 0 && height_frame_ > 0)
    {
        for (int i = 0; i < Monster_FRAMES; i++)
        {
            frame_clip_[i].x = width_frame_ * i;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }
    }
}

void MonsterObject::DoPlayer(Map& map_data)
{
    if (come_back_time_ == 0) {
        if (!on_ground_) {
            y_val_ += Monster_GRAVITY_SPEED;
            if (y_val_ >= Monster_MAX_FALL_SPEED) {
                y_val_ = Monster_MAX_FALL_SPEED;
            }
        }

        if (frame_ >= Monster_FRAMES) {
            frame_ = 0;
        }
    }
    else if (come_back_time_ > 0)
    {
        come_back_time_--;

        if (come_back_time_ == 0)
        {
            if (x_pos_ > 128)
            {
                x_pos_ -= 128;
            }
            if (x_pos_ < 0)
            {
                x_pos_ = 0;
            }
            y_pos_ = 0;
            x_val_ = 0;
            y_val_ = 0;
            on_ground_ = false;
        }
    }
    CheckToMap(map_data);
}

void MonsterObject::CheckToMap(Map& map_data)
{
    int x1 = 0, x2 = 0;
    int y1 = 0, y2 = 0;
    on_ground_ = 0;

    int height_min = height_frame_;

    x1 = (x_pos_ + x_val_) / TILE_SIZE;
    x2 = (x_pos_ + x_val_ + width_frame_ - 1) / TILE_SIZE;
    y1 = (y_pos_) / TILE_SIZE;
    y2 = (y_pos_ + height_min - 1) / TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if (x_val_ > 0)
        {
            if (map_data.tile[y1][x2] != 0 || map_data.tile[y2][x2] != 0)
            {
                x_pos_ = x2 * TILE_SIZE;
                x_pos_ -= width_frame_ + 1;
                x_val_ = 0;
            }
        }
        else if (x_val_ < 0)
        {
            if (map_data.tile[y1][x1] != 0 || map_data.tile[y2][x1] != 0)
            {
                x_pos_ = (x1 + 1) * TILE_SIZE;
                x_val_ = 0;
            }
        }
    }

    int width_min = width_frame_;

    x1 = (x_pos_) / TILE_SIZE;
    x2 = (x_pos_ + width_min) / TILE_SIZE;
    y1 = (y_pos_ + y_val_) / TILE_SIZE;
    y2 = (y_pos_ + y_val_ + height_frame_) / TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if (y_val_ > 0)
        {
            if (map_data.tile[y2][x1] != 0 || map_data.tile[y2][x2] != 0)
            {
                y_pos_ = y2 * TILE_SIZE;
                y_pos_ -= height_frame_;
                y_val_ = 0;
                on_ground_ = 1;
            }
        }
        else if (y_val_ < 0)
        {
            if (map_data.tile[y1][x1] != 0 || map_data.tile[y1][x2] != 0)
            {
                y_pos_ = (y1 + 1) * TILE_SIZE;
                y_val_ = 0;
            }
        }
    }

    x_pos_ += x_val_;
    y_pos_ += y_val_;

    if (x_pos_ < 0)
    {
        x_pos_ = 0;
    }
    else if (x_pos_ + width_frame_ > map_data.max_x_)
    {
        x_pos_ = map_data.max_x_ - width_frame_ - 1;
    }

    if (y_pos_ > map_data.max_y_)
    {
        come_back_time_ = 30;
    }
}

void MonsterObject::CheckFalling(Map& map_data)
{
    int check_x1 = (x_pos_ + width_frame_ / 2) / TILE_SIZE;
    int check_y = (y_pos_ + height_frame_ + 1) / TILE_SIZE;

    if (map_data.tile[check_y][check_x1] == 0)
    {
        on_ground_ = false;
    }
}

void MonsterObject::Show(SDL_Renderer* screen) {
    if (come_back_time_ == 0) {
        rect_.x = x_pos_ - map_x_;
        rect_.y = y_pos_ - map_y_;
        frame_++;
        if (frame_ >= Monster_FRAMES) {
            frame_ = 0;
        }

        SDL_Rect* currentClip = &frame_clip_[frame_];
        SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };
        if (currentClip != NULL && p_object_ != nullptr) {
            renderQuad.w = currentClip->w;
            renderQuad.h = currentClip->h;
            SDL_RenderCopy(screen, p_object_, currentClip, &renderQuad);
        }
    }
}

void MonsterObject::Initattack(Mainattack* p_attack, SDL_Renderer* screen)
{
    if (p_attack != NULL)
    {
        bool ret = p_attack->LoadImg("img//vu_khí _3.png", screen);
        if (ret)
        {
            p_attack->set_is_move(true);
            p_attack->set_attack_dir(Mainattack::DIR_LEFT);
            p_attack->SetRect(rect_.x + 5, rect_.y + 10);
            p_attack->set_x_val(15);
            attack_list_.push_back(p_attack);
        }
    }
}


void MonsterObject::Makeattack(SDL_Renderer* des, const int& x_limit, const int& y_limit)
{
    for (int i = 0; i < attack_list_.size(); i++)
    {
        Mainattack* p_attack = attack_list_.at(i);
        if (p_attack != NULL)
        {
            if (p_attack->get_is_move())
            {
                int distance_attack = rect_.x + width_frame_ - p_attack->GetRect().x;
                if (distance_attack > 0 && distance_attack < 400)
                {
                    p_attack->HandleMove(x_limit, y_limit);
                    p_attack->Render(des);
                }
                else
                {
                    p_attack->set_is_move(false);
                }
            }
            else
            {
                p_attack->set_is_move(true);
                p_attack->SetRect(rect_.x + 5, rect_.y + 10);
            }
        }
    }
}

void MonsterObject::Removeattack(const int& idx)
{
    if (attack_list_.size() > 0 && idx < attack_list_.size())
    {
        Mainattack* p_attack = attack_list_.at(idx);
        attack_list_.erase(attack_list_.begin() + idx);

        if (p_attack)
        {
            delete p_attack;
            p_attack = NULL;
        }
    }
}