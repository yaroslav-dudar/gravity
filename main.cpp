#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include <iostream>

void createGround(b2World& world, float x, float y)
{
    b2BodyDef rectBodyDef;
    rectBodyDef.type = b2_staticBody;
    rectBodyDef.position.Set(x, y);

    b2Body* body = world.CreateBody(&rectBodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(800.f, 20.f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 0.0f;

    body->CreateFixture(&fixtureDef);
}

void createBox(b2World& world, float x, float y)
{
    b2BodyDef rectBodyDef;
    rectBodyDef.type = b2_dynamicBody; //this will be a dynamic body
    rectBodyDef.position.Set(x, y); //set the starting position
    rectBodyDef.angle = 0; //set the starting angle

    b2Body* body = world.CreateBody(&rectBodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(20.f, 20.f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.density = 1.0f;
    // Override the default friction.
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.4f;

    body->CreateFixture(&fixtureDef);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML and Box2d");

    b2Vec2 gravity(0.f, 9.8f);
    b2World* world = new b2World(gravity);
    std::cout << world;

    createGround(*world, 0, 580);
    createBox(*world, 100.f, 100.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        /** Simulate the world */
        world->Step(1/60.f, 8, 3);

        window.clear();
        /** Iterate over box2d bodies and draw them */
        for (b2Body* body = world->GetBodyList(); body != 0; body = body->GetNext())
        {
            if (body->GetType() == b2_dynamicBody)
            {
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(20.f, 20.f));
                // The origin of an object defines the center point for all transformations
                rect.setOrigin(20.f, 20.f);
                rect.setFillColor(sf::Color::White);
                rect.setPosition(body->GetPosition().x, body->GetPosition().y);
                rect.setRotation(body->GetAngle() * 180/b2_pi);
                window.draw(rect);
            } else {
                sf::RectangleShape rect;
                rect.setFillColor(sf::Color::White);
                rect.setOrigin(400.f, 10.f);
                rect.setSize(sf::Vector2f(800, 20));
                rect.setPosition(body->GetPosition().x, body->GetPosition().y);
                rect.setRotation(body->GetAngle() * 180/b2_pi);
                window.draw(rect);
            }
        }

        window.display();
    }

    return 0;
}
