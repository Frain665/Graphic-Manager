#include <Graphics/InterfaceElements/ProgressBar.h>

void ProgressBar::updateFill()
{
	float percentage = std::clamp(_currentValue / _maxValue, 0.f, 1.f);
	sf::Vector2f newSize;

	if (_isVertical)
	{
		newSize =
		{
			_background.getSize().x,
			_background.getSize().y * percentage
		};
		_fill.setPosition(
			_background.getPosition().x,
			_background.getPosition().y + _background.getSize().y - newSize.y
		);
	}
	else {
		newSize =
		{
			_background.getSize().x * percentage,
			_background.getSize().y
		};
		_fill.setPosition(_background.getPosition());
	}

	_fill.setSize(newSize);

	if (_showText)
	{
		_text.setString(std::to_string(static_cast<int>(percentage * 100)) + "%");
		updateTextPosition();
	}
	if (_useGradient)
	{
		sf::Vector2f size = _fill.getSize();
		_gradientVertices[0].position = sf::Vector2f(0, 0);
		_gradientVertices[1].position = sf::Vector2f(size.x, 0);
		_gradientVertices[2].position = sf::Vector2f(size.x, size.y);
		_gradientVertices[3].position = sf::Vector2f(0, size.y);
	}
}

void ProgressBar::update(float deltaTime)
{
	if (_smoothness > 0.f &&
		std::abs(_currentValue - _targetValue) > 0.01f)
	{
		_currentValue = std::lerp(_currentValue, _targetValue, _smoothness * deltaTime);
		updateFill();
	}
}

ProgressBar::ProgressBar(const sf::Vector2f& size,
	const sf::Color& bgColor,
	const sf::Color& fillColor)
	:_maxValue(100.f),
	_currentValue(0.f),
	_targetValue(0.f),
	_isVertical(false),
	_smoothness(0.f),
	_showText(false)
{
	_background.setSize(size);
	_background.setFillColor(bgColor);
	_background.setOutlineThickness(0.f);

	_fill.setSize({ 0.f, size.y });
	_fill.setFillColor(fillColor);
	_fill.setPosition(0.f, 0.f);

	_text.setString("0%");
	_text.setCharacterSize(16);
	_text.setFillColor(sf::Color::White);

	updateTextPosition();

	_border.setSize(size);
	_border.setFillColor(sf::Color::Transparent);
	_border.setOutlineThickness(0.f);
	_border.setOutlineColor(sf::Color::White);
}

ProgressBar::ProgressBar(ProgressBar&& other)
	:_gradientVertices(std::move(other._gradientVertices)),
	_background(std::move(other._background)),
	_fill(std::move(other._fill)),
	_border(std::move(other._border)),
	_text(std::move(other._text)),
	_font(other._font),
	_maxValue(other._maxValue),
	_currentValue(other._currentValue),
	_targetValue(other._targetValue),
	_smoothness(other._smoothness),
	_isVertical(other._isVertical),
	_showBorder(other._showBorder),
	_showText(other._showText),
	_useGradient(other._useGradient),
	_isDragging(other._isDragging),
	_isEnabled(other._isEnabled),
	_gradientFill(std::move(other._gradientFill)),
	_onComplete(std::move(other._onComplete)),
	_lastClickTime(other._lastClickTime),
	_clickDelay(other._clickDelay)
{
	other._maxValue = 0;
	other._currentValue = 0;
	other._targetValue = 0;
	other._smoothness = 0;
	other._isVertical = false;
	other._showBorder = false;
	other._showText = true;
	other._useGradient = false;
	other._isDragging = false;
	other._isEnabled = true;

}

