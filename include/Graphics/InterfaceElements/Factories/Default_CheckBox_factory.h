#include <Graphics/InterfaceElements/Factories/CheckBox_factory.h>

class DefaultCheckBoxFactory : public CheckBoxFactory
{
private:

public:
	std::unique_ptr<CheckBox> createCheckBox(const sf::Font& font,
		const std::string& text,
		const sf::Vector2f& pos,
		unsigned int characterSize = 16) const override
	{

	}
};