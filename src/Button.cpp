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

void Button::setPosition(float xPos, float yPos)
{
	assert(_shape.getSize().x > 0 && _shape.getSize().y > 0);
	assert(!_config.title.getString().isEmpty());

	auto shapeCenter = sf::Vector2f(xPos, yPos) + _shape.getSize() / ButtonConstants::HALF_DIVIDER;
	auto textSize = _config.title.getLocalBounds();

	_config.buttonPosition = sf::Vector2f(xPos, yPos);
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

sf::Color Button::lerpColors(const sf::Color& a, const sf::Color& b, float t)
{
	t = std::clamp(t, 0.0f, 0.1f);

	return sf::Color(
		static_cast<sf::Uint8>(a.r + (b.r - a.r) * t),
		static_cast<sf::Uint8>(a.g + (b.g - a.g) * t),
		static_cast<sf::Uint8>(a.b + (b.g - a.b) * t),
		static_cast<sf::Uint8>(a.a + (b.a - a.a) * t)
	);
}

sf::RectangleShape& Button::getShape()
{
	return _shape;
}

bool Button::isClicked(const sf::Event& event) const
{
	return _wasPressed;
}

void Button::draw(sf::RenderWindow& window)
{
	window.draw(_shape);
	window.draw(_config.title);
}

void Button::handleEvent(const sf::RenderWindow& window, const sf::Event& event)
{
	if (_state == ButtonState::Disabled) return;

	auto mousePos = sf::Mouse::getPosition(window);

	bool contains = _shape.getGlobalBounds().contains(
		static_cast<float>(mousePos.x),
		static_cast<float>(mousePos.y)
	);

	if (contains)
	{
		if (event.type == sf::Event::MouseButtonPressed &&
			event.mouseButton.button == sf::Mouse::Left)
		{
			_state = ButtonState::Pressed;
			_wasPressed = true;
		}
		else if (event.type == sf::Event::MouseButtonReleased &&
			event.mouseButton.button == sf::Mouse::Left &&
			_wasPressed)
		{
			if (_config.onClickAction)
				_config.onClickAction();
			_wasPressed = false;
			_state = ButtonState::Hovered;
		}
		else if (_state != ButtonState::Pressed)
		{
			_state = ButtonState::Hovered;
		}
	}
	else
	{
		_state = ButtonState::Normal;

		_wasPressed = false;

	}

	updateAppearance();
}

void Button::updateAppearance()
{

	sf::Color targetColor;

	switch (_state)
	{
	case ButtonState::Hovered:
		_shape.setFillColor(_config.hoverColor);
		break;
	case ButtonState::Pressed:
		_shape.setFillColor(_config.pressedColor);
		break;
	case ButtonState::Disabled:
		_shape.setFillColor(_config.disabledColor);
		break;
	default:
		_shape.setFillColor(_config.normalColor);
	}

	_shape.setFillColor(lerpColors(_shape.getFillColor(), targetColor, 0.1f));
}
