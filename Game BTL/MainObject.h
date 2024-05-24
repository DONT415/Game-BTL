#ifndef MAINOBJECT_H_INCLUDED
#define MAINOBJECT_H_INCLUDED

#include "Commonfunc.h"
#include "BaseObject.h"
#include "Mainattack..h"
#include <vector>


const int MAIN_SPEED = 8;

class MonsterObject;

class MainObject : public BaseObject {
public:
    MainObject();
    ~MainObject();

    enum WalkType {
        WALK_NONE = 0,
        WALK_RIGHT = 1,
        WALK_LEFT = 2,
        ATTACK_LEFT = 4,
        ATTACK_RIGHT = 3,
    };

    bool LoadImg(std::string path, SDL_Renderer* screen);
    void HandleInputAction(SDL_Event events, SDL_Renderer* screen);
    void SetClips();
    void Show(SDL_Renderer* des);
    void DoFalling(Map& map_data);
    void CheckToMap(Map& map_data);
    void SetMapXY(const int map_x, const int map_y) { map_x_ = map_x; map_y_ = map_y; }
    void CenterEntityOnMap(Map& map_data);
    void CheckFalling(Map& map_data);
    SDL_Rect GetRect() const { return rect_; }
    void set_attack_list(std::vector<Mainattack*> attack_list)
    {
        p_attack_list = attack_list;
    }
    std::vector<Mainattack*> get_attack_list() const { return p_attack_list; }

    void HandleAttack(SDL_Renderer* des);
    void RemoveAttack(const int& idx);
    SDL_Rect GetRectFrame();
    void set_come_back_time_(const int& cb_time) { come_back_time_ = cb_time; }
private:
    std::vector<Mainattack*> p_attack_list;
    SDL_Rect frame_clip_[10];
    Input input_type_;
    int frame_;
    int x_pos_;
    int y_pos_;
    int x_val_;
    int y_val_;
    int status_;
    int width_frame_;
    int height_frame_;
    bool on_ground_;
    int map_x_;
    int map_y_;
    bool is_jumping_;
    int come_back_time_;
};

#endif // MAINOBJECT_H_INCLUDED
