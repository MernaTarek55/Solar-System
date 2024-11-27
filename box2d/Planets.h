#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
class Planets {
public:
    Planets(b2World& world, sf::Texture& texture, sf::Vector2f position, b2Body* starBody);

    void applyGravity(b2Body* starBody);

    bool checkCollisionWithStar(b2Body* starBody);

    void updateSpritePosition();

    void draw(sf::RenderWindow& window);
    void draworbit(sf::RenderWindow& window);
    b2Body* getBody() const { return body; }
    static sf::Vector2f metersToPixels(const b2Vec2& meters);

    static b2Vec2 pixelsToMeters(const sf::Vector2f& pixels);
    static const float PIXELS_PER_METER;
    static const float STAR_RADIUS;
    static const float PLANET_RADIUS;
    static const float GRAVITATIONAL_CONSTANT;
    static const float STAR_MASS;
private:
    
    
    
    void setInitialOrbitVelocity(b2Body* starBody) {
        b2Vec2 starPosition = starBody->GetPosition();
        b2Vec2 planetPosition = body->GetPosition();
        b2Vec2 direction = starPosition - planetPosition;
        float distance = direction.Length();
        direction.Normalize();

        b2Vec2 tangentialVelocity(-direction.y, direction.x);
        float orbitalSpeed = sqrt(GRAVITATIONAL_CONSTANT * STAR_MASS / distance);
        tangentialVelocity *= orbitalSpeed;
        body->SetLinearVelocity(tangentialVelocity);
    }

    b2Body* body;
    sf::Sprite sprite;
    sf::VertexArray orbitArray;
};