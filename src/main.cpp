#include <SFML/Graphics.hpp>
#include <vector>

#include "constraint.h"
#include "particle.h"
#include "input.h"

const int WIDTH = 1080;
const int HEIGHT = 640;
const float GRAVITY = 9.8f;
const float TIME_STEP = 0.1f;
const int ROW = 50;
const int COLUMN = 50;
const float REST_DISTANCE = 5.0f;

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Cloth Sim");
    window.setFramerateLimit(60);

    std::vector<Particle> particles;
    std::vector<Constraint> constraints;

    // init particles
    for (int row = 0; row < ROW; row++) {
        for (int col = 0; col < COLUMN; col++) {
            float x = col * REST_DISTANCE + WIDTH/3;
            float y = row * REST_DISTANCE + HEIGHT/3;
            bool pinned = (row == 0);
            particles.emplace_back(x, y, pinned);
        }
    }

    // init constraints
    for (int row = 0; row < ROW; row++) {
        for (int col = 0; col < COLUMN; col++) {
            if (col < COLUMN - 1) {
                constraints.emplace_back(&particles[row * COLUMN + col], &particles[row * COLUMN + col + 1]);
            }
            if (row < ROW - 1) {
                constraints.emplace_back(&particles[row * COLUMN + col], &particles[(row + 1) * COLUMN + col]);
            }
        }
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            InputHandler::handle_mouse_click(event, particles, constraints);
        }

        for (auto& particle : particles) {
            particle.apply_force(sf::Vector2f(0, GRAVITY));
            particle.update(TIME_STEP);
            particle.constraint_to_screen(WIDTH, HEIGHT);
        }

        for (size_t i = 0; i < 5; i++) {
            for (auto& constraint : constraints) {
                constraint.satisfy();
            }
        }

        window.clear(sf::Color::Black);

        // draw particles as points
        for (const auto& particle : particles) {
            sf::Vertex point(particle.pos, sf::Color::White);
            window.draw(&point, 1, sf::Points);
        }

        // draw constraint as lines
        for (const auto& constraint : constraints) {
            if (!constraint.active) {
                continue;
            }
            sf::Vertex line[] = {
                sf::Vertex(constraint.p1->pos, sf::Color::White),
                sf::Vertex(constraint.p2->pos, sf::Color::White)
            };
            window.draw(line, 2, sf::Lines);
        }

        window.display();
    }
}