#include "Graphics/InterfaceElements/Button.h"

Button::Button(const ButtonConfig& config)
	:_config(config)
{
	_shape.setSize(_config.buttonSize);
	_shape.setPosition(_config.buttonPosition);
	_shape.setFillColor(_config.normalColor);
	_shape.setOutlineThickness(_config.outlineThickness);
	_shape.setOutlineColor(_config.outlineColor);

	_config.title.setPosition(
		_config.buttonPosition.x + _config.buttonSize.x / 2 - _config.title.getGlobalBounds().width / 2,
		_config.buttonPosition.y + _config.buttonSize.y / 2 - _config.title.getGlobalBounds().height / 2
	);

}

void Button::setPosition(const sf::Vector2f& pos)
{
	assert(_shape.getSize().x > 0 && _shape.getSize().y > 0);
	assert(!_config.title.getString().isEmpty());

	auto shapeCenter = sf::Vector2f(pos) + _shape.getSize() / ButtonConstants::HALF_DIVIDER;
	auto textSize = _config.title.getLocalBounds();

	_config.buttonPosition = pos;
	_shape.setPosition(_config.buttonPosition);

	_config.title.setPosition(
		shapeCenter.x - textSize.width * ButtonConstants::CENTER_ALIGN_FACTOR,
		shapeCenter.y - textSize.height * ButtonConstants::CENTER_ALIGN_FACTOR
	);

}

void Button::setEnabled(bool enabled)
{
	_state = enabled ? ButtonState::Normal : ButtonState::Disabled;
}

void Button::setSize(sf::Vector2f size)
{
	if (size.x <= 0 || size.y <= 0) return;

	_shape.setSize(size);
	_config.buttonSize = size;

	sf::FloatRect textBounds = _config.title.getLocalBounds();
	_config.title.setOrigin(textBounds.left + textBounds.width / 2,
		textBounds.top + textBounds.height / 2);
	_config.title.setPosition(
		_shape.getPosition().x + size.x / 2,
		_shape.getPosition().y + size.y / 2
	);

	updateAppearance();
}

sf::Color Button::lerpColors(const sf::Color& a, const sf::Color& b, float t)
{
	t = std::clamp(t, 0.0f, 1.0f); 

	if (a == b) return a;

	return sf::Color(
		static_cast<sf::Uint8>(a.r + (b.r - a.r) * t),
		static_cast<sf::Uint8>(a.g + (b.g - a.g) * t),
		static_cast<sf::Uint8>(a.b + (b.b - a.b) * t), 
		static_cast<sf::Uint8>(a.a + (b.a - a.a) * t)
	);
}

sf::Color Button::lerpOverTime(const sf::Color& from, const sf::Color& to, float speed)
{
	float t = std::min(_animationClock.getElapsedTime().asSeconds() * speed, 1.0f);
	return lerpColors(from, to, t);
}

sf::RectangleShape& Button::getShape()
{
	return _shape;
}

bool Button::isClicked()
{
	if (_wasClicked)
	{
		_wasClicked = false;
		return true;
	}
	
	return false;
}

void Button::draw(sf::RenderWindow& window)
{
	window.draw(_shape);
	window.draw(_config.title);
}

void Button::handleEvent(const sf::RenderWindow& window, const sf::Event& event)
{
	if (_state == ButtonState::Disabled)
		return;

	auto mousePos = sf::Mouse::getPosition(window);
	bool contains = _shape.getGlobalBounds().contains(
		static_cast<float>(mousePos.x),
		static_cast<float>(mousePos.y)
	);

	if (event.type == sf::Event::MouseMoved)
	{
		if (!contains && _state == ButtonState::Pressed)
		{
			_state = ButtonState::Normal;
		}
	}

	if (contains)
	{

		if (contains)
		{
			if (event.type == sf::Event::MouseButtonPressed &&
				event.mouseButton.button == sf::Mouse::Left)
			{
				_state = ButtonState::Pressed;
			}
			else if (event.type == sf::Event::MouseButtonReleased &&
				event.mouseButton.button == sf::Mouse::Left &&
				_state == ButtonState::Pressed &&
				!_wasClicked)
			{
				if (_config.onClickAction)
					_config.onClickAction();
				_wasClicked = true;
				_state = ButtonState::Hovered;
				_animationClock.restart();
			}
		}
		else
		{
			_state = ButtonState::Normal;
		}

	}
	else
	{
		_state = ButtonState::Normal;
		_wasClicked = false;
	}

	updateAppearance();
}

void Button::updateAppearance()
{

	sf::Color targetColor;

	switch (_state)
	{
	case ButtonState::Hovered:
		if (_hoverTimer.getElapsedTime().asMilliseconds() > 100)  
			targetColor = _config.hoverColor;
		break;

	case ButtonState::Pressed:
		targetColor = _config.pressedColor;
		_shape.setFillColor(lerpOverTime(_shape.getFillColor(), targetColor, 0.12f));
		break;

	case ButtonState::Disabled:
		targetColor = _config.disabledColor;
		_shape.setFillColor(lerpOverTime(_shape.getFillColor(), targetColor, 0.1f));
		break;

	default: 
		targetColor = _config.normalColor;
		_shape.setFillColor(lerpOverTime(_shape.getFillColor(), targetColor, 0.15f));
	}
}
