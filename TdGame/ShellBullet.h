#pragma once
#include "Bullet.h"
#include "ResourceManager.h"
class ShellBullet :
    public Bullet
{
public:
    ShellBullet();
    ~ShellBullet() = default;

    /*
    * �����û��ը�����ո������
    * �����������ı�ը�߼�
    */
    void on_render(SDL_Renderer* render) override;
    void on_update(double delta) override;
    void on_collide(Enemy* enemy) override;
private:
    Animation animation_explode;
};

