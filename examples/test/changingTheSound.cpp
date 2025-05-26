#ifndef EXAMPLE_HPP
#define EXAMPLE_HPP

#include <iostream>
#include <random>
#include <optional>
#include <memory>
#include <string>
#include <vector>

#include <GraphicsManager.h>

std::vector<std::unique_ptr<Button>> _buttons;
std::vector<std::unique_ptr<CheckBox>> _checkboxes;
std::vector<std::unique_ptr<TextField>> _textFields;

std::unique_ptr<ProgressBar> _volumeBar;

std::vector<std::optional<AnchoredElement>> _buttonAnchors;
std::vector<std::optional<AnchoredElement>> _checkboxAnchors;
std::vector<std::optional<AnchoredElement>> _textFieldsAnchors;

sf::Font font;


void updateButtons()
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

void uploadResources()
{
	if (!font.loadFromFile(RESOURCES_DIR "Fonts/defaultFont"))
	{
		throw FontException("Fonts");
	}

	DefaultButtonFactory defaultButtonFactory(font);
	DefaultCheckBoxFactory defaultCheckBoxFactory;

	_buttons.push_back(defaultButtonFactory.createButton("some kind of method", { 400, 50 }));
	_buttons.push_back(defaultButtonFactory.createButton("some kind of method", { 400, 50 }));
	_buttons.push_back(defaultButtonFactory.createButton("some kind of method", { 400, 50 }));
	_buttons.push_back(defaultButtonFactory.createButton("some kind of method", { 400, 50 }));
	_buttons.push_back(defaultButtonFactory.createButton("some kind of method", { 400, 50 }));

	auto createCheckboxCallback = [](const std::string& name)
		{
			return [name](bool checked)
				{

					if (name == "checkBox")
					{
						std::cout << ":)" << std::endl;
					}
					else if (name == "checkBox2")
					{
						std::cout << ":(" << std::endl;
					}
					else
					{
						std::cout << name << " " << (checked ? "ON" : "OFF") << std::endl;
					}
				};
		};

	auto checkbox = defaultCheckBoxFactory.createCheckBox(font, "checkBox", { 300, 400 });
	checkbox->setCallback(createCheckboxCallback("checkBox"));
	auto checkbox1 = defaultCheckBoxFactory.createCheckBox(font, "checkBox1", { 300, 400 });
	checkbox1->setCallback(createCheckboxCallback("checkBox1"));
	auto checkbox2 = defaultCheckBoxFactory.createCheckBox(font, "checkBox2", { 300, 400 });
	checkbox2->setCallback(createCheckboxCallback("checkBox2"));
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
	_volumeBar->showPercentage(true, font, 16);
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
		sf::Vector2f(240, 50)
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

int test()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Example");

	uploadResources();

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
		}

		updateButtons();

		window.clear();

		window.clear();

		_volumeBar->draw(window);

		for (auto const& button : _buttons)
		{
			button->draw(window);
		}

		for (const auto& box : _checkboxes)
		{
			box->draw(window);
		}

		for (const auto& textField : _textFields)
		{
			textField->draw(window);
		}

		window.display();
	}

	return EXIT_SUCCESS;
}

#endif //EXAMPLE_HPP