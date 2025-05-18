#ifndef TEXT_FIELD_HPP
#define TEXT_FIELD_HPP

#include <iostream>
#include <memory>
#include <chrono>
#include <string>
#include <functional>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <Graphics/InterfaceElements/Widget.h>
#include <Exceptions.h>

class TextField : public Widget
{
public:

	TextField();

	const std::string& getText() const;

	void setCharacterSize(unsigned int characterSize);
	void setSize(const float& width, const float& height);
	void setSize(const sf::Vector2f& size);
	void setMaxLength(unsigned int length);
	void setText(const std::string& text);
	void setPosition(const sf::Vector2f& pos) override;

	void handleEvent(const sf::RenderWindow& window, const sf::Event& event) override;
	void handleTextInput(sf::Uint32 unicode);
	void draw(sf::RenderWindow& window) override;

private:
	sf::Clock _keyRepeatClock;
	sf::Font _font;

	sf::Text _text;
	sf::RectangleShape _background;

	sf::Color _activeColor;
	sf::Color _inactiveColor;

	bool _isActive;
	std::string _inputString;
	unsigned int _characterSize;
	unsigned int _maxLength;

	std::chrono::steady_clock::time_point _lastClickTime;
	const std::chrono::milliseconds _clickDelay{ 200 };
};

#endif //TEXT_FIELD_HPP