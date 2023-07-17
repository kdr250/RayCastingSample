#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <memory>
#include <utility>
#include <vector>
#include "RayCastingSystem.h"
#include "Vec2.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720),
                            "Ray Casting Sample",
                            sf::Style::Titlebar | sf::Style::None);

    std::vector<std::shared_ptr<sf::Shape>> shapes;
    std::vector<Vec2> points;
    std::vector<std::pair<Vec2, Vec2>> lines;
    std::vector<Vec2> rotatedLines;
    std::vector<Vec2> candidates;
    sf::VertexArray rayCasting(sf::Triangles, 900);

    std::shared_ptr<sf::RectangleShape> rect =
        std::make_shared<sf::RectangleShape>(sf::Vector2f(100.0f, 100.0f));
    rect->setFillColor(sf::Color::Blue);
    rect->setPosition(sf::Vector2f(600.0f, 100.0f));
    std::shared_ptr<sf::Shape> rectShape = std::dynamic_pointer_cast<sf::Shape>(rect);
    shapes.push_back(rectShape);

    std::shared_ptr<sf::ConvexShape> convex = std::make_shared<sf::ConvexShape>();
    convex->setPointCount(5);
    convex->setPoint(0, sf::Vector2f(300.0f, 600.0f));
    convex->setPoint(1, sf::Vector2f(450.0f, 610.0f));
    convex->setPoint(2, sf::Vector2f(420.0f, 680.0f));
    convex->setPoint(3, sf::Vector2f(330.0f, 700.0f));
    convex->setPoint(4, sf::Vector2f(300.0f, 650.0f));
    convex->setFillColor(sf::Color::Blue);
    std::shared_ptr<sf::Shape> convexShape = std::dynamic_pointer_cast<sf::Shape>(convex);
    shapes.push_back(convexShape);

    std::shared_ptr<sf::CircleShape> triangle = std::make_shared<sf::CircleShape>(50.0f, 3);
    triangle->setFillColor(sf::Color::Blue);
    triangle->setPosition(sf::Vector2f(100.0f, 100.0f));
    triangle->setRotation(30.0f);
    std::shared_ptr<sf::Shape> triangleShape = std::dynamic_pointer_cast<sf::Shape>(triangle);
    shapes.push_back(triangleShape);

    Vec2 windowTopLeft     = Vec2(window.getSize().x * 0, window.getSize().y * 0);
    Vec2 windowTopRight    = Vec2(window.getSize().x * 1, window.getSize().y * 0);
    Vec2 windowBottomRight = Vec2(window.getSize().x * 1, window.getSize().y * 1);
    Vec2 windowBottomLeft  = Vec2(window.getSize().x * 0, window.getSize().y * 1);

    points.push_back(windowTopLeft);
    points.push_back(windowTopRight);
    points.push_back(windowBottomRight);
    points.push_back(windowBottomLeft);

    lines.push_back(std::make_pair(windowTopLeft, windowTopRight));
    lines.push_back(std::make_pair(windowTopRight, windowBottomRight));
    lines.push_back(std::make_pair(windowBottomRight, windowBottomLeft));
    lines.push_back(std::make_pair(windowBottomLeft, windowTopLeft));

    for (auto& shape : shapes)
    {
        for (int i = 0; i < shape->getPointCount(); i++)
        {
            sf::Vector2f point = shape->getTransform().transformPoint(shape->getPoint(i));
            points.push_back(Vec2(point.x, point.y));
        }

        for (int i = 1; i <= shape->getPointCount(); i++)
        {
            sf::Vector2f pointA = shape->getTransform().transformPoint(shape->getPoint(i - 1));
            Vec2 a              = Vec2(pointA.x, pointA.y);
            sf::Vector2f pointB =
                shape->getTransform().transformPoint(shape->getPoint(i % shape->getPointCount()));
            Vec2 b = Vec2(pointB.x, pointB.y);
            lines.push_back(std::make_pair(a, b));
        }
    }

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

        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        Vec2 mousePos              = Vec2(mousePosition.x, mousePosition.y);

        for (auto& point : points)
        {
            Intersect first = {false, Vec2(0, 0)};
            for (auto& l : lines)
            {
                Intersect intersect =
                    RayCastingSystem::lineIntersect(l.first, l.second, mousePos, point);
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

                    if (point != windowTopLeft && point != windowTopRight
                        && point != windowBottomRight && point != windowBottomLeft)
                    {
                        float maxWindowLength = std::max(window.getSize().x, window.getSize().y);
                        Vec2 mouseToPoint     = position - mousePos;

                        Vec2 plus =
                            mousePos.rotate(1.0).normalize().multiply(maxWindowLength + 100000.0f);
                        rotatedLines.push_back(plus);

                        Vec2 minus =
                            mousePos.rotate(-1.0).normalize().multiply(maxWindowLength + 10000.0f);
                        rotatedLines.push_back(minus);
                    }
                }
            }
        }

        for (auto& point : rotatedLines)
        {
            Intersect first = {false, Vec2(0, 0)};
            for (auto& l : lines)
            {
                Intersect intersect =
                    RayCastingSystem::lineIntersect(l.first, l.second, mousePos, point);
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
                candidates.push_back(first.position);
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

        rayCasting = sf::VertexArray(sf::Triangles, 900);

        for (int i = 0; i < candidates.size(); i++)
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

        for (auto& shape : shapes)
        {
            window.draw(*shape);
        }

        window.draw(rayCasting);

        candidates.clear();
        rotatedLines.clear();

        window.display();
    }

    return 0;
}