ProgressBar& ProgressBar::operator=(ProgressBar&& other)
{
	if (this != &other)
	{
		_gradientVertices = std::move(other._gradientVertices);
		_background = std::move(other._background);
		_fill = std::move(other._fill);
		_border = std::move(other._border);
		_text = std::move(other._text);
		_font = other._font;

		_maxValue = other._maxValue;
		_currentValue = other._currentValue;
		_targetValue = other._targetValue;
		_smoothness = other._smoothness;
		_isVertical = other._isVertical;
		_showBorder = other._showBorder;
		_showText = other._showText;
		_useGradient = other._useGradient;
		_isDragging = other._isDragging;
		_isEnabled = other._isEnabled;
		_gradientFill = std::move(other._gradientFill);
		_onComplete = std::move(other._onComplete);
		_lastClickTime = other._lastClickTime;
		_clickDelay = other._clickDelay;

		other._maxValue = 0;
		other._currentValue = 0;
		other._targetValue = 0;
		other._smoothness = 0;
		other._isVertical = false;
		other._showBorder = false;
		other._showText = true;
		other._useGradient = false;
		other._isDragging = false;
		other._isEnabled = true;
	}
	return *this;
}

void ProgressBar::setValue(float value)
{
	if (!std::isfinite(value)) {
		throw std::invalid_argument("Progress value must be finite");
	}

	float newValue = std::clamp(value, 0.f, _maxValue);

	if (std::abs(_currentValue - newValue) > std::numeric_limits<float>::epsilon())
	{
		_currentValue = newValue;
		updateFill();

		if (_onValueChanged)
		{
			_onValueChanged(_currentValue);
		}

		if (_currentValue >= _maxValue - std::numeric_limits<float>::epsilon() && _onComplete)
		{
			_onComplete();
		}
	}
}

void ProgressBar::setMaxValue(float maxValue)
{
	if (maxValue <= std::numeric_limits<float>::epsilon()) {
		throw std::invalid_argument("Max value must be greater than zero");
	}

	if (std::abs(_maxValue - maxValue) > std::numeric_limits<float>::epsilon()) {
		_maxValue = maxValue;
		_currentValue = std::min(_currentValue, _maxValue);
		updateFill();
	}
}

void ProgressBar::setOrientation(bool isVertical)
{
	if (_isVertical != isVertical)
	{
		_isVertical = isVertical;

		float progress = getPercentage() / 100.f;

		sf::Vector2f newSize = _background.getSize();
		if (_isVertical)
		{
			newSize = { newSize.y, newSize.x };
		}

		_background.setSize(newSize);
		updateFill();
	}
}

void ProgressBar::setPosition(const sf::Vector2f& pos)
{
	_background.setPosition(pos);

	if (_isVertical)
	{
		_fill.setPosition(
			pos.x,
			pos.y + _background.getSize().y - _fill.getSize().y
		);
	}
	else
	{
		_fill.setPosition(pos);
	}

	_border.setPosition(pos);

	if (_showText)
	{
		updateTextPosition();
	}
}

void ProgressBar::enableBorder(bool enable, const sf::Color& color, float thickness)
{
	if (enable)
	{
		_border.setOutlineColor(color);
		_border.setOutlineThickness(thickness);

		_border.setSize(_background.getSize());
		_border.setPosition(_background.getPosition());

		_background.setSize({
			_background.getSize().x - 2 * thickness,
			_background.getSize().y - 2 * thickness
			});
		_background.setPosition(
			_background.getPosition().x + thickness,
			_background.getPosition().y + thickness
		);
	}
	else
	{
		_border.setOutlineThickness(0.f);
		_background.setSize({
			_background.getSize().x + 2 * _border.getOutlineThickness(),
			_background.getSize().y + 2 * _border.getOutlineThickness()
			});
		_background.setPosition(
			_background.getPosition().x - _border.getOutlineThickness(),
			_background.getPosition().y - _border.getOutlineThickness()
		);
	}

	updateFill();
}

void ProgressBar::setSmoothness(float smoothness)
{
	_smoothness = std::clamp(smoothness, 0.0f, 1.0f);

	if (_smoothness <= 0.01f) {
		_currentValue = _targetValue;
		updateFill();
	}
}

