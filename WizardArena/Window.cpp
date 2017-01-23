#include "Window.hpp"

Window::Window(){ 
	setup("Window", sf::Vector2u(640,480));
}

Window::Window(const std::string& title, const sf::Vector2u& size){ 
	setup(title,size);
}

Window::~Window(){ window.close(); }

void Window::setup(const std::string& title, const sf::Vector2u& size){
	windowTitle = title;
	windowSize = size;
	fullscreen = false;
	done = false;
	isFocus = true;

	eventManager.addCallback(StateType(0),"Fullscreen_toggle",&Window::toggleFullscreen,this);
	eventManager.addCallback(StateType(0),"Window_close", &Window::close, this);

	create();
}

void Window::create(){
	sf::Uint32 style = sf::Style::Default;
	if(fullscreen){ style = sf::Style::Fullscreen; }

	window.create(sf::VideoMode(windowSize.x,windowSize.y,32),windowTitle,style);
	window.setFramerateLimit(72);
}

void Window::beginDraw(){ window.clear(sf::Color::Black); }
void Window::endDraw(){ window.display(); }

bool Window::isDone(){ return done; }
bool Window::isFullscreen(){ return fullscreen; }
bool Window::isFocused(){ return isFocus; }

sf::RenderWindow* Window::getRenderWindow(){ return &window; }
EventManager* Window::getEventManager(){ return &eventManager; }
sf::Vector2u Window::getWindowSize(){ return windowSize; }

void Window::toggleFullscreen(EventDetails* l_details){
	fullscreen = !fullscreen;
	window.close();
	create();
}

void Window::close(EventDetails* l_details){ done = true; }

void Window::update(){
	sf::Event event;

	while(window.pollEvent(event)){
		if (event.type == sf::Event::LostFocus){
			isFocus = false; 
			eventManager.setFocus(false); 
		}
		else if (event.type == sf::Event::GainedFocus){
			isFocus = true; 
			eventManager.setFocus(true); 
		}
		eventManager.handleEvent(event);
	}

	eventManager.update();
}

sf::FloatRect Window::getViewSpace() {
	sf::Vector2f viewCenter = window.getView().getCenter();
	sf::Vector2f viewSize = window.getView().getSize();
	sf::Vector2f viewSizeHalf(viewSize.x / 2, viewSize.y / 2);
	sf::FloatRect viewSpace(viewCenter - viewSizeHalf, viewSize);
	return viewSpace;
}