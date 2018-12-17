#include<stdio.h>
#include<string>
#include<iostream>
#include <SFML\Graphics.hpp>

using namespace std;
#define WIDTH 1200
#define HEIGHT 800
//----------------------------------------------------------------


//----------------------------------------------------------------

void delay(int time) {
	sf::Time z;
	auto x = z.asMilliseconds();
	while (x-- > 0);
}


//----------------------------------------------------------------
void main() {

	//init window
	sf::RenderWindow window;
	window.create(sf::VideoMode(WIDTH, HEIGHT), "Grapher");

	//data variable
	sf::Event e;
	sf::Vector2i cPos;//mouse pos

	//my shapes
	auto circle = sf::CircleShape(100, 30);
	circle.setFillColor(sf::Color(255, 255, 0, 255));
	circle.setOrigin(100, 100);
	//create box
	bool isDrawBox = false;
	auto box = sf::RectangleShape(sf::Vector2f(100, 100));
	box.setOrigin(50, 50);


	while (window.isOpen())
	{
		window.clear();
	
		cPos = sf::Mouse::getPosition(window);


		//handle event "input"
		while (window.pollEvent(e)) {
			switch (e.type)
			{
			case sf::Event::EventType::MouseWheelScrolled:
				circle.rotate(e.mouseWheelScroll.delta);
				//	circle.setScale(originalScale.x+e.mouseWheelScroll.delta/2, originalScale.y + e.mouseWheelScroll.delta/2);
				break;


			case sf::Event::MouseButtonPressed:
				if (e.mouseButton.button == sf::Mouse::Button::Left) {
					circle.setFillColor(sf::Color::Red);
					
					isDrawBox = true;
				}
				if (e.mouseButton.button == sf::Mouse::Button::Right) {
					circle.setFillColor(sf::Color::Blue);
				}
				break;
			case sf::Event::MouseButtonReleased:
				if (e.mouseButton.button == sf::Mouse::Button::Left) {
					isDrawBox = false;
				}
				break;
			case sf::Event::Closed:
				window.close();
				break;
			default:
				break;
			}//switch
		}


		circle.setPosition(cPos.x, cPos.y);
		box.setPosition(cPos.x, cPos.y);

		sf::Vector2f circlePos = circle.getPosition();
		//auto circlePos = circle.getPosition();
		cout << circlePos.x;
		cout << circlePos.y;

		//draw every thing
		window.draw(circle);
		if (isDrawBox) {
			window.draw(box);

//isDrawBox = false;
		}
		window.display();


	}//while
}//main

//----------------------------------------------------------------
