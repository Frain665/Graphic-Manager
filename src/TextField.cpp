#include <Graphics/InterfaceElements/TextField.h>

TextField::TextField()
	: _isActive(false),
	_characterSize(24),
	_activeColor(sf::Color::White),
	_inactiveColor(sf::Color(180, 180, 180)),
	_maxLength(20)
{
	if (!_font.loadFromFile(RESOURCES_DIR "fonts/defaultFont.otf"))
	{
		throw FontException(RESOURCES_DIR "fonts/defaultFont.otf");
	}

	_text.setFont(_font);
	_text.setCharacterSize(_characterSize);
	_text.setFillColor(_inactiveColor);
	_text.setPosition(sf::Vector2f(50.f, 100.f));

	_background.setSize(sf::Vector2f(100.f, 50.f));
	_background.setFillColor(sf::Color::Transparent);
	_background.setOutlineThickness(2.f);
	_background.setOutlineColor(_inactiveColor);
}

const std::string& TextField::getText() const
{
	return _inputString;
}

void TextField::setCharacterSize(unsigned int characterSize)
{
	_characterSize = characterSize;
	_text.setCharacterSize(characterSize);
}

void TextField::setSize(const float& width, const float& height)
{
	_background.setSize(sf::Vector2f(width, height));
}

void TextField::setSize(const sf::Vector2f& size)
{
	_background.setSize(size);
}

void TextField::setMaxLength(unsigned int length)
{
	_maxLength = length;
}

void TextField::setText(const std::string& text)
{
	_inputString = text;
	_text.setString(_inputString);
}

void TextField::setPosition(const sf::Vector2f& pos)
{
	_background.setPosition(sf::Vector2f(pos));
	_text.setPosition(pos.x + 10.f, pos.y + 10);
} 

void TextField::handleEvent(const sf::RenderWindow& window, const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed &&
		event.mouseButton.button == sf::Mouse::Left &&
		(std::chrono::steady_clock::now() - _lastClickTime) > _clickDelay)
	{
		_lastClickTime = std::chrono::steady_clock::now();

		const auto mousePos = window.mapPixelToCoords(
			{ event.mouseButton.x, event.mouseButton.y },
			window.getView()
		);

		const bool wasActive = _isActive;

		_isActive = _background.getGlobalBounds().contains(mousePos);

		if (wasActive != _isActive)
		{
			_text.setFillColor(_isActive ? _activeColor : _inactiveColor);
			_background.setOutlineColor(_isActive ? _activeColor : _inactiveColor);
		}
		if (_isActive)
		{
			_keyRepeatClock.restart();
		}
	}

	if (_isActive && event.type == sf::Event::TextEntered)
	{
		handleTextInput(event.text.unicode);
	}

	if (_isActive && event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Enter ||
			event.key.code == sf::Keyboard::Tab)
		{
			_isActive = false;
			_text.setFillColor(_inactiveColor);
			_background.setOutlineColor(_inactiveColor);
		}
	}
}

void TextField::handleTextInput(sf::Uint32 unicode)
{
	if (_keyRepeatClock.getElapsedTime().asSeconds() > 100)
	{
		return;
	}

	if (unicode == '\b')
	{
		if (!_inputString.empty())
		{
			_inputString.pop_back();
		}
	}
	else if (unicode >= 32 && unicode <= 128 && _inputString.size() < _maxLength)
	{
		_inputString += static_cast<char>(unicode);
	}

	_text.setString(_inputString);
	_keyRepeatClock.restart();
}

void TextField::draw(sf::RenderWindow& window)
{
	window.draw(_background);
	window.draw(_text);
}
