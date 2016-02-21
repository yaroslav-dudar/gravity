#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML and Box2d works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    b2Vec2 Gravity(0.f, 9.8f);
    b2World World(Gravity);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}