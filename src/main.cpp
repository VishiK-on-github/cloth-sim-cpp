#include <SFML/Graphics.hpp>
#include <vector>

#include "particle.h"
#include "constraint.h"

const int WIDTH = 1080;
const int HEIGHT = 640;
const float PARTICLE_RADIUS = 30.0f;
const float GRAVITY = 10.f;
const float TIME_STEP = 0.1f;

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Cloth Sim");
    window.setFramerateLimit(60);

    std::vector<Particle> particles;
    particles.emplace_back(WIDTH/2 - 50, HEIGHT/2 - 50);
    particles.emplace_back(WIDTH/2 + 50, HEIGHT/2 + 50);
    particles.emplace_back(WIDTH/2 + 50, HEIGHT/2 - 50);
    particles.emplace_back(WIDTH/2 - 50, HEIGHT/2 + 50);

    std::vector<Constraint> constraints;
    constraints.emplace_back(&particles[0], &particles[1]);
    constraints.emplace_back(&particles[0], &particles[2]);
    constraints.emplace_back(&particles[0], &particles[3]);
    constraints.emplace_back(&particles[1], &particles[1]);
    constraints.emplace_back(&particles[1], &particles[2]);
    constraints.emplace_back(&particles[2], &particles[3]);


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        for (auto& particle : particles) {
            particle.apply_force(sf::Vector2f(0, GRAVITY));
            particle.update(TIME_STEP);
            particle.constraint_to_screen(WIDTH, HEIGHT, PARTICLE_RADIUS);
        }

        // for (size_t i = 0; i < 5; i++) {
        //     for (auto& constraint : constraints) {
        //         constraint.satisfy();
        //     }
        // }

        window.clear(sf::Color::Black);
        
        // draw particles
        for (const auto& particle : particles) {
            sf::CircleShape circle(PARTICLE_RADIUS);
            circle.setFillColor(sf::Color::White);
            circle.setPosition(particle.pos);
            window.draw(circle);
        }

        window.display();
    }
}