#ifndef PARTICLE_H
#define PARTICLE_H

#include <SFML/Graphics.hpp>

class Particle {
public:
    sf::Vector2f pos;
    sf::Vector2f prev_pos;
    sf::Vector2f acc;

    Particle(float x, float y) : pos(x, y), prev_pos(x, y), acc(0, 0) {}

    void apply_force(const sf::Vector2f& force) {
        acc += force;
    }

    void update(float time_step) {
        sf::Vector2f velocity = pos - prev_pos;
        prev_pos = pos;
        pos += velocity + acc * time_step * time_step;
        acc = sf::Vector2f(0, 0);
    }

    void constraint_to_screen(float width, float height, float radius) {
        if (pos.x < radius) {
            pos.x = radius;
        }
        if (pos.x > width - radius) {
            pos.x = width - radius;
        }
        if (pos.y < radius) {
            pos.y = radius;
        }
        if (pos.y > height - radius) {
            pos.y = height - radius;
        }
    }
};

#endif // PARTICLE_H