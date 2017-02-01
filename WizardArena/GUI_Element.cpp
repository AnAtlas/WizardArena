#include "GUI_Element.hpp"
#include "GUI_Interface.hpp"
#include "GUI_Manager.hpp"
#include "SharedContext.hpp"

GUI_Element::GUI_Element(const std::string& name, const GUI_ElementType& type, GUI_Interface* owner)
	:name(name), type(type), owner(owner), state(GUI_ElementState::Neutral), needsRedraw(false), active(true), isControl(false)
{

}

GUI_Element::~GUI_Element() {
	releaseResources();
}

void GUI_Element::updateStyle(const GUI_ElementState& state, const GUI_Style& style) {
	//Resource management
	if (style.backgroundImage != this->style[state].backgroundImage) {
		releaseTexture(this->style[state].backgroundImage);
		requireTexture(style.backgroundImage);
	}
	if (style.glyph != this->style[state].glyph) {
		releaseTexture(this->style[state].glyph);
		requireTexture(style.textFont);
	}
	if (style.textFont != this->style[state].textFont) {
		releaseFont(this->style[state].textFont);
		requireFont(style.textFont);
	}
	// Style Application
	this->style[state] = style;
	if (state == this->state) {
		setRedraw(true);
		applyStyle();
	}
}

sf::Vector2f GUI_Element::getGlobalPosition() {
	sf::Vector2f pos = getPosition();
	if (owner == nullptr || owner == this)
		return pos;
	pos += owner->getGlobalPosition();
	if (isControl())
		return pos;
	pos.x -= owner->scrollHorizontal;
	pos.y -= owner->scrollVertical;
	return position;
}

bool GUI_Element::isInside(const sf::Vector2f& point) {
	sf::Vector2f pos = getGlobalPosition();
	return(point.x >= pos.x &&
		point.y >= pos.y &&
		point.x <= pos.x + style.at(state).size.x &&
		point.y <= pos.y + style.at(state).size.y);
}

void GUI_Element::setPosition(const sf::Vector2f& pos) {
	position = pos;
	if (owner == nullptr || owner == this)
		return;
	const auto& padding = owner->getPadding();
	if (position.x < padding.x)
		position.x = padding.x;
	if (position.y < padding.y)
		position.y = padding.y;
}

void GUI_Element::setState(const GUI_ElementState& state) {
	if (this->state == state)
		return;
	this->state = state;
	setRedraw(true);
}
void GUI_Element::applyStyle() {
	applyTextStyle();
	applyBackgroundStyle();
	applyGlyphStyle();
	if (owner != this && !isControl()) {
		owner->adjustContentSize(this);
	}
}

void GUI_Element::applyTextStyle() {
	FontManager* fonts = owner->getManager()->getContext()->fontManager;
	const GUI_Style& currentStyle = style[state];
	if (currentStyle.textFont != "") {
		visual.text.setFont(*fonts->getResource(currentStyle.textFont));
		visual.text.setColor(currentStyle.textColor);
		visual.text.setCharacterSize(currentStyle.textSize);
		if (currentStyle.textCenterOrigin) {
			sf::FloatRect rect = visual.text.getLocalBounds();
			visual.text.setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
		}
		else {
			visual.text.setOrigin(0.0f, 0.0f);
		}
	}
	visual.text.setPosition(position + currentStyle.textPadding);
}

void GUI_Element::applyBackgroundStyle() {
	TextureManager* textures = owner->getManager()->getContext()->textureManager;
	const GUI_Style& currentStyle = style[state];
	if (currentStyle.backgroundImage != "") {
		visual.backgroundImage.setTexture(*textures->getResource(currentStyle.backgroundImage));
		visual.backgroundImage.setColor(currentStyle.backgroundImageColor);
	}
	visual.backgroundImage.setPosition(position);
	visual.backgroundSolid.setSize(sf::Vector2f(currentStyle.size));
	visual.backgroundSolid.setFillColor(currentStyle.backgroundColor);
	visual.backgroundSolid.setPosition(position);
}

void GUI_Element::applyGlyphStyle() {
	const GUI_Style& currentStyle = style[state];
	TextureManager* textures = owner->getManager()->getContext()->textureManager;
	if (currentStyle.glyph != "") {
		visual.glyph.setTexture(*textures->getResource(currentStyle.glyph));
	}
	visual.glyph.setPosition(position + currentStyle.glyphPadding);
}
void GUI_Element::requireTexture(const std::string& name) {
	if (name == "")
		return;
	owner->getManager()->getContext()->textureManager->requireResource(name);
}

void GUI_Element::requireFont(const std::string& name) {
	if (name == "")
		return;
	owner->getManager()->getContext()->fontManager->requireResource(name);
}

void GUI_Element::releaseResources() {
	for (auto &itr : style) {
		releaseTexture(itr.second.backgroundImage);
		releaseTexture(itr.second.glyph);
		releaseFont(itr.second.textFont);
	}
}

void GUI_Element::releaseTexture(const std::string& name) {
	if (name == "")
		return;
	owner->getManager()->getContext()->textureManager->releaseResource(name);
}

void GUI_Element::releaseFont(const std::string& name) {
	if (name == "")
		return;
	owner->getManager()->getContext()->fontManager->releaseResource(name);
}

const sf::Vector2f& GUI_Element::getSize() {
	return style.at(state).size;
}

void GUI_Element::setActive(const bool& active) {
	if (active != this->active) {
		this->active = active;
		setRedraw(true);
	}
}

std::string GUI_Element::getText() {
	return visual.text.getString();
}

void GUI_Element::setText(const std::string& text) {
	visual.text.setString(text);
	setRedraw(true);
}