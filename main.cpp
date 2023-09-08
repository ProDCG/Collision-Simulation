#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "solver.hpp"
#include "renderer.hpp"

static sf::Color getColor(float t) {
    double pi = 2 * asin(1.0);
    const float r = sin(t);
    const float g = sin(t + 0.33f * 2.0f * pi);
    const float b = sin(t + 0.66f * 2.0f * pi);
    return { static_cast<uint8_t>(255.0f * r * r),
    static_cast<uint8_t>(255.0f * g * g),
    static_cast<uint8_t>(255.0f * b * b) };
}

int main() {
    constexpr int32_t window_width = 1300;
    constexpr int32_t window_height = 1300;
    const uint32_t frame_rate = 60;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 1;

    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Simulation", sf::Style::Default, settings);
    window.setFramerateLimit(frame_rate);
    
    Solver solver;
    Renderer renderer{ window };

    const float spawn_delay = 0.025f;
    const sf::Vector2f spawn_position = { 15.0f, 200.0f };
    const sf::Vector2f spawn_speed = {1000, 0.0f };

    solver.setSimulationUpdateRate(frame_rate);

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        }

        solver.update();
        window.clear(sf::Color::Black);
        renderer.render(solver);
        window.display();

        if (clock.getElapsedTime().asSeconds() >= spawn_delay && solver.getObjectCount() < 4000) {
            clock.restart();
            for (int i = 0; i < 5; ++i) {
                sf::Vector2f addition = { 0.0f, i * 15.0f };
                auto& object = solver.addObject(spawn_position + addition, 7.5f);
                solver.setObjectVelocity(object, spawn_speed);
                object.color = getColor(solver.getTime());
            }
        }
    }
}