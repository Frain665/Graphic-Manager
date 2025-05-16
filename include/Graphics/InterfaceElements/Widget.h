#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

class Widget
{
public:
	virtual void draw(sf::RenderWindow& window) = 0;
	virtual void handleEvent(const sf::RenderWindow& window, const sf::Event& event) = 0;
	virtual void setPosition(const sf::Vector2f& pos) = 0;

	virtual ~Widget() = default;
};

#endif //WIDGET_HPP