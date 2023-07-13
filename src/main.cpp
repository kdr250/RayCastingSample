#include <SFML/Graphics.hpp>
#include <iostream>
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

    sf::Vector2f vec;

    /*sf::CircleShape triangle(50.0f, 3);
    triangle.setFillColor(sf::Color::Blue);
    triangle.setPosition(sf::Vector2f(100.0f, 100.0f));
    shapes.push_back(&triangle);*/

    sf::RectangleShape rect(sf::Vector2f(100.0f, 100.0f));
    rect.setFillColor(sf::Color::Blue);
    rect.setPosition(sf::Vector2f(600.0f, 100.0f));
    shapes.push_back(&rect);

    std::cout << rect.getTransform().transformPoint(rect.getPoint(0)).x << " , "
              << rect.getTransform().transformPoint(rect.getPoint(0)).y << std::endl;
    std::cout << rect.getTransform().transformPoint(rect.getPoint(1)).x << " , "
              << rect.getTransform().transformPoint(rect.getPoint(1)).y << std::endl;
    std::cout << rect.getTransform().transformPoint(rect.getPoint(2)).x << " , "
              << rect.getTransform().transformPoint(rect.getPoint(2)).y << std::endl;
    std::cout << rect.getTransform().transformPoint(rect.getPoint(3)).x << " , "
              << rect.getTransform().transformPoint(rect.getPoint(3)).y << std::endl;

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
        window.draw(line);

        Vec2 a(rect.getTransform().transformPoint(rect.getPoint(2)).x,
               rect.getTransform().transformPoint(rect.getPoint(2)).y);
        Vec2 b(rect.getTransform().transformPoint(rect.getPoint(3)).x,
               rect.getTransform().transformPoint(rect.getPoint(3)).y);

        Vec2 c(window.getSize().x / 2.0, window.getSize().y / 2.0);
        Vec2 d(window.getSize().x / 2.0, 0.0);

        auto& intersect = lineIntersect(a, b, c, d);
        if (intersect.result)
        {
            Vec2& position = intersect.position;
            intersectPoint.setPosition(sf::Vector2f(position.x, position.y));
            window.draw(intersectPoint);
        }

        window.display();
    }

    return 0;
}
