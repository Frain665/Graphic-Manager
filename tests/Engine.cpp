#include "Engine.h"

void Engine::initVariables()
{
	_windowTitle = "Test";
	_window = nullptr;
	_event = {};
}

void Engine::uploadResources()
{
	if (!_font.loadFromFile(_fontPath))
	{
		throw FontException(_fontPath);
	}

	DefaultButtonFactory defaultButtonFactory(_font);

	_buttons.push_back(defaultButtonFactory.createButton("Generate", { 200, 50 }));
	_buttons.push_back(defaultButtonFactory.createButton("Copy", { 200, 50 }));

	auto createCheckboxCallback = [](const std::string& name)
		{
			return [name](bool checked) {
				std::cout << name << " " << (checked ? "ON" : "OFF") << std::endl;
				};
		};

	auto addCheckbox = [&](const std::string& name, sf::Vector2f pos)
		{
			auto cb = std::make_unique<CheckBox>(_font, name, pos);
			cb->setCallback(createCheckboxCallback(name));
			_checkboxes.push_back(std::move(cb));
		};

	addCheckbox("Enable some", { 400, 200 });
	addCheckbox("Enable some", { 400, 100 });
	addCheckbox("Enable some", { 400, 600 });
	addCheckbox("Enable some", { 400, 200 });
	addCheckbox("Enable some", { 400, 100 });
	addCheckbox("Enable some", { 400, 600 });
	addCheckbox("Enable some", { 400, 200 });
	addCheckbox("Enable some", { 400, 100 });
	addCheckbox("Enable some", { 400, 600 });

	_textField = std::make_shared <TextField>();
	_textField->setSize(sf::Vector2f(100.f, 50.f));
	_textField->setPosition(sf::Vector2f(400, 300.f));

	auto setupAnchors = [](auto& anchors, auto& container, auto... args) 
		{
		anchors.resize(container.size());
		for (size_t i = 0; i < container.size(); ++i) 
		{
			anchors[i].emplace(args..., [&container, i](const auto& offset, const auto& size)
				{
				if (i < container.size())
				{
			
					sf::Vector2f finalOffset = 
					{
						offset.x,
						offset.y + (i * (size.y + 10))
					};
					container[i]->setPosition(finalOffset);
					container[i]->getShape().setSize(size);
				}
				});
		}
		};


	setupAnchors(_buttonAnchors, _buttons,
		AnchorHorizontal::CENTER,
		AnchorVertical::BOTTOM,
		sf::Vector2f(-70, -100),
		sf::Vector2f(200, 50)
	);


	setupAnchors(_checkboxAnchors, _checkboxes,
		AnchorHorizontal::LEFT,
		AnchorVertical::TOP,
		sf::Vector2f(-0, 20),
		sf::Vector2f(150, 15)
	);

}

void Engine::initWindow()
{
	_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600),
		_windowTitle, sf::Style::Close | sf::Style::Titlebar);
	_window->setFramerateLimit(60);

	if (!_window)
	{
		throw WindowNotInitializedException("Game::initWindow() -> ");
	}
}

void Engine::init()
{
	initVariables();
	uploadResources();

	try
	{
		initWindow();
	}
	catch (const WindowNotInitializedException& exception)
	{
		std::cerr << "CRITICAL: " << exception.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (const FileLoadException& exception)
	{
		std::cerr << "RESOURCE ERROR: " << exception.what() << std::endl;
	}
	catch (const std::exception& exception)
	{
		std::cerr << "UNKNOWN ERROR: " << exception.what() << std::endl;
	}
}

void Engine::update()
{
	this->handleInput();

	_textField->handleEvent(*_window, _event);

	for (auto const& box : _checkboxes)
	{
		box->handleEvent(*_window, _event);
	}

	for (auto const& button : _buttons)
	{
		button->handleEvent(*_window, _event);
	}

	updateButtons();

	const sf::Vector2u windowSize = _window->getSize();

	size_t min_anchors = std::min(_buttonAnchors.size(), _checkboxAnchors.size());
	for (size_t i = 0; i < min_anchors; ++i) 
	{
		if (_buttonAnchors[i]) _buttonAnchors[i]->update(windowSize);
		if (_checkboxAnchors[i]) _checkboxAnchors[i]->update(windowSize);
	}


	for (size_t i = min_anchors; i < _buttonAnchors.size(); ++i) 
	{
		if (_buttonAnchors[i]) _buttonAnchors[i]->update(windowSize);
	}
	for (size_t i = min_anchors; i < _checkboxAnchors.size(); ++i) 
	{
		if (_checkboxAnchors[i]) _checkboxAnchors[i]->update(windowSize);
	}
}

void Engine::render()
{
	_window->clear();

	for (auto const& button : _buttons)
	{
		button->draw(*_window);
	}

	for (const auto& box : _checkboxes)
	{
		box->draw(*_window);
	}

	_textField->draw(*_window);

	_window->display();
}

void Engine::updateButtons()
{

}

void Engine::handleInput()
{

	while (_window->pollEvent(_event))
	{
		switch (_event.type)
		{
		case sf::Event::Closed:
			_window->close();
			break;
		case sf::Event::KeyPressed:
			if (_event.key.code == sf::Keyboard::Escape)
			{
				_window->close();
			}
			break;
		case sf::Event::Resized:
			for (size_t i = 0; i < _buttonAnchors.size() && _checkboxAnchors.size(); ++i)
			{
				_buttonAnchors[i]->update(_window->getSize());
				_checkboxAnchors[i]->update(_window->getSize());
			}
			break;
		default:
			break;
		}
	}
}

void Engine::run()
{
	init();

	while (_window->isOpen())
	{
		update();
		render();
	}
}

Engine& Engine::getInstance()
{
	static Engine instance;
	return instance;
}