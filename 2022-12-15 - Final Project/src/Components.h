                                          
#pragma once
                                            
#include "Animation.h"
#include "Assets.h"


class Component
{
public:
    bool has = false;
};

class CTransform : public Component
{
public:
    Vec2 pos = { 0.0, 0.0 };
    Vec2 prevPos = { 0.0, 0.0 };
    Vec2 scale = { 1.0, 1.0 };
    Vec2 velocity = { 0.0, 0.0 };
    Vec2 facing = { 0.0, 1.0 };
    float angle = 0;
    Vec2 iniPos = { 0,0 };
    Vec2 tilePos = { 0,0 };
    Vec2 roomPos = { 0,0 };
    std::string animName = "";

    CTransform() {}
    CTransform(const Vec2& p)
        : pos(p) {}
    CTransform(const Vec2& p, const Vec2& sp, const Vec2& sc, float a)
        : pos(p), prevPos(p), velocity(sp), scale(sc), angle(a) {}

};
                                            
class CLifeSpan : public Component
{
public:
    int lifespan = 0;
    int frameCreated = 0;
    CLifeSpan() {}
    CLifeSpan(int duration, int frame)
        : lifespan(duration), frameCreated(frame) {}
};

class CPingPongMovement : public Component {

public:

    float maxDistance;
    float speed = 0;
    bool m_tileup = false;
    
    Vec2 centerPos = { 0, 0 };
    CPingPongMovement() {}
    CPingPongMovement(float dis, float s) : maxDistance(dis), speed(s) {}

};

class CSelectable : public Component {
public:

    bool Selected = false;
    int Amount = 0;
    int Amount2 = 0;

    

    CSelectable(){}

};

class CSensor : public Component {
public:

    int num = 0;
    bool Active = false;

    CSensor() {}

};

class CBullet : public Component {

public:

    int damage = 1;
    int speed = 10;
    bool projectile = false;
    int speedOffset = 0;
    bool offset = true;
    int FrameActive = 0;

    CBullet(int d, int s):damage(d), speed(s){}
    CBullet(int d, int s, bool pro, bool pos) :damage(d), speed(s), projectile(pro), offset(pos) {}
    CBullet(){}

};

class CSmart : public Component {

public:

    int FrameShot = 0;
    int BehaviourChange = 0;

    CSmart(){}


};

class CGrabbable : public Component {
public:

    bool PlayerCarrying = false;
    bool OnGround = false;

    bool pink = false;
    bool green = false;
    bool white = false;
    
    CGrabbable(){}


};


class CInventory : public Component {
public:

    bool MainInventory = false;
    
    bool Arrow = false;
    bool Stone = false;
    bool Fireball = false;
    
    bool Visible = false;

    CInventory(){}

    CInventory(bool main, bool arrow, bool stone, bool fire) : MainInventory(main), Arrow(arrow),
        Stone(stone), Fireball(fire) {}

};

class CFloorButton :public Component {

public:

    
    std::string EntityName = "-1";
    Vec2 OriginalEntityPos = { -1,-1 };
    bool Colliding = false;

    CFloorButton() {}

};

class CButtonControlled :public Component {

public:

    CButtonControlled() {}

};

class CCollectible : public Component {

public:
    
    int Num = 10;
    
    CCollectible(int n):Num(n){}
    CCollectible(){}

};



class CButton :public Component {

public:

    std::string buttonName;
    std::string EntityName = "-1";
    Vec2 OriginalEntityPos = { -1,-1 };
    bool RightArrow = false;
    bool LeftArrow = false;
    bool LeftArrow2 = false;
    bool RightArrow2 = false;
    bool Blade = false;

    CButton(std::string n) : buttonName(n) {}
    CButton() {}

};

class CRotator : public Component {

public:

    float rotatingSpeed;

    CRotator() {}

    CRotator(float speed) : rotatingSpeed(speed) {}

};

class CDraggable : public Component {

public:

    bool Dragging = false;
    CDraggable() {}

};

class CDamage : public Component
{
public:
    int damage = 1;
    CDamage() {}
    CDamage(int d)
        : damage(d) {}
};

class CInvincibility : public Component
{
public:
    int iframes = 0;
    CInvincibility() {}
    CInvincibility(int f)
        : iframes(f) {}
};

class CHealth : public Component
{
public:
    int max = 1;
    int current = 1;
    bool Hit = false;
    int frameHit = 0;
    bool playerCarrying = false;
    CHealth() {}
    CHealth(int m, int c)
        : max(m), current(c) {}
};

class CInput : public Component
{
public:
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool attack = false;

    CInput() {}
};

class CBoundingBox : public Component
{
public:
    Vec2 size;
    Vec2 halfSize;
    bool blockMove = false;
    bool blockVision = false;
    CBoundingBox() {}
    CBoundingBox(const Vec2& s)
        : size(s), halfSize(s.x / 2, s.y / 2) {}
    CBoundingBox(const Vec2& s, bool m, bool v)
        : size(s), blockMove(m), blockVision(v), halfSize(s.x / 2, s.y / 2) {}
};

class CAnimation : public Component
{
public:
    Animation animation;
    bool repeat = false;
    CAnimation() {}
    CAnimation(const Animation& animation, bool r)
        : animation(animation), repeat(r) {}
};

class CState : public Component
{
public:
    std::string state = "stand";
    CState() {}
    CState(const std::string& s) : state(s) {}
};

class CFollowPlayer : public Component
{
public:
    Vec2 home = { 0, 0 };
    float speed = 0;
 
    CFollowPlayer() {}
    CFollowPlayer(Vec2 p, float s)
        : home(p), speed(s) {}

};

class CPatrol : public Component
{
public:
    std::vector<Vec2> positions;
    size_t currentPosition = 0;
    float speed = 0;
    
    int LastShotFrame = 0;
    CPatrol() {}
    CPatrol(std::vector<Vec2>& pos, float s) : positions(pos), speed(s) {}
};

