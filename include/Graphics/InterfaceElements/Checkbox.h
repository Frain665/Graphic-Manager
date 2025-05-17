#ifndef CHECKBOX_HPP
#define CHECKBOX_HPP

#include <iostream>
#include <memory>
#include <chrono>
#include <functional>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

#include <Graphics/InterfaceElements/Widget.h>
#include <Exceptions.h>

class CheckBox : public Widget
{
private:
	bool _isChecked;
	sf::RectangleShape _box;
	sf::RectangleShape _checkMark;
	sf::Text _label;
	sf::Font _font;

	std::function<void(bool)> _callback;

	std::chrono::steady_clock::time_point _lastClickTime;
	const std::chrono::milliseconds _clickDelay{ 200 };
	

public:
	CheckBox(const std::string& text, const sf::Vector2f& pos, unsigned int characterSize = 16);
	CheckBox(CheckBox&& other) noexcept;

	CheckBox& operator=(CheckBox&& other) noexcept;

	void setPosition(const sf::Vector2f& pos) override;
	void setSize(const sf::Vector2f& size);
	void setChecked(bool checked);
	void setCallback(const std::function<void(bool)>& func);

	bool getChecked() const;
	sf::Vector2f getPosition() const;
	sf::Vector2f getSize() const;
	sf::RectangleShape& getShape();

	void draw(sf::RenderWindow& window) override;
	void handleEvent(const sf::RenderWindow& window, const sf::Event& event) override;
};

#endif //CHECKBOX_HPP