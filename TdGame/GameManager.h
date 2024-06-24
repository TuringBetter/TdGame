#pragma once
#include "Manager.h"
#include "ConfigManager.h"
#include "ResourceManager.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>

class GameManager :
    public Manager<GameManager>
{
    /*
     * 为什么要申明一个友元
     * 因为基类_instance=std::shared_ptr<T>(new T)会调用HttpMgr的构造函数
     * 因此要将 Manager<GameManager>声明为友元
    */
    friend class Manager<GameManager>;
public:
    int run(int argc,char** argv);

public:
    /*
     * 为什么析构要公有？
     * 因为基类中的static std::shared_ptr<T> _instance在析构的时候
     * 调用的是智能指针模板类的析构
     * 最后要调用T类型的析构
    **/
    ~GameManager();
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* render = nullptr;

    /* 新的画布 */
    SDL_Texture* tex_tile_map = nullptr;

    SDL_Event event;
    bool is_quit = false;
private:
    GameManager();
    void init_assert(bool flag, const char* err_msg);
    void on_input();
    void on_update(double delta);
    void on_render();

    bool generate_tile_map_texture();
};

