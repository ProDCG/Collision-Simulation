#include <SFML/Graphics.hpp>

struct Obj {
    sf::Vector2f last_position;
    sf::Vector2f position;
    sf::Vector2f acceleration;
    float radius = 10.0f;
    sf::Color color = sf::Color::White;

    Obj() = default;
    Obj(sf::Vector2f position_, float radius_)
        :position{ position_ }, last_position(position_), acceleration{0.0f, 0.0f}, radius{radius_} {}

    void update(double dt) {}

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



    private:


        void applyGravity() {}
        void checkCollisions(double dt) {}
        void applyConstraints(double dt) {}
        void updateObjects(double dt) {}
};

int main() {
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::White);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}