#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include "Planets.h"

const int NUM_PLANETS = 9;
enum PlanetTextures {
    EARTH,
    MARS,
    VENUS,
    JUPITER,
    MERCURY,
    NEPTUNE,
    PLUTO,
    SATURN,
    URANUS,
    
};

const std::string PlanetTextureFiles[NUM_PLANETS] = {
    "Asserts/Images/earth.png",
    "Asserts/Images/mars.png",
    "Asserts/Images/venus.png",
    "Asserts/Images/jupiter.png",
    "Asserts/Images/mercury.png",
    "Asserts/Images/neptune.png",
    "Asserts/Images/pluto.png",
    "Asserts/Images/saturn.png",
    "Asserts/Images/uranus.png",
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Solar System Simulation");
    window.setFramerateLimit(60);

    b2Vec2 gravity(0.f, 0.f);
    b2World world(gravity);

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("Asserts/Images/space.jpg")) {
        return -1;
    }
    sf::Sprite backgroundSprite(backgroundTexture);

    sf::Texture sunTexture;
    if (!sunTexture.loadFromFile("Asserts/Images/sun.png")) {
        return -1;
    }
    sf::Sprite sunSprite(sunTexture);
    sunSprite.setOrigin(sunTexture.getSize().x / 2.f, sunTexture.getSize().y / 2.f);
    sunSprite.setPosition(400.f, 300.f);
    sunSprite.setScale(Planets::STAR_RADIUS * 2.f / sunTexture.getSize().x, Planets::STAR_RADIUS * 2.f / sunTexture.getSize().y);
    
    b2BodyDef starBodyDef;
    starBodyDef.position = Planets::pixelsToMeters(sunSprite.getPosition());
    b2Body* starBody = world.CreateBody(&starBodyDef);

    b2CircleShape starShape;
    starShape.m_radius = Planets::STAR_RADIUS / Planets::PIXELS_PER_METER;
    starBody->CreateFixture(&starShape, 0.f);

    std::vector<sf::Texture> planetTextures(NUM_PLANETS);
    for (int i = 0; i < NUM_PLANETS; ++i) {
        if (!planetTextures[i].loadFromFile(PlanetTextureFiles[i])) {
            return -1;
        }
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, NUM_PLANETS - 1);

    std::vector<Planets> planets;
    int c = 0;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f clickPosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                int textureIndex = dist(gen);
                planets.emplace_back(world, planetTextures[textureIndex], clickPosition, starBody);
            }
        }

        for (auto it = planets.begin(); it != planets.end();) {
            it->applyGravity(starBody);
            if (it->checkCollisionWithStar(starBody)) {
                world.DestroyBody(it->getBody());
                it = planets.erase(it);
            }
            else {
                ++it;
            }
        }

        world.Step(1 / 60.f, 8, 3);

        window.clear(sf::Color::Black);
        window.draw(backgroundSprite);
        window.draw(sunSprite);
        for (auto& planet : planets) {
            planet.draworbit(window);

        }
        for (auto& planet : planets) {
            planet.updateSpritePosition();
            planet.draw(window);

        }
        if (c % 10 == 0)
            sunSprite.setScale(-sunSprite.getScale().x, sunSprite.getScale().y);
        c++;
        window.display();
    }
    
    return 0;
}
