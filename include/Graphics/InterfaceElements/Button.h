#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <iostream>
#include <string>
#include <cassert>
#include <functional>

#include <Graphics/InterfaceElements/Widget.h>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Clock.hpp>

namespace ButtonConstants
{
	constexpr float HALF_DIVIDER = 2.0f;
	constexpr float CENTER_ALIGN_FACTOR = 0.5f;
}

enum class ButtonState { Normal, Hovered, Pressed, Disabled };

struct ButtonConfig
{
	sf::Text title;

	sf::Color outlineColor;
	sf::Color normalColor;
	sf::Color hoverColor;
	sf::Color pressedColor;
	sf::Color disabledColor;

	std::function<void()> onClickAction;

	float outlineThickness;

	sf::Vector2f buttonSize;
	sf::Vector2f buttonPosition;
};

class Button : public Widget
{
public:
	explicit Button(const ButtonConfig& config);

	void setPosition(const sf::Vector2f& pos) override;
	void setEnabled(bool enabled);
	void setSize(sf::Vector2f size);

	sf::Color lerpColors(const sf::Color& a, const sf::Color& b, float t);
	sf::Color lerpOverTime(const sf::Color& from, const sf::Color& to, float speed);

	sf::RectangleShape& getShape();
	bool isClicked();

	void draw(sf::RenderWindow& window) override;
	void handleEvent(const sf::RenderWindow& window, const sf::Event& event) override;
	void updateAppearance();

private:
	sf::RectangleShape _shape;

	ButtonConfig _config;
	ButtonState _state = ButtonState::Normal;

	bool _wasClicked = false;

	sf::Clock _animationClock;
	sf::Clock _hoverTimer;
};

#endif //BUTTON_HPP