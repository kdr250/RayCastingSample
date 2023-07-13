#include <SFML/Graphics.hpp>
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

    /*sf::CircleShape triangle(50.0f, 3);
    triangle.setFillColor(sf::Color::Blue);
    triangle.setPosition(sf::Vector2f(100.0f, 100.0f));
    shapes.push_back(&triangle);*/

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

    /*sf::ConvexShape convex;
    convex.setPointCount(5);
    convex.setPoint(0, sf::Vector2f(300.0f, 600.0f));
    convex.setPoint(1, sf::Vector2f(450.0f, 610.0f));
    convex.setPoint(2, sf::Vector2f(420.0f, 680.0f));
    convex.setPoint(3, sf::Vector2f(330.0f, 700.0f));
    convex.setPoint(4, sf::Vector2f(300.0f, 650.0f));
    convex.setFillColor(sf::Color::Blue);
    shapes.push_back(&convex);*/

    sf::RectangleShape line(sf::Vector2f(window.getSize().y / 2.0f, 5.0f));
    line.setPosition(window.getSize().x / 2, window.getSize().y / 2);
    line.rotate(270.0f);
    line.setFillColor(sf::Color::Yellow);

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

        /*for (auto& shape : shapes)
        {
            window.draw(*shape);
        }*/

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
                intersectPoint.setPosition(sf::Vector2f(position.x, position.y));
                window.draw(intersectPoint);
            }
        }

        window.display();
    }

    return 0;
}
