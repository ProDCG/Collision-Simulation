#pragma once
#include <vector>
#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>

struct Obj {
    sf::Vector2f last_position;
    sf::Vector2f position;
    sf::Vector2f acceleration;
    float radius = 10.0f;
    sf::Color color = sf::Color::White;

    Obj() = default;
    Obj(sf::Vector2f position_, float radius_)
        :position{ position_ }, last_position(position_), acceleration{ 0.0f, 0.0f }, radius{ radius_ } {}

    void update(float dt) {
        sf::Vector2f displacement = position - last_position;
        last_position = position;

        position = position + displacement + acceleration * (dt * dt);
        acceleration = {};
    }

    void accelerate(sf::Vector2f a) {
        acceleration += a;
    }

    void setVelocity(sf::Vector2f v, float dt) {
        last_position = position - (v * dt);
    }

    void addVelocity(sf::Vector2f v, float dt) {
        last_position -= (v * dt);
    }

    [[nodiscard]]
    sf::Vector2f getPosition() const {
        return position;
    }

    [[nodiscard]]
    sf::Vector2f getVelocity(float dt) const {
        return (position - last_position) / dt;
    }
};

class Solver {
public:
    Solver() = default;

    const std::vector<Obj>& getObjects() const {
        return s_objects;
    }

    Obj& addObject(sf::Vector2f position, float radius) {
        s_objects.emplace_back(position, radius);
        return s_objects.back();
    }

    void update() {
        s_time += s_frame_dt;

        const float step_dt = getStepDt();

        for (uint32_t i{ s_substeps }; i--;) {
            applyGravity();
            checkCollisions(step_dt);
            applyConstraints();
            updateObjects(step_dt);
        }
    }

    void setObjectVelocity(Obj& obj, sf::Vector2f velocity) {
        obj.setVelocity(velocity, getStepDt());
    }

    void setSimulationUpdateRate(uint32_t rate) {
        s_frame_dt = 1.0f / static_cast<float>(rate);
    }

    [[nodiscard]]
    float getTime() const {
        return s_time;
    }

    [[nodiscard]]
    float getStepDt() const {
        return s_frame_dt / static_cast<float>(s_substeps);
    }

    [[nodiscard]]
    uint32_t getObjectCount() const {
        return s_objects.size();
    }

private:
    sf::Vector2f s_gravity = { 0, 1000.0f };

    std::vector<Obj> s_objects;
    float s_frame_dt = 0.0f;
    float s_time = 0.0f;
    uint32_t s_substeps = 8;

    void applyGravity() {
        for (auto& obj : s_objects) {
            obj.accelerate(s_gravity);
        }
    }

    void checkCollisions(float dt) {
        const uint64_t obj_count = s_objects.size();
        for (uint64_t i{ 0 }; i < obj_count; ++i) {
            Obj& obj_1 = s_objects[i];
            
            for (uint64_t k{ i + 1 }; k < obj_count; ++k) {
                Obj& obj_2 = s_objects[k];
                const sf::Vector2f v = obj_1.position - obj_2.position;
                const float distance = v.x * v.x + v.y * v.y;
                const float min_distance = obj_1.radius + obj_2.radius;

                if (distance < min_distance * min_distance) {
                    const float dist = sqrt(distance);
                    const sf::Vector2f n = v / dist;
                    const float mr1 = obj_1.radius / (obj_1.radius + obj_1.radius);
                    const float mr2 = obj_2.radius / (obj_2.radius + obj_2.radius);
                    const float delta = 0.5f * (dist - min_distance);

                    obj_1.position -= n * (mr2 * delta);
                    obj_2.position += n * (mr1 * delta);
                }
            }
        }
    }

    void applyConstraints() {
        for (auto& obj : s_objects) {
            sf::Vector2f position = obj.getPosition();

            if (position.x < 0 + obj.radius) {
                const float n = (position.x - obj.radius);
                obj.position.x = obj.radius - n;
            }

            if (position.x > 1300 - obj.radius) {
                const float n = (position.x + obj.radius) - 1300;
                obj.position.x = (1300 - obj.radius) - n;
            }

            if (position.y > 1300 - obj.radius) {
                const float n = (position.y + obj.radius) - 1300;
                obj.position.y = (1300 - obj.radius) - n;
            }

            if (position.y < 0 + obj.radius) {
                const float n = (position.y - obj.radius);
                obj.position.y = (obj.radius) - n;
            }
        }
    }

    void updateObjects(float dt) {
        for (auto& obj : s_objects) {
            obj.update(dt);
        }
    }
};