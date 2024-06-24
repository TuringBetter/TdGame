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
     * ΪʲôҪ����һ����Ԫ
     * ��Ϊ����_instance=std::shared_ptr<T>(new T)�����HttpMgr�Ĺ��캯��
     * ���Ҫ�� Manager<GameManager>����Ϊ��Ԫ
    */
    friend class Manager<GameManager>;
public:
    int run(int argc,char** argv);

public:
    /*
     * Ϊʲô����Ҫ���У�
     * ��Ϊ�����е�static std::shared_ptr<T> _instance��������ʱ��
     * ���õ�������ָ��ģ���������
     * ���Ҫ����T���͵�����
    **/
    ~GameManager();
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* render = nullptr;

    /* �µĻ��� */
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

