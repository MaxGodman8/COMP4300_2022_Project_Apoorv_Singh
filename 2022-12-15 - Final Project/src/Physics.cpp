
#include "Physics.h"
#include "Components.h"
using namespace std;

Vec2 Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    auto& aTransform = a->getComponent<CTransform>();
    auto& bTransform = b->getComponent<CTransform>();
    auto& aBox = a->getComponent<CBoundingBox>();
    auto& bBox = b->getComponent<CBoundingBox>();
    int differenceX = abs(aTransform.pos.x - bTransform.pos.x);
    int differenceY = abs(aTransform.pos.y - bTransform.pos.y);
    int x = aBox.halfSize.x + bBox.halfSize.x - differenceX;
    int y = aBox.halfSize.y + bBox.halfSize.y - differenceY;

    return Vec2(x, y);
}
                                            
Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    auto& aTransform = a->getComponent<CTransform>();
    auto& bTransform = b->getComponent<CTransform>();
    auto& aBox = a->getComponent<CBoundingBox>();
    auto& bBox = b->getComponent<CBoundingBox>();
    int dx = abs(aTransform.prevPos.x - bTransform.prevPos.x);
    int dy = abs(aTransform.prevPos.y - bTransform.prevPos.y);
    int x = aBox.halfSize.x + bBox.halfSize.x - dx;
    int y = aBox.halfSize.y + bBox.halfSize.y - dy;

    return Vec2(x, y);
}

bool Physics::IsInside(const Vec2& pos, std::shared_ptr<Entity> e)
{                                           
    return false;
}

bool onSegment(const Vec2& p, const Vec2& q, const Vec2& r)
{
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
        return true;

    return false;
}

Intersect Physics::LineIntersect(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& d)
{

    bool isIntersect = false;

    int o1 = orientation(a, b, d);
    int o2 = orientation(a, b, c);
    int o3 = orientation(c, d, a);
    int o4 = orientation(c, d, b);

    if (o1 != o2 && o3 != o4) {
        isIntersect = true;
    }


    if (o1 == 0 && onSegment(a, c, b)) { isIntersect = true; }

    // p1, q1 and q2 are collinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(a, d, b)) { isIntersect = true; }

    // p2, q2 and p1 are collinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(c, a, d)) { isIntersect = true; }

    // p2, q2 and q1 are collinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(c, b, d)) { isIntersect = true; }

    


    return { isIntersect, Vec2(0,0) };
}



int Physics::orientation(const Vec2& a, const Vec2& b, const Vec2& c) {

    int value = (b.y - a.y) * (c.x - b.x) - (c.y - b.y) * (b.x - a.x);

    if (value == 0)return 0;

    if (value > 0) {
        return 1;
    }
    else {
        return -1;
    }

}

bool Physics::EntityIntersect(const Vec2& a, const Vec2& b, std::shared_ptr<Entity> e)
{


    CBoundingBox& box = e->getComponent<CBoundingBox>();
    CTransform& transform = e->getComponent<CTransform>();

    Vec2 p1(transform.pos.x - box.halfSize.x, transform.pos.y - box.halfSize.y);

    Vec2 p2(transform.pos.x + box.halfSize.x, transform.pos.y - box.halfSize.y);

    Vec2 p3(transform.pos.x + box.halfSize.x, transform.pos.y + box.halfSize.y);

    Vec2 p4(transform.pos.x - box.halfSize.x, transform.pos.y + box.halfSize.y);

    

    if (LineIntersect(a, b, p1, p2).result) { return true; }

    if (LineIntersect(a, b, p2, p3).result) { return true; }

    if (LineIntersect(a, b, p3, p4).result) { return true; }

    if (LineIntersect(a, b, p1, p4).result) { return true; }

    return false;
}
                                            