#include <SFML\Graphics.hpp>

using namespace sf;

int main() {
	RenderWindow window(VideoMode(400, 400), "Asteroids");
	RectangleShape shape(Vector2f(100, 100));
	shape.setFillColor(Color::Green);
	shape.setPosition(100, 100);
	while (window.isOpen()) {
		Event event;
		while(window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
		}
		window.clear();
		window.draw(shape);
		window.display();
	}
	return EXIT_SUCCESS;
}