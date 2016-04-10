#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include <iostream>
#include <math.h>

b2Body* createGround(b2World& world, float x, float y)
{
    b2BodyDef rectBodyDef;
    rectBodyDef.type = b2_staticBody;
    rectBodyDef.position.Set(x, y);

    b2Body* body = world.CreateBody(&rectBodyDef);

    b2CircleShape shape;
    shape.m_radius = 40.f;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 0.0f;

    body->CreateFixture(&fixtureDef);
    return body;
}

b2Body* createBox(b2World& world, float x, float y)
{
    b2BodyDef rectBodyDef;
    rectBodyDef.type = b2_dynamicBody; //this will be a dynamic body
    rectBodyDef.position.Set(x, y); //set the starting position
    rectBodyDef.angle = 0; //set the starting angle

    b2Body* body = world.CreateBody(&rectBodyDef);
    //body->SetTransform(body->GetPosition(), 0.1f);

    b2PolygonShape shape;
    shape.SetAsBox(10.f, 10.f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.density = 1.0f;
    // Override the default friction.
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.5f;

    body->CreateFixture(&fixtureDef);
    body->SetGravityScale(0);

    return body;
}

b2Vec2 getBodyForce(b2Body& planet_body, b2Body& dynamic_body)
{
    b2Vec2 planet_distance = b2Vec2(0,0);
    b2Vec2 planet_position = planet_body.GetWorldCenter();

    planet_distance += dynamic_body.GetWorldCenter() - planet_position;
    // Calculate the magnitude of the force to apply to the debris.
    // This is proportional to the distance between the planet and
    // the debris. The force is weaker the further away the debris.
    float force = (dynamic_body.GetMass() * 100.f) / pow(planet_distance.Length(), 2);
    // change the direction of the vector so that the force will be
    // towards the planet.
    planet_distance *= force * -1;
    return planet_distance;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML and Box2d");
    sf::Color grey = sf::Color(128,128,128);

    sf::Texture earth;
    if (!earth.loadFromFile("media/earth.png"))
        return -1;

    b2Vec2 gravity(0.f, 9.8f);
    b2World* world = new b2World(gravity);

    b2Body* ground = createGround(*world, 400, 300);

    // init box
    b2Body* box = createBox(*world, 200.f, 50.f);
    box->ApplyForce(b2Vec2(-5000.f,200000.f), box->GetWorldCenter(), false);


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
                // custom gravity
                body->ApplyForce(getBodyForce(*ground, *body), body->GetWorldCenter(), true);

                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(18.f, 18.f));
                // The origin of an object defines the center point for all transformations
                rect.setOrigin(9.f, 9.f);

                rect.setFillColor(sf::Color::White);
                rect.setOutlineColor(grey);
                rect.setOutlineThickness(2.f);

                rect.setPosition(body->GetPosition().x, body->GetPosition().y);
                rect.setRotation(body->GetAngle() * 180/b2_pi);
                window.draw(rect);
            } else {
                sf::CircleShape circle;
                circle.setTexture(&earth);
                circle.setFillColor(sf::Color::White);
                circle.setRadius(40.f);
                circle.setOrigin(40.f, 40.f);
                circle.setPosition(body->GetPosition().x, body->GetPosition().y);
                circle.setRotation(body->GetAngle() * 180/b2_pi);
                window.draw(circle);
            }
        }

        window.display();
    }

    return 0;
}