void ProgressBar::setFillGradient(const sf::Color& start, const sf::Color& end)
{
	_gradientVertices.clear();

	_gradientVertices.append(sf::Vertex(
		sf::Vector2f(0, 0),
		start
	));

	_gradientVertices.append(sf::Vertex(
		sf::Vector2f(_background.getSize().x, 0),
		start
	));

	_gradientVertices.append(sf::Vertex(
		sf::Vector2f(_background.getSize().x, _background.getSize().y),
		end
	));

	_gradientVertices.append(sf::Vertex(
		sf::Vector2f(0, _background.getSize().y),
		end
	));

	_useGradient = true;

	updateFill();
}

sf::Color ProgressBar::lerpColors(const sf::Color& a, const sf::Color& b, float t)
{
	return sf::Color();
}

float ProgressBar::getPercentage() const
{
	return (_maxValue > 0) ? (_currentValue / _maxValue) * 100.f : 0.f;
}

void ProgressBar::updateTextPosition()
{
	if (!_showText) return;

	sf::FloatRect textBounds = _text.getLocalBounds();
	_text.setOrigin(textBounds.left + textBounds.width / 2.0f,
		textBounds.top + textBounds.height / 2.0f);

	sf::Vector2f center = {
		_background.getPosition().x + _background.getSize().x / 2.0f,
		_background.getPosition().y + _background.getSize().y / 2.0f
	};

	_text.setPosition(center);
}

void ProgressBar::updatePercentageText()
{
	if (!_showText) return;

	int percentage = static_cast<int>(std::round((_currentValue / _maxValue) * 100));
	_text.setString(std::to_string(percentage) + "%");
}

void ProgressBar::showPercentage(bool show, const sf::Font& font, unsigned int charSize)
{
	_showText = show;

	if (_showText)
	{
		_text.setFont(font);
		_text.setCharacterSize(charSize);

		const sf::Color bgColor = _background.getFillColor();
		_text.setFillColor(
			(bgColor.r + bgColor.g + bgColor.b > 384) ?
			sf::Color::Black : sf::Color::White
		);

		updatePercentageText();
		updateTextPosition();
	}
	else
	{
		_text.setString("");
	}
}

void ProgressBar::updateProgressFromMouse(const sf::Vector2f& mousePos)
{
	const sf::FloatRect bounds = _background.getGlobalBounds();
	float progress = 0.f;

	if (_isVertical)
	{
		const float relativeY = mousePos.y - bounds.top;
		progress = 1.f - (relativeY / bounds.height);
	}
	else
	{
		const float relativeX = mousePos.x - bounds.left;
		progress = relativeX / bounds.width;
	}

	setValue(progress * _maxValue);

	if (_onValueChanged) _onValueChanged(_currentValue);
}

void ProgressBar::draw(sf::RenderWindow& window)
{
	window.draw(_background);
	window.draw(_fill);

	if (_useGradient)
	{
		// window.transform = _fill.getTransform();
		window.draw(_gradientVertices);
	}
	else
	{
		window.draw(_fill);
	}

	if (_border.getOutlineThickness() > 0.f)
	{
		window.draw(_border);
	}

	if (_showText)
	{
		window.draw(_text);
	}
}

void ProgressBar::handleEvent(const sf::RenderWindow& window, const sf::Event& event)
{
	const auto mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	const bool isHovered = _background.getGlobalBounds().contains(mousePos);

	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Left &&
			(std::chrono::steady_clock::now() - _lastClickTime) > _clickDelay)
		{
			_lastClickTime = std::chrono::steady_clock::now();

			if (isHovered)
			{
				_isDragging = true;
				updateProgressFromMouse(mousePos);
			}
		}
	}
	else if (event.type == sf::Event::MouseButtonReleased)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			_isDragging = false;
		}
	}
	else if (event.type == sf::Event::MouseMoved && _isDragging)
	{
		updateProgressFromMouse(mousePos);
	}
}
