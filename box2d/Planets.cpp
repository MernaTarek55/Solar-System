#include "Planets.h"
#include <iostream>

Planets::Planets(b2World& world, sf::Texture& texture, sf::Vector2f position, b2Body* starBody)
    : body(nullptr), orbitArray(sf::LinesStrip) {
    PLANET_RADIUS = float((rand()%100)/10+5);
    orbitcolor = sf::Color::Color(rand() % 255, rand() % 255, rand() % 255, 255);
    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
    sprite.setScale(PLANET_RADIUS * 2.f / texture.getSize().x, PLANET_RADIUS * 2.f / texture.getSize().y);
    sprite.setPosition(position);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = pixelsToMeters(position);
    body = world.CreateBody(&bodyDef);

    b2CircleShape shape;
    shape.m_radius = PLANET_RADIUS / PIXELS_PER_METER;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.f;
    body->CreateFixture(&fixtureDef);

    setInitialOrbitVelocity(starBody);
}

void Planets::applyGravity(b2Body* starBody)
{
    b2Vec2 planetPosition = body->GetPosition();
    b2Vec2 starPosition = starBody->GetPosition();
    
    b2Vec2 direction = starPosition - planetPosition;
    float distance = direction.Length();
    if (distance > 0.1f) {
        float forceMagnitude = GRAVITATIONAL_CONSTANT * STAR_MASS * body->GetMass() / (distance * distance);
        direction.Normalize();
        b2Vec2 force = forceMagnitude * direction;
        body->ApplyForceToCenter(force, true);

        sf::Vertex v(sprite.getPosition());
        v.color = orbitcolor;
        orbitArray.append(v);
    }
}

bool Planets::checkCollisionWithStar(b2Body* starBody)
{
    b2Vec2 planetPosition = body->GetPosition();
    b2Vec2 starPosition = starBody->GetPosition();

    float distance = (starPosition - planetPosition).Length();
    return distance < (STAR_RADIUS + PLANET_RADIUS) / PIXELS_PER_METER;
}

void Planets::updateSpritePosition()
{
    sprite.setPosition(metersToPixels(body->GetPosition()));
}

void Planets::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
void Planets::draworbit(sf::RenderWindow& window)
{
    window.draw(orbitArray);
}
sf::Vector2f Planets::metersToPixels(const b2Vec2& meters)
{
    return sf::Vector2f(meters.x * PIXELS_PER_METER, meters.y * PIXELS_PER_METER);
}
b2Vec2 Planets::pixelsToMeters(const sf::Vector2f& pixels)
{
    return b2Vec2(pixels.x / PIXELS_PER_METER, pixels.y / PIXELS_PER_METER);
}
const float Planets::PIXELS_PER_METER = 50.f;
const float Planets::STAR_RADIUS = 50.f;
const float Planets::GRAVITATIONAL_CONSTANT = 0.1f;
const float Planets::STAR_MASS = 2000.f;