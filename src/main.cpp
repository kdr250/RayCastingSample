#include <SFML/Graphics.hpp>
#include <vector>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720),
                            "Ray Casting Sample",
                            sf::Style::Titlebar | sf::Style::None);

    std::vector<sf::Shape*> shapes;

    /*sf::CircleShape triangle(50.0f, 3);
    triangle.setFillColor(sf::Color::Blue);
    triangle.setPosition(sf::Vector2f(100.0f, 100.0f));
    shapes.push_back(&triangle);*/

    sf::RectangleShape rect(sf::Vector2f(100.0f, 100.0f));
    rect.setFillColor(sf::Color::Blue);
    rect.setPosition(sf::Vector2f(600.0f, 100.0f));
    shapes.push_back(&rect);

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

        window.display();
    }

    return 0;
}
