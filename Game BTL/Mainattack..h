#pragma once
#ifndef MAINATTACK_H
#define MAINATTACK_H

#include "BaseObject.h"
#include "CommonFunc.h"

class Mainattack : public BaseObject
{
public:
    Mainattack();
    ~Mainattack();

    enum AttackDir
    {
        DIR_RIGHT = 20,
        DIR_LEFT = 21,
    };

    enum AttackType
    {
        Attack1 = 50,
        Attack2 = 51,

    };

    void set_x_val(const int& x_val) { x_val_ = x_val; }
    void set_y_val(const int& y_val) { y_val_ = y_val; }
    int get_x_val() const { return x_val_; }
    int get_y_val() const { return y_val_; }
    void set_is_move(const bool& is_move) { is_move_ = is_move; }
    bool get_is_move()const { return is_move_; }
    void HandleMove(const int& x_boder_, const int& y_boder);
    void set_attack_dir(const int& AttackDir) { attack_dir = AttackDir; }
    int get_attack_dir() const { return attack_dir; }
    void set_attack_type(const int& attackTyle) { attack_type = attackTyle; }
    int set_attack_type() const { return attack_type; }
    bool LoadAttack(SDL_Renderer* des);
private:
    int x_val_;
    int y_val_;
    bool is_move_;
    int attack_dir;
    int attack_type;

};

#endif // 
