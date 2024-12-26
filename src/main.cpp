#include <SFML/Graphics.hpp>
#include <vector>

#include "constraint.h"
#include "particle.h"
#include "input.h"

const int WIDTH = 1080; // screen width
const int HEIGHT = 640; // screen height
const float GRAVITY = 9.8f; // acceleration due to gravity
const float TIME_STEP = 0.1f;
const int ROW = 50; // rows in the mesh
const int COLUMN = 50; // columns in the mesh
const float REST_DISTANCE = 5.0f; // length of the edge of the mesh
const float OFFSET = 10.0f;

int main()
{   
    // defining window size and name
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Cloth Sim");
    window.setFramerateLimit(60); // fps limit

    std::vector<Particle> particles;
    std::vector<Constraint> constraints;

    // arrange the particles to be vertices in a grid
    for (int row = 0; row < ROW; row++) {
        for (int col = 0; col < COLUMN; col++) {
            float x = col * REST_DISTANCE;
            float y = row * REST_DISTANCE;
            bool pinned = (row == 0);
            particles.emplace_back(x, y, pinned);
        }
    }

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

    // check if window is open
    while (window.isOpen()) {
        sf::Event event;

        // look for events happening in the window
        while (window.pollEvent(event)) {

            // close the window if event type is closed
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // for other events check if we have appropriate mouse event
            InputHandler::handle_mouse_click(event, particles, constraints);
        }

        // for every particle
        for (auto& particle : particles) {
            particle.apply_force(sf::Vector2f(0, GRAVITY)); // apply gravity in the y axis
            particle.update(TIME_STEP); // update particle position after time TIME_STEP
            particle.constraint_to_screen(WIDTH, HEIGHT); // verify constraint or bounds
        }

        // positional updates due to new position of particles
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