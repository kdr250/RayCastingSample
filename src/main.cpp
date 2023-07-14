#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
#include "Vec2.h"

struct Intersect
{
    bool result;
    Vec2 position;
};

Intersect lineIntersect(Vec2& a, Vec2& b, Vec2& c, Vec2& d)
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

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720),
                            "Ray Casting Sample",
                            sf::Style::Titlebar | sf::Style::None);

    std::vector<sf::Shape*> shapes;
    std::vector<Vec2> points;
    std::vector<std::pair<Vec2, Vec2>> lines;
    std::vector<Vec2> candidates;
    sf::VertexArray rayCasting(sf::Triangles, 90);

    sf::RectangleShape rect(sf::Vector2f(100.0f, 100.0f));
    rect.setFillColor(sf::Color::Blue);
    rect.setPosition(sf::Vector2f(600.0f, 100.0f));
    shapes.push_back(&rect);

    for (int i = 0; i < rect.getPointCount(); i++)
    {
        auto& point = rect.getTransform().transformPoint(rect.getPoint(i));
        points.push_back(Vec2(point.x, point.y));
    }

    for (int i = 1; i <= rect.getPointCount(); i++)
    {
        auto& pointA = rect.getTransform().transformPoint(rect.getPoint(i - 1));
        Vec2 a       = Vec2(pointA.x, pointA.y);
        auto& pointB = rect.getTransform().transformPoint(rect.getPoint(i % rect.getPointCount()));
        Vec2 b       = Vec2(pointB.x, pointB.y);
        lines.push_back(std::make_pair(a, b));
    }

    sf::CircleShape intersectPoint(3.0);
    intersectPoint.setFillColor(sf::Color::Red);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();

                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape)
                    {
                        window.close();
                    }
            }
        }

        window.clear(sf::Color::Black);

        window.draw(rect);

        auto& mousePosition = sf::Mouse::getPosition(window);
        Vec2 mousePos       = Vec2(mousePosition.x, mousePosition.y);

        for (auto& point : points)
        {
            Intersect first = {false, Vec2(0, 0)};
            for (auto& l : lines)
            {
                auto& intersect = lineIntersect(l.first, l.second, mousePos, point);
                if (intersect.result)
                {
                    if (first.result)
                    {
                        Vec2 intersectToMouse = mousePos - intersect.position;
                        Vec2 firstToMouse     = mousePos - first.position;
                        if (intersectToMouse.length() < firstToMouse.length())
                        {
                            first = intersect;
                        }
                    }
                    else
                    {
                        first = intersect;
                    }
                }
            }

            if (first.result)
            {
                Vec2& position = first.position;
                if (position == point)
                {
                    candidates.push_back(point);
                }
            }
        }

        std::sort(candidates.begin(),
                  candidates.end(),
                  [mousePos](Vec2& pos1, Vec2& pos2)
                  {
                      Vec2 pos1ToMouse = pos1 - mousePos;
                      Vec2 pos2ToMouse = pos2 - mousePos;
                      return std::atan2(pos1ToMouse.x, pos1ToMouse.y)
                             < std::atan2(pos2ToMouse.x, pos2ToMouse.y);
                  });

        rayCasting = sf::VertexArray(sf::Triangles, 90);

        for (int i = 0; i < candidates.size() - 1; i++)
        {
            Vec2 pos1 = candidates[i];
            Vec2 pos2 = candidates[(i + 1) % candidates.size()];

            rayCasting[i * 3 + 0].position = sf::Vector2f(pos1.x, pos1.y);
            rayCasting[i * 3 + 1].position = sf::Vector2f(pos2.x, pos2.y);
            rayCasting[i * 3 + 2].position = sf::Vector2f(mousePos.x, mousePos.y);

            rayCasting[i * 3 + 0].color = sf::Color::Yellow;
            rayCasting[i * 3 + 1].color = sf::Color::Yellow;
            rayCasting[i * 3 + 2].color = sf::Color::Yellow;
        }

        window.draw(rayCasting);

        candidates.clear();

        window.display();
    }

    return 0;
}
