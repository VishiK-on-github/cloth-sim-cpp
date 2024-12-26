#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "particle.h"
#include "constraint.h"

const float CLICK_TOLERANCE = 5.0f;

class InputHandler {
public:

    // method to tear the constraint line when mouse is close to it
    static void handle_mouse_click(const sf::Event& event, std::vector<Particle>& particles, std::vector<Constraint>& constraints) {

        // check if the event type is a left mouse click
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            // get x and y co-ords of the mouse
            float mouse_x = static_cast<float>(event.mouseButton.x);
            float mouse_y = static_cast<float>(event.mouseButton.y);
            tear_cloth(mouse_x, mouse_y, particles, constraints);
        }
    }

private:
    static float point_to_segment_distance(float px, float py, float x1, float y1, float x2, float y2) {
        
        // x and y diffs for the constraint
        float ABx = x2 - x1;
        float ABy = y2 - y1;

        // x and y diffs between the mouse 
        // pointer and the constraint vertex/particle 1
        float APx = px - x1;
        float APy = py - y1;

        // x and y diffs between the mouse 
        // pointer and the constraint vertex/particle 2
        float BPx = px - x2;
        float BPy = py - y2;

        // calculating the perpendicular projection
        // i dont understand it clearly tbh
        float AB_AP = ABx * APx + ABy * APy;
        float AB_AB = ABx * ABx + ABy * ABy;
        float t = AB_AP / AB_AB;

        if (t < 0.0f) { // mouse closer to vertex 1
            return std::sqrt(APx * APx + APy * APy);
        } else if (t > 1.0f) { // mouse closer to vertex 2
            return std::sqrt(BPx * BPx + BPy * BPy);
        } else {
            // find projections then the distance
            float proj_x = x1 + t * ABx;
            float proj_y = y1 + t * ABy;
            return std::sqrt((px - proj_x) * (px - proj_x) + (py - proj_y) * (py - proj_y));
        }
    }

    static Constraint* find_nearest_constraint(float mouse_x, float mouse_y, const std::vector<Constraint>& constraints) {
        
        Constraint *nearest_constraint = nullptr;
        // threshold of nearest constraint, if the distance 
        // between mouse pointer and constraint is larger 
        // than this the constraint wont break
        float min_distance = CLICK_TOLERANCE;

        // iterate over all constraints to check distance and get candidate constraints
        // optimal: sample some nearby constraints instead of all the constraints to
        // check distance
        for (const auto& constraint : constraints) {
            float distance = point_to_segment_distance(mouse_x, mouse_y, 
            constraint.p1->pos.x, constraint.p1->pos.y,
            constraint.p2->pos.x, constraint.p2->pos.y);

            // if distance is smaller than min distance cast and get 
            // the constraint, we will deactivate or tear it later
            if (distance < min_distance) {
                min_distance = distance;
                nearest_constraint = const_cast<Constraint*>(&constraint);
            }
        }
        return nearest_constraint;
    }

    static void tear_cloth(float mouse_x, float mouse_y, 
                            const std::vector<Particle>& particles, 
                            std::vector<Constraint>& constraints) {
        // find the closests constraints to the mouse position
        Constraint *nearest = find_nearest_constraint(mouse_x, mouse_y, constraints);

        // if we find the nearest constraint line, we break/tear deactivate it
        if (nearest) {
            nearest->deactivate();
        }
    }
};

#endif // INPUT_HANDLER