#include "CommonFunc.h"
#include "BaseObject.h"
#include "map.h"
#include "MainObject.h"
#include "Timer.h"
#include "MonsterOBject.h"
#include "TextObject.h"
#include "MainPower.h"


BaseObject g_background;
Map game_map;
TTF_Font* font_time = NULL;

bool InitData()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0) return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("Chem Zombie",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (g_window == NULL)
    {
        success = false;
    }
    else
    {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen == NULL)
            success = false;
        else
        {
            SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) & imgFlags))
                success = false;
        }
        if (TTF_Init() == -1)
        {
            success = false;
        }
        font_time = TTF_OpenFont("font//times.ttf", 15);
        if (font_time == NULL)
        {
            success = false;
        }
    }
    return success;
}

void close()
{
    g_background.Free();

    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    IMG_Quit();
    SDL_Quit();
}

bool LoadBackground()
{
    bool ret = g_background.LoadImg("background.png", g_screen);
    if (!ret)
    {
        return false;
    }

    return true;
}

std::vector<MonsterObject*> MakeMonsterList()
{
    std::vector<MonsterObject*> list_monsters;
    MonsterObject* monster_objs = new MonsterObject[40];

    for (int i = 0; i < 40; i++)
    {
        MonsterObject* monster_obj = (monster_objs + i);
        if (monster_obj != NULL)
        {
            monster_obj->LoadImg("img//Orc_Attack02.png", g_screen);
            monster_obj->Setclips();
            monster_obj->set_x_pos(500 + i * 500);
            monster_obj->set_y_pos(200);
            Mainattack* p_attack = new  Mainattack();
            monster_obj->Initattack(p_attack, g_screen);
            list_monsters.push_back(monster_obj);
        }
    }

    return list_monsters;
}

int main(int argc, char* argv[])
{
    if (InitData() == false)
    {
        return -1;
    }

    if (!LoadBackground())
    {
        return -1;
    }
    TIMER fps_time;

    GameMap game_map;
    game_map.LoadMap("map//map01.dat");
    game_map.LoadMapTiles(g_screen);

    MainObject P_run;
    P_run.LoadImg("img//_Run.png", g_screen);
    P_run.SetClips();

    MainPower main_power;
    main_power.Init(g_screen);
    std::vector<MonsterObject*> monsters_list = MakeMonsterList();

    int num_die = 0;

    TextObject time_game;
    time_game.setColor(TextObject::WHITE_TEXT);

    TextObject mark_game;
    mark_game.setColor(TextObject::WHITE_TEXT);
    int mark_value = 0;

    bool quit = false;
    while (!quit)
    {
        fps_time.start();
        while (SDL_PollEvent(&g_event) != 0) {
            if (g_event.type == SDL_QUIT) {
                quit = true;
            }
            P_run.HandleInputAction(g_event, g_screen);
        }

        SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
        SDL_RenderClear(g_screen);

        g_background.Render(g_screen, NULL);

        Map map_data = game_map.GetMap();

        P_run.HandleAttack(g_screen);
        P_run.Show(g_screen);
        P_run.DoFalling(map_data);
        P_run.SetMapXY(map_data.start_x_, map_data.start_y_);

        game_map.DrawMap(g_screen);
        game_map.SetMap(map_data);

        main_power.Show(g_screen);

        for (int i = 0; i < monsters_list.size(); i++)
        {
            MonsterObject* obj_monster = monsters_list.at(i);
            obj_monster->SetMapXY(map_data.start_x_, map_data.start_y_);
            obj_monster->DoPlayer(map_data);
            obj_monster->Makeattack(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
            obj_monster->Show(g_screen);

            SDL_Rect rect_player = P_run.GetRectFrame();
            bool is_col1 = false;
            std::vector<Mainattack*> attack_list = obj_monster->get_attack_list();
            for (int am = 0; am < attack_list.size(); am++)
            {

                Mainattack* p_attack = attack_list.at(am);
                if (p_attack)
                {
                    is_col1 = SDLCommonFunc::CheckCollision(p_attack->GetRect(), rect_player);
                    if (is_col1 == true)
                    {
                        obj_monster->Removeattack(am);
                    }
                }
            }
            SDL_Rect rect_monster = obj_monster->GetRectFrame();
            bool is_col2 = SDLCommonFunc::CheckCollision(rect_player, rect_monster);
            if (is_col2 || is_col1)
            {
                num_die++;
                if (num_die <= 3)
                {
                    P_run.SetRect(0, 0);
                    P_run.set_come_back_time_(60);
                    SDL_Delay(1000);
                    main_power.Decrease();
                    main_power.Render(g_screen);
                    continue;
                }
                else
                {
                    if (MessageBox(NULL, L"GAME OVER", L"Info", MB_OK | MB_ICONSTOP) == IDOK)
                    {
                        obj_monster->Free();
                        close();
                        SDL_Quit();
                        return 0;
                    }
                }

            }

        }


        std::vector<Mainattack*> attack_arr = P_run.get_attack_list();
        for (int am = 0; am < attack_arr.size(); am++)
        {
            Mainattack* p_attack = attack_arr.at(am);
            if (p_attack)
            {
                for (int i = 0; i < monsters_list.size(); i++)
                {
                    MonsterObject* obj_monster = monsters_list.at(i);
                    SDL_Rect monster_rect;
                    monster_rect.x = obj_monster->GetRect().x;
                    monster_rect.y = obj_monster->GetRect().y;
                    monster_rect.w = obj_monster->get_width_frame();
                    monster_rect.h = obj_monster->get_height_frame();
                    bool is_col = SDLCommonFunc::CheckCollision(p_attack->GetRect(), monster_rect);
                    if (is_col)
                    {
                        mark_value++;
                        P_run.RemoveAttack(am);
                        obj_monster->Free();
                        monsters_list.erase(monsters_list.begin() + i);
                    }
                }
            }
        }

        std::string str_time = "Time: ";
        Uint32 time_val = SDL_GetTicks() / 1000;
        Uint32 val_time = 300 - time_val;

        if (val_time <= 0)
        {
            if (MessageBox(NULL, L"GAME OVER", L"Info", MB_OK | MB_ICONSTOP) == IDOK)
            {
                quit = true;
                break;
            }
        }
        else
        {
            std::string str_val = std::to_string(val_time);
            str_time += str_val;

            time_game.SetText(str_time);
            time_game.loadFromRenderedText(font_time, g_screen);
            time_game.RenderText(g_screen, SCREEN_WIDTH - 100, 15);
        }

        std::string val_str_mark = std::to_string(mark_value);
        std::string strMark("Mark: ");
        strMark += val_str_mark;

        mark_game.SetText(strMark);
        mark_game.loadFromRenderedText(font_time, g_screen);
        mark_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 50, 15);

        SDL_RenderPresent(g_screen);

        int real_time = fps_time.getTicks();
        int time_one_frame = 1000 / FRAMES_PER_SECOND;

        if (real_time < time_one_frame)
        {
            int delay_time = time_one_frame - real_time;
            if (delay_time >= 0) SDL_Delay(delay_time);
        }
    }

    for (int i = 0; i < monsters_list.size(); i++)
    {
        MonsterObject* monster_obj = monsters_list.at(i);
        monster_obj->Free();
        monster_obj = NULL;
    }

    monsters_list.clear();
    close();
    return 0;
}