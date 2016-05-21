#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include <iostream>
#include <math.h>

#include <stdexcept>
#include <typeinfo>
using namespace std;

class Box2DWorld
{
    private:
        b2World* world;
        b2Body* ground;
    public:
        Box2DWorld();

        b2Body* create_ground(b2World&, float, float);
        b2Body* create_box(b2World&, float, float);
        b2Vec2 apply_force(b2Body& box);

        b2World* get_world();

};

class SFMLWindow
{
    private:
        int width, height;
        sf::RenderWindow sfml_window;
        sf::Color border_color;
        sf::Texture earth;
        Box2DWorld world;
    public:
        SFMLWindow(int, int);
        void run();
        void render_body(b2Body *);

};

SFMLWindow::SFMLWindow(int width, int height)
{
    sfml_window.create(sf::VideoMode(width, height), "SFML and Box2d");
    border_color = sf::Color(128,128,128);

    if (!earth.loadFromFile("media/earth.png"))
        throw std::invalid_argument("Invalid texture path");

    world = Box2DWorld();
}

void SFMLWindow::render_body(b2Body *body)
{
    if (body->GetType() == b2_dynamicBody)
    {
        // apply force to box (simulate gravity)
        world.apply_force(*body);

        sf::RectangleShape rect;
        rect.setSize(sf::Vector2f(18.f, 18.f));
        // The origin of an object defines the center point for all transformations
        rect.setOrigin(9.f, 9.f);

        rect.setFillColor(sf::Color::White);
        rect.setOutlineColor(border_color);
        rect.setOutlineThickness(2.f);

        rect.setPosition(body->GetPosition().x, body->GetPosition().y);
        rect.setRotation(body->GetAngle() * 180/b2_pi);
        sfml_window.draw(rect);
    } else {
        sf::CircleShape circle;
        circle.setTexture(&earth);
        circle.setFillColor(sf::Color::White);
        circle.setRadius(40.f);
        circle.setOrigin(40.f, 40.f);
        circle.setPosition(body->GetPosition().x, body->GetPosition().y);
        circle.setRotation(body->GetAngle() * 180/b2_pi);
        sfml_window.draw(circle);
    }
}

void SFMLWindow::run()
{
    while (sfml_window.isOpen())
    {
        sf::Event event;
        while (sfml_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                sfml_window.close();
            }
        }

        sfml_window.clear();
        world.get_world()->Step(1/60.f, 8, 3);

        for (b2Body* body = world.get_world()->GetBodyList(); body != 0; body = body->GetNext())
        {
            render_body(body);
        }

        sfml_window.display();
    }
}

Box2DWorld::Box2DWorld()
{
    // init world
    world = new b2World(b2Vec2(0.f, 9.8f));
    // create static body
    ground = create_ground(*world, 400, 300);
    // create dynamic bodies and apply init forse to them
    b2Body* box = create_box(*world, 200.f, 50.f);
    box->ApplyForce(b2Vec2(-5000.f,200000.f), box->GetWorldCenter(), false);

}

b2World* Box2DWorld::get_world()
{
    return world;
}

b2Body* Box2DWorld::create_ground(b2World& world, float x, float y)
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

b2Body* Box2DWorld::create_box(b2World& world, float x, float y)
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

b2Vec2 Box2DWorld::apply_force(b2Body& box)
{
    b2Vec2 planet_distance = b2Vec2(0,0);
    b2Vec2 planet_position = ground->GetWorldCenter();

    planet_distance += box.GetWorldCenter() - planet_position;
    // Calculate the magnitude of the force to apply to the debris.
    // This is proportional to the distance between the planet and
    // the debris. The force is weaker the further away the debris.
    float force = (box.GetMass() * 100.f) / pow(planet_distance.Length(), 2);
    // apply force to box
    // change the direction of the vector so that the force will be
    // towards the planet.
    planet_distance *= force * -1;
    box.ApplyForce(planet_distance, box.GetWorldCenter(), true);
}

int main()
{
    SFMLWindow sfml_window(800, 600);
    sfml_window.run();
    return 0;
}
