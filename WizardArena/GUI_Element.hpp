#pragma once
#include <unordered_map>
#include <string>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "GUI_Style.hpp"

enum class GUI_ElementType{ Window, Label, Button, Scrollbar, Textfield };

using ElementStyles = std::unordered_map<GUI_ElementState, GUI_Style>;

class GUI_Interface;

class GUI_Element {
	friend class GUI_Interface;

protected:
	void applyTextStyle();
	void applyBackgroundStyle();
	void applyGlyphStyle();

	void requireTexture(const std::string& name);
	void requireFont(const std::string& name);
	void releaseTexture(const std::string& name);
	void releaseFont(const std::string& name);
	void releaseResources();
	std::string name;
	sf::Vector2f position;
	ElementStyles style; //Style of drawables
	GUI_Visual visual; // Drawable bits
	GUI_ElementType type;
	GUI_ElementState state;
	GUI_Interface* owner;

	bool needsRedraw;
	bool active;
	bool isControl;

public:
	GUI_Element(const std::string& name, const GUI_ElementType& type, GUI_Interface* owner);
	virtual ~GUI_Element();

	// Event Methods
	virtual void readIn(std::stringstream& stream) = 0;
	virtual void onClick(const sf::Vector2f& mousePos) = 0;
	virtual void onRelease() = 0;
	virtual void onHover(const sf::Vector2f& mousePos) = 0;
	virtual void onLeave() = 0;
	virtual void update(float dT) = 0;
	virtual void draw(sf::RenderTarget* target) = 0;

	virtual void updateStyle(const GUI_ElementState& state, const GUI_Style& style);
	virtual void applyStyle();

	//Getters and Setters
	GUI_ElementType getType();
	const std::string& getName();
	void setName(const std::string& name);
	const sf::Vector2f& getPosition() const;
	void setPosition(const sf::Vector2f& pos);
	const sf::Vector2f& getSize() const;
	GUI_ElementState getState();
	void setState(const GUI_ElementState& state);
	void setRedraw(const bool& redraw);
	bool needsRedraw();
	void setOwner(GUI_Interface* owner);
	GUI_Interface* getOwner();
	bool hasOwner();
	bool isActive();
	void setActive(const bool& active);
	bool isInside(const sf::Vector2f& point);
	sf::Vector2f getGlobalPosition();
	bool isControl();
	std::string getText();
	void setText(const std::string& text);

	friend std::stringstream& operator >> (std::stringstream& stream, GUI_Element& b) {
		b.readIn(stream);
		return stream;
	}
};