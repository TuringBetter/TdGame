#pragma once
#include "Manager.h"
#include "BulletType.h"
#include "Bullet.h"
#include "ArrowBullet.h"
#include "AxeBullet.h"
#include "ShellBullet.h"
#include <vector>
class BulletManager :
    public Manager<BulletManager>
{
    friend class Manager<BulletManager>;
public:
    typedef std::vector<Bullet*> BulletList;
public:
    ~BulletManager();

    void on_update(double delta);
    void on_render(SDL_Renderer* render);

    BulletList& get_bullet_list();
    void fire_bullet(BulletType type,const Vector2& position,const Vector2& velovity, double damage);

private:
    BulletList bullet_list;
private:
    BulletManager()=default;
};

