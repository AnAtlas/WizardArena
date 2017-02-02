#pragma once
#include "GUI_Element.hpp"

#include <unordered_map>

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

using Elements = std::unordered_map<std::string, GUI_Element*>;

class GUI_Manager;

class GUI_Interface : public GUI_Element {
	friend class GUI_Element;
	friend class GUI_Manager;

private:
	Elements elements;
	sf::Vector2f elementPadding;
	GUI_Interface* parent;
	GUI_Manager* guiManager;

	sf::RenderTexture* backdropTexture;
	sf::Sprite backdrop;

	//Movement
	sf::RectangleShape titleBar;
	sf::Vector2f moveMouseLast;
	bool showTitleBar;
	bool movable;
	bool beingMoved;
	bool focused;

	//Variable Size
	sf::RenderTexture* contentTexture;
	sf::Sprite content;
	sf::Vector2f contentSize;
	int scrollHorizontal;
	int scrollVertical;
	bool contentRedraw;
	void adjustContentSize(const GUI_Element* reference = nullptr);
	void setContentSize(const sf::Vector2f& vec);

	//Control Layer
	sf::RenderTexture* controlTexture;
	sf::Sprite control;
	bool controlRedraw;

	void defocusTextfields();

public:
	GUI_Interface(const std::string& name, GUI_Manager* guiManager);
	~GUI_Interface();

	void setPosition(const sf::Vector2f& pos);

	bool addElement(const GUI_ElementType& type, const std::string& name);
	GUI_Element* getElement(const std::string& name);
	bool removeElement(const std::string& name);

	bool hasParent();
	GUI_Manager* getManager();

	bool isInside(const sf::Vector2f& point);

	void focus();
	void deFocus();
	bool isFocused();

	void readIn(std::stringstream& stream);
	void onClick(const sf::Vector2f& mousePos);
	void onRelease();
	void onHover(const sf::Vector2f& mousePos);
	void onLeave();
	void onTextEntered(const char& ch);

	bool isBeingMoved();
	bool isMovable();
	void beginMoving();
	void stopMoving();

	void applyStyle();

	const sf::Vector2f& getPadding();
	void setPadding(const sf::Vector2f& padding);
	sf::Vector2f getGlobalPosition();
	const sf::Vector2f& getContentSize();

	void redraw();
	bool needsContentRedraw();
	void redrawContent();
	bool needsControlRedraw();
	void redrawControls();

	void toggleTitleBar();

	void update(float dT);
	void draw(sf::RenderTarget* target);

	void updateScrollHorizontal(unsigned int percent);
	void updateScrollVertical(unsigned int percent);
};
