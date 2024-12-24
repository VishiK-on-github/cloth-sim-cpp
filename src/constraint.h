#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "particle.h"
#include <cmath>
#include <limits>

class Constraint {
public:
    Particle *p1;
    Particle *p2;
    float initial_length;

    Constraint(Particle *p1, Particle *p2) : p1(p1), p2(p2) {
        initial_length = std::hypot(p2->pos.x - p1->pos.x, 
                                    p2->pos.y - p1->pos.y);
    }

    void satisfy() {
        sf::Vector2f delta = p2->pos - p1->pos;
        float current_length = std::hypot(delta.x, delta.y);
        float difference = (current_length - initial_length) / current_length;
        sf::Vector2f correction = delta * 0.5f * difference;

        p1->pos += correction;
        p2->pos -= correction;
    }
};

#endif // CONSTRAINT_H