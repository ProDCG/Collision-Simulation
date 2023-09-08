#pragma once
#include "solver.hpp"

class Renderer {
	public:
		explicit Renderer(sf::RenderTarget& target) :m_target{ target } {}

		void render(const Solver& solver) const {
			sf::CircleShape circle{ 1.0f };
			circle.setOrigin(1.0f, 1.0f);
			
			const auto& objects = solver.getObjects();
			for (auto& obj : objects) {
				circle.setPosition(obj.position);
				circle.setScale(obj.radius, obj.radius);
				circle.setFillColor(obj.color);
				m_target.draw(circle);
			}
		}

	private:
		sf::RenderTarget& m_target;
};