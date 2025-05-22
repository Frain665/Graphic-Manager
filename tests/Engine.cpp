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
	DefaultCheckBoxFactory defaultCheckBoxFactory;

	_buttons.push_back(defaultButtonFactory.createButton("Generate", { 200, 50 }));
	_buttons.push_back(defaultButtonFactory.createButton("Something", { 200, 50 }));
	_buttons.push_back(defaultButtonFactory.createButton("Something", { 200, 50 }));
	_buttons.push_back(defaultButtonFactory.createButton("Something", { 200, 50 }));
	_buttons.push_back(defaultButtonFactory.createButton("Something", { 200, 50 }));

	auto createCheckboxCallback = [](const std::string& name)
		{
			return [name](bool checked)
				{

					if (name == "test")
					{
						std::cout << "^)" << std::endl;
					}
					else if (name == "test2")
					{
						std::cout << "............." << std::endl;
					}
					else
					{
						std::cout << name << " " << (checked ? "ON" : "OFF") << std::endl;
					}
				};
		};

	auto checkbox = defaultCheckBoxFactory.createCheckBox(_font, "test", { 300, 400 });
	checkbox->setCallback(createCheckboxCallback("test"));
	auto checkbox1 = defaultCheckBoxFactory.createCheckBox(_font, "test1", { 300, 400 });
	checkbox1->setCallback(createCheckboxCallback("test1"));
	auto checkbox2 = defaultCheckBoxFactory.createCheckBox(_font, "test2", { 300, 400 });
	checkbox2->setCallback(createCheckboxCallback("test2"));

	_checkboxes.push_back(std::move(checkbox));
	_checkboxes.push_back(std::move(checkbox1));
	_checkboxes.push_back(std::move(checkbox2));

	auto textField = std::make_unique<TextField>();
	textField->setSize(sf::Vector2f(300.f, 50.f));
	textField->setPosition(sf::Vector2f(400.f, 100.f));

	_textFields.emplace_back(std::move(textField));

	_volumeBar = std::make_unique<ProgressBar>(sf::Vector2f(300, 50), sf::Color(50, 50, 50), sf::Color::Green);
	_volumeBar->setPosition(sf::Vector2f(30.f, 300.f));
	_volumeBar->setOrientation(true);
	_volumeBar->showPercentage(true, _font, 16);
	_volumeBar->setMaxValue(100.f);
	_volumeBar->setValue(1.f);
	_volumeBar->_onValueChanged = [](float value)
		{
			std::cout << "Volume changed: " << value << "%\n";
		};

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
								offset.y + (static_cast<float>(i) * (size.y + 10))
							};
							container[i]->setPosition(finalOffset);
							container[i]->setSize(size);
						}
					});
			}
		};


	setupAnchors(_buttonAnchors, _buttons,
		AnchorHorizontal::CENTER,
		AnchorVertical::BOTTOM,
		sf::Vector2f(-70, -300),
		sf::Vector2f(200, 50)
	);


	setupAnchors(_checkboxAnchors, _checkboxes,
		AnchorHorizontal::LEFT,
		AnchorVertical::TOP,
		sf::Vector2f(-0, 20),
		sf::Vector2f(150, 15)
	);

	setupAnchors(_textFieldsAnchors, _textFields,
		AnchorHorizontal::CENTER,
		AnchorVertical::TOP,
		sf::Vector2f(0, 20),
		sf::Vector2f(300, 50)
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
	if (!_window) return;


	handleInput();

	_volumeBar->handleEvent(*_window, _event);

	const sf::Vector2u windowSize = _window->getSize();

	auto updateAnchors = [windowSize](auto& anchors)
		{
			for (auto& anchor : anchors)
			{
				if (anchor) anchor->update(windowSize);
			}
		};

	updateAnchors(_buttonAnchors);
	updateAnchors(_checkboxAnchors);
	updateAnchors(_textFieldsAnchors);

	for (auto const& textField : _textFields)
	{
		if (textField) textField->handleEvent(*_window, _event);
	}

	for (auto const& box : _checkboxes)
	{
		if (box) box->handleEvent(*_window, _event);
	}

	for (auto const& button : _buttons)
	{
		if (button) button->handleEvent(*_window, _event);
	}

	updateButtons();
}

void Engine::render()
{
	_window->clear();

	_volumeBar->draw(*_window);

	for (auto const& button : _buttons)
	{
		button->draw(*_window);
	}

	for (const auto& box : _checkboxes)
	{
		box->draw(*_window);
	}

	for (const auto& textField : _textFields)
	{
		textField->draw(*_window);
	}



	_window->display();
}

void Engine::updateButtons()
{
	if (_buttons.at(0)->isClicked())
	{
		std::cout << "Generate..." << std::endl;
	}
	else if (_buttons.at(1)->isClicked())
	{
		std::cout << "..." << std::endl;
	}
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
				_textFields[i]->handleEvent(*_window, _event);
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