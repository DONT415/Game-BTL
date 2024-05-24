#include "MainObject.h"

MainObject::MainObject()
{
    frame_ = 0;
    x_pos_ = 0;
    y_pos_ = 0;
    x_val_ = 0;
    y_val_ = 0;
    status_ = -1;
    width_frame_ = 0;
    height_frame_ = 0;
    on_ground_ = false;
    map_x_ = 0;
    map_y_ = 0;
    is_jumping_ = false;
    come_back_time_ = 0;

}

MainObject::~MainObject()
{
    Free();
}

SDL_Rect MainObject::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = rect_.w / 10;
    rect.h = rect_.h;
    return rect;
}

bool MainObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = this->BaseObject::LoadImg(path, screen);
    if (ret)
    {
        this->width_frame_ = rect_.w / 10;
        this->height_frame_ = SPRITE_SIZE;
    }
    return ret;
}

void MainObject::SetClips()
{
    if (width_frame_ > 0 && height_frame_ > 0)
    {
        for (int i = 0; i < 10; i++)
        {
            frame_clip_[i].x = width_frame_ * i;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }
    }
}

void MainObject::HandleInputAction(SDL_Event events, SDL_Renderer* screen)
{
    if (events.type == SDL_KEYDOWN)
    {
        switch (events.key.keysym.sym)
        {
        case SDLK_RIGHT:
            status_ = WALK_RIGHT;
            input_type_.right_ = 1;

            break;
        case SDLK_LEFT:
            status_ = WALK_LEFT;
            input_type_.left_ = 1;

            break;
        case SDLK_UP:
            if (on_ground_)
            {
                is_jumping_ = true;
                on_ground_ = false;
            }
            break;
        }
    }
    else if (events.type == SDL_KEYUP)
    {
        switch (events.key.keysym.sym)
        {
        case SDLK_RIGHT:
            input_type_.right_ = 0;
            break;
        case SDLK_LEFT:
            input_type_.left_ = 0;
            break;
        }
    }
    if (events.type == SDL_MOUSEBUTTONDOWN)
    {
        if (events.button.button == SDL_BUTTON_LEFT)
        {
            Mainattack* p_attack = new Mainattack();
            

            if (status_ == WALK_LEFT)
            {
                p_attack->set_attack_dir(Mainattack::DIR_LEFT);
                p_attack->SetRect(this->rect_.x, this->rect_.y + height_frame_ * 0.55);
                p_attack->set_x_val(-20);
                p_attack->LoadImg("img//vu_khi1.png", screen);
            }
            else
            {
                p_attack->set_attack_dir(Mainattack::DIR_RIGHT);
                p_attack->SetRect(this->rect_.x + width_frame_ - 20, this->rect_.y + height_frame_ * 0.55);
                p_attack->set_x_val(20);
                p_attack->LoadImg("img//vu_khi.png", screen);
            }
            
            p_attack->set_is_move(true);
            p_attack_list.push_back(p_attack);
        }
    }
}


void MainObject::HandleAttack(SDL_Renderer* des)
{
    for (int i = 0; i < p_attack_list.size(); i++)
    {
        Mainattack* p_attack = p_attack_list.at(i);
        if (p_attack != NULL)
        {
            if (p_attack->get_is_move() == true)
            {
                p_attack->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
                p_attack->Render(des);
            }
            else
            {
                p_attack_list.erase(p_attack_list.begin() + i);
                if (p_attack != NULL) {
                    delete p_attack;
                    p_attack = NULL;
                }
            }
        }
    }
}

void MainObject::RemoveAttack(const int& idx)
{
    if (p_attack_list.size() > 0 && idx < p_attack_list.size())
    {
       Mainattack* p_attack = p_attack_list.at(idx);
       p_attack_list.erase(p_attack_list.begin() + idx);

        if (p_attack)
        {
            delete p_attack;
            p_attack = NULL;
        }
    }
}
void MainObject::Show(SDL_Renderer* des)
{
    if (status_ == WALK_LEFT)
    {
        LoadImg("img//_Run1.png", des);
    }
    else if (status_ == WALK_RIGHT)
    {
        LoadImg("img//_Run.png", des);
    }

    if (input_type_.left_ == 1 || input_type_.right_ == 1)
    {
        frame_++;
    }
    else
    {
        frame_ = 0;
    }

    if (frame_ >= 10)
    {
        frame_ = 0;
    }

    if (come_back_time_ == 0)
    {
        rect_.x = x_pos_ - map_x_;
        rect_.y = y_pos_ - map_y_;

        SDL_Rect* current_clip = &frame_clip_[frame_];
        SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };
        SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
    }
}

void MainObject::DoFalling(Map& map_data)
{
    if (come_back_time_ == 0)
    {
        x_val_ = 0;
        if (!on_ground_)
        {
            y_val_ += GRAVITY_SPEED;
        }

        if (is_jumping_ && on_ground_)
        {
            y_val_ -= JUMP_SPEED;
            is_jumping_ = false;
            on_ground_ = false;
        }

        if (input_type_.left_ == 1)
        {
            x_val_ -= MAIN_SPEED;
        }
        else if (input_type_.right_ == 1)
        {
            x_val_ += MAIN_SPEED;
        }

        CheckToMap(map_data);
        CenterEntityOnMap(map_data);
    }

    if (come_back_time_ > 0)
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
}

void MainObject::CenterEntityOnMap(Map& map_data)
{
    map_data.start_x_ = x_pos_ - (SCREEN_WIDTH / 3);
    if (map_data.start_x_ < 0)
    {
        map_data.start_x_ = 0;
    }
    else if (map_data.start_x_ + SCREEN_WIDTH >= map_data.max_x_)
    {
        map_data.start_x_ = map_data.max_x_ - SCREEN_WIDTH;
    }

    map_data.start_y_ = y_pos_ - (SCREEN_HEIGHT / 3);
    if (map_data.start_y_ < 0)
    {
        map_data.start_y_ = 0;
    }
    else if (map_data.start_y_ + SCREEN_HEIGHT >= map_data.max_y_)
    {
        map_data.start_y_ = map_data.max_y_ - SCREEN_HEIGHT;
    }
}

void MainObject::CheckToMap(Map& map_data)
{
    int x1 = 0;
    int x2 = 0;

    int y1 = 0;
    int y2 = 0;

    int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;
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

    int width_min_ = width_frame_ < TILE_SIZE ? width_frame_ : TILE_SIZE;
    x1 = (x_pos_) / TILE_SIZE;
    x2 = (x_pos_ + width_min_) / TILE_SIZE;

    y1 = (y_pos_ + y_val_) / TILE_SIZE;
    y2 = (y_pos_ + y_val_ + height_frame_ - 1) / TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if (y_val_ > 0)
        {
            if (map_data.tile[y2][x1] != 0 || map_data.tile[y2][x2] != 0)
            {
                y_pos_ = y2 * TILE_SIZE;
                y_pos_ -= (height_frame_ + 1);
                y_val_ = 0;
                on_ground_ = true;
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
        x_pos_ = map_data.max_x_ + width_frame_ - 1;
    }

    CheckFalling(map_data);

    if (y_pos_ > map_data.max_y_)
    {
        come_back_time_ = 30;
    }
}

void MainObject::CheckFalling(Map& map_data)
{
    int check_x1 = (x_pos_ + width_frame_ / 2) / TILE_SIZE; int check_y = (y_pos_ + height_frame_ + 1) / TILE_SIZE;

    if (map_data.tile[check_y][check_x1] == 0)
    {
        on_ground_ = false;
    }
}


