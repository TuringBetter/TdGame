#pragma once
#include "Banner.h"
#include "PlayerManager.h"
#include "Manager.h"
#include "ConfigManager.h"
#include "ResourceManager.h"
#include "WaveManager.h"
#include "EnemyManager.h"
#include "TowerManager.h"
#include "EnemyManager.h"
#include "StatusBar.h"
#include "Panel.h"
#include "UpgradePanel.h"
#include "PlacePanel.h"

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

    StatusBar status_bar;

    Panel* place_panel = nullptr;
    Panel* upgrade_panel = nullptr;

    Banner* banner = nullptr;
private:
    GameManager();
    void init_assert(bool flag, const char* err_msg);
    void on_input();
    void on_update(double delta);
    void on_render();

    bool generate_tile_map_texture();

    bool get_cursor_idx_tile(SDL_Point& idx_tile_selected,int screen_x,int screen_y);
    bool can_place_tower(const SDL_Point& idx_tile_selected)const;
    void get_selected_tile_center_pos(SDL_Point& pos, const SDL_Point& idx_tile_selected);
    bool check_home(const SDL_Point& idx_tile_selected);
};

