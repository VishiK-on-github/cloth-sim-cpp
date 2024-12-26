#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "particle.h"
#include <cmath>
#include <limits>

class Constraint {
public:
    /*
        A constraint as a line is described by two points/particles
    */

    Particle* p1; // first particle describing the constraint line
    Particle* p2; // second particle describing the constraint line
    float initial_length; // length between the particles describing the line/constraint
    bool active; // is the constraint active or has been torn/broken by the mouse click

    Constraint(Particle* p1, Particle* p2)
        : p1(p1)
        , p2(p2)
    {
        initial_length = std::hypot(p2->pos.x - p1->pos.x, // length between two points
            p2->pos.y - p1->pos.y);
        active = true; // currently the constraint is not broken hence active=true
    }

    void satisfy()
    {   
        // do not check this condition if the constraint is broken
        if (!active) return;


        sf::Vector2f delta = p2->pos - p1->pos; // particle position difference
        float current_length = std::hypot(delta.x, delta.y); // length of the line now
        float difference = (current_length - initial_length) / current_length; // factor of difference
        sf::Vector2f correction = delta * 0.5f * difference; // correction needed

        // if not pinned add correction factor to particle 1 and 2
        if (!p1->is_pinned) p1->pos += correction;
        if (!p2->is_pinned) p2->pos -= correction;
    }

    // if the constraint/line is cut set it to not active
    void deactivate()
    {
        active = false;
    }
};

#endif // CONSTRAINT_H