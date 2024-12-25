#ifndef PARTICLE_H
#define PARTICLE_H

#include <SFML/Graphics.hpp>

class Particle {
public:
    sf::Vector2f pos;
    sf::Vector2f prev_pos;
    sf::Vector2f acc;
    bool is_pinned;

    Particle(float x, float y, bool pinned)
        : pos(x, y)
        , prev_pos(x, y)
        , acc(0, 0)
        , is_pinned(pinned)
    {
    }

    void apply_force(const sf::Vector2f& force)
    {
        if (!is_pinned) {
            acc += force;
        }
    }

    void update(float time_step)
    {
        if (!is_pinned) {
            sf::Vector2f velocity = pos - prev_pos;
            prev_pos = pos;
            pos += velocity + acc * time_step * time_step;
            acc = sf::Vector2f(0, 0);
        }
    }

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