#ifndef PARTICLE_H
#define PARTICLE_H

#include <SFML/Graphics.hpp>

class Particle {
public:
    sf::Vector2f pos; // current position
    sf::Vector2f prev_pos; // previous position
    sf::Vector2f acc; // acceleration
    bool is_pinned; // used to denote if the particle is fixed

    Particle(float x, float y, bool pinned)
        : pos(x, y)
        , prev_pos(x, y)
        , acc(0, 0)
        , is_pinned(pinned)
    {
    }

    // update the g force
    void apply_force(const sf::Vector2f& force)
    {
        if (!is_pinned) {
            acc += force;
        }
    }

    // update the position of the particle using 
    // velocity and acceleration
    void update(float time_step)
    {
        if (!is_pinned) {
            sf::Vector2f velocity = pos - prev_pos; // not dividing by time to save some bytes and faster calculation
            prev_pos = pos;
            pos += velocity + 0.5f * acc * time_step * time_step; // ideally should be d = v*t + 1/2 * a * t^2
            acc = sf::Vector2f(0, 0);
        }
    }

    // resetting position of the particle incase we hit boundary conditions
    // basically if particle goes beyond dimensions of the screen we reset it.
    void constraint_to_screen(float width, float height)
    {
        if (pos.x < 0)
            pos.x = 0;
        if (pos.x > width)
            pos.x = width;
        if (pos.y < 0)
            pos.y = 0;
        if (pos.y > height)
            pos.y = height;
    }
};

#endif // PARTICLE_H