#ifndef PROGRESS_BAR_HPP
#define PROGRESS_BAR_HPP

#include <iostream>
#include <functional>
#include <chrono>
#include <cmath>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

#include <Graphics/InterfaceElements/Widget.h>
#include <Exceptions.h>

class ProgressBar : public Widget
{
private:
	sf::VertexArray _gradientVertices;

	sf::RectangleShape _background;
	sf::RectangleShape _fill;
	sf::RectangleShape _border;
	sf::Text _text;
	sf::Font _font;

	float _maxValue = 100.f;
	float _currentValue = 0.f;
	float _targetValue = 0.f;
	float _smoothness = 0.f;

	bool _isVertical = false;
	bool _showBorder = false;
	bool _showText = true;
	bool _useGradient = false;
	bool _isDragging = false;
	bool _isEnabled = true;

	std::vector<sf::Vertex> _gradientFill;

	std::function<void()> _onComplete;

	std::chrono::steady_clock::time_point _lastClickTime;
	std::chrono::milliseconds _clickDelay{ 200 };

	void updateFill();

public:
	ProgressBar(const sf::Vector2f& size,
		const sf::Color& bgColor,
		const sf::Color& fillColor);

	ProgressBar(ProgressBar&& other);
	ProgressBar& operator=(ProgressBar&& other);

	std::function<void(float)> _onValueChanged;

	void setEnabled(bool enabled) { _isEnabled = enabled; }
	bool isEnabled() const { return _isEnabled; }
	void setValue(float value);
	void setMaxValue(float maxValue);
	void setOrientation(bool isVertical);
	void setPosition(const sf::Vector2f& pos) override;
	void enableBorder(bool enable, const sf::Color& color, float thickness = 1.f);
	void setSmoothness(float smoothness);
	void setFillGradient(const sf::Color& start, const sf::Color& end);
	sf::Color lerpColors(const sf::Color& a, const sf::Color& b, float t);

	float getPercentage() const;

	void showPercentage(bool show, const sf::Font& font,
		unsigned int charSize = 16);
	void updateProgressFromMouse(const sf::Vector2f& mousePos);

	void draw(sf::RenderWindow& window) override;
	void handleEvent(const sf::RenderWindow& window, const sf::Event& event) override;
	void updateTextPosition();
	void updatePercentageText();
	void update(float deltaTime);
};

#endif //PROGRESS_BAR_HPP