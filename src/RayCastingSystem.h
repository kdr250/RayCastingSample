#pragma once
#include "Vec2.h"

struct Intersect
{
    bool result;
    Vec2 position;
};

class RayCastingSystem
{
public:
    static Intersect lineIntersect(Vec2& a, Vec2& b, Vec2& c, Vec2& d)
    {
        Vec2 r    = b - a;
        Vec2 s    = d - c;
        float rxs = r.crossProduct(s);
        Vec2 cma  = c - a;
        float t   = cma.crossProduct(s) / rxs;
        float u   = cma.crossProduct(r) / rxs;
        if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
        {
            return {true, Vec2(a.x + t * r.x, a.y + t * r.y)};
        }
        else
        {
            return {false, Vec2(0.0f, 0.0f)};
        }
    }
};
