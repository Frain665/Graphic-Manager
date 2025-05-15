#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <iostream>
#include <cassert>
#include <functional>

#include <Graphics/InterfaceElements/Widget.h>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

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

	void setPosition(float xPos, float yPos) override;
	void setEnabled(bool enabled);

	sf::RectangleShape& getShape();
	bool isClicked(const sf::Event& event) const;

	void draw(sf::RenderWindow& target) override;
	void handleEvent(const sf::Event& event) override;

private:
	sf::RectangleShape _shape;

	ButtonConfig _config;
	ButtonState _state = ButtonState::Normal;

	bool _wasPressed = false;
};

#endif //BUTTON_HPP