#ifndef MONSTEROBJECT_H_INCLUDED
#define MONSTEROBJECT_H_INCLUDED

#include "CommonFunc.h"
#include "BaseObject.h"
#include "Map.h"
#include "Mainattack..h"


const int Monster_FRAMES = 6;
const int Monster_GRAVITY_SPEED = 1;
const int Monster_SPRITE_SIZE = 100;
const int Monster_MAX_FALL_SPEED = 10;

class MonsterObject : public BaseObject
{
public:
    MonsterObject();
    ~MonsterObject();



    void set_x_val(const int& xVal) { x_val_ = xVal; }
    void set_y_val(const int& yVal) { y_val_ = yVal; }

    void set_x_pos(const int& xPos) { x_pos_ = xPos; }
    void set_y_pos(const int& yPos) { y_pos_ = yPos; }
    float get_x_pos() const { return x_pos_; }
    float get_y_pos() const { return y_pos_; }

    void SetMapXY(const int map_x, const int map_y) { map_x_ = map_x, map_y_ = map_y; }
    void Setclips();
    bool LoadImg(std::string path, SDL_Renderer* screen);
    void Show(SDL_Renderer* screen);
    int get_width_frame() const { return width_frame_; }
    int get_height_frame() const { return height_frame_; }
    void CheckToMap(Map& map_data);
    void DoPlayer(Map& map_data);
    void CheckFalling(Map& map_data);

    std::vector<Mainattack*> get_attack_list() const { return attack_list_; }
    void set_attack_list(const std::vector<Mainattack*>& am_list) { attack_list_ = am_list; }
    void Initattack(Mainattack* p_attack, SDL_Renderer* screen);
    void Makeattack(SDL_Renderer* des, const int& x_limit, const int& y_limit);
    void Removeattack(const int& idx);
    SDL_Rect GetRectFrame();

private:
    SDL_Rect frame_clip_[Monster_FRAMES];
    bool on_ground_;
    int width_frame_;
    int height_frame_;
    int frame_;
    int come_back_time_;
    float x_pos_;
    float y_pos_;
    float x_val_;
    float y_val_;
    int map_x_;
    int map_y_;
    std::vector<Mainattack*> attack_list_;
};

#endif // MONSTEROBJECT_H_INCLUDED

