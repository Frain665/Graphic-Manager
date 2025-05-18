#include <Graphics/InterfaceElements/Checkbox.h>

CheckBox::CheckBox(const sf::Font& font, 
	const std::string& text,
	const sf::Vector2f& pos,
	unsigned int characterSize)
	:_isChecked(false), _font(font)
{
	if (!_font.loadFromFile(RESOURCES_DIR "Fonts/defaultFont.otf"))
	{
		throw FontException("Error loading the font!");
	}

	_box.setSize({ 100.f, 20.f });
	_box.setFillColor(sf::Color(200, 200, 200));
	_box.setOutlineThickness(2.f);
	_box.setOutlineColor(sf::Color::Black);

	_checkMark.setSize({ 12.f, 12.f });
	_checkMark.setFillColor(sf::Color::Blue);
	_checkMark.setPosition(pos.x + 4.f, pos.y + 4.f);

	_label.setFont(_font);
	_label.setString(text);
	_label.setCharacterSize(characterSize);
	_label.setFillColor(sf::Color::Black);

	setPosition(pos);
}

CheckBox::CheckBox(CheckBox&& other) noexcept
	:_isChecked(other._isChecked),
	_box(std::move(other._box)),
	_checkMark(std::move(other._checkMark)),
	_label(std::move(other._label)),
	_font(other._font),
	_callback(std::move(other._callback)) {}

CheckBox& CheckBox::operator=(CheckBox&& other) noexcept
{
	if (this != &other) 
	{
		_isChecked = other._isChecked;
		_box = std::move(other._box);
		_checkMark = std::move(other._checkMark);
		_label = std::move(other._label);
		_font = other._font;
		_callback = std::move(other._callback);
	}

	return *this;
}

void CheckBox::setPosition(const sf::Vector2f& pos)
{
	_box.setPosition(pos);
	_checkMark.setPosition(pos.x + 0.4f, pos.y + 0.4f);
	_label.setPosition(pos.x + 30.f, pos.y);
}

void CheckBox::setSize(const sf::Vector2f& size)
{
	_box.setSize(size);
	_checkMark.setSize(size - sf::Vector2f{ 0.8f, 0.8f });
}

void CheckBox::setChecked(bool checked)
{
	_isChecked = checked;
}

void CheckBox::setCallback(const std::function<void(bool)>& func)
{
	_callback = func;
}

bool CheckBox::getChecked() const
{
	return _isChecked;
}

sf::Vector2f CheckBox::getPosition() const
{
	return _box.getPosition();
}

sf::Vector2f CheckBox::getSize() const
{
	return _box.getSize();
}

sf::RectangleShape& CheckBox::getShape()
{
	return _box;
}

void CheckBox::draw(sf::RenderWindow& window)
{
	window.draw(_box);
	window.draw(_checkMark);
	window.draw(_label);
}

void CheckBox::handleEvent(const sf::RenderWindow& window, const sf::Event& event)
{

	auto now = std::chrono::steady_clock::now();

	if (event.type == sf::Event::MouseButtonPressed &&
		event.mouseButton.button == sf::Mouse::Left && 
		(now - _lastClickTime) > _clickDelay)
	{
		_lastClickTime = now;

		auto mousePosition = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
		auto contains = _box.getGlobalBounds().contains(mousePosition);

		if (contains)
		{
			_isChecked = !_isChecked;

			if (_isChecked) 
			{
				_box.setFillColor(_ACTIVE_BG_COLOR);    
				_checkMark.setFillColor(sf::Color::Green);
				_label.setFillColor(sf::Color::White);
			}
			else 
			{
				_box.setFillColor(_INACTIVE_BG_COLOR); 
				_checkMark.setFillColor(sf::Color::Blue); 
				_label.setFillColor(sf::Color::Black);      
			}

			if (_callback)
			{
				_callback(_isChecked);
			}
		}
	}
}
