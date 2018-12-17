#include<iostream>
using namespace std;
#include<SFML\Graphics.hpp>
#include<SFML\Audio.hpp>
#include"Point.h"

#define screenWidth 600
#define screenHeight 400
#define dotSize 6
#define sizeI 5 //Points per row
#define sizeJ 5 //Points per column

Point **points;
int **boardStatus;
int currPlayer, p1Score, p2Score;
int startIndexI, endIndexI, startIndexJ, endIndexJ;
int lastHoveredStartI, lastHoveredStartJ, lastHoveredEndI, LastHoveredEndJ;

//Prototypes
bool isBetweenPoints(int, int);
void togglePlayer();
bool isSquare(int, int);

int main(int argc, char ** argv){
	currPlayer = 1; p1Score = 0; p2Score = 0;
	startIndexI = -1; endIndexI = -1; startIndexJ = -1; endIndexJ = -1;
	
	points = new Point*[sizeI];
	boardStatus = new int*[sizeI];
	for (int i = 0; i < sizeI; i++) {
		points[i] = new Point[sizeJ];
		boardStatus[i] = new int[sizeJ];
		for (int j = 0; j < sizeJ; j++) {
			boardStatus[i][j] = 0; //0: empty | 1: Player 1 | 2: Player 2
		}
	}

	//Calculates position of each dot
	int rowStep = screenWidth / sizeJ; int rowCounter = rowStep / 2;
	int colStep = screenHeight / sizeI; int colCounter = colStep / 1.5;
	for (int i = 0; i < sizeI; i++) {
		for (int j = 0; j < sizeJ; j++) {
			points[i][j].setPosition(rowCounter, colCounter);
			rowCounter += rowStep;
		}
		colCounter += colStep / 1.5;
		rowCounter = rowStep / 2;
	}
	
	//init
	sf::Event event;
	sf::RenderWindow renderWindow(sf::VideoMode(screenWidth, screenHeight), "Dots & Squares");
	sf::SoundBuffer buffer;
	sf::Sound sound;
	////Music
	buffer.loadFromFile("ChillingMusic.wav");
	sound.setBuffer(buffer);
	if (sound.getStatus() != sf::Music::Status::Playing)
		sound.play();
	//Background
	sf::Texture texture;
	texture.loadFromFile("BG.png");
	sf::Sprite BG(texture);
	//Dots on screen
	sf::CircleShape dots[sizeI][sizeJ];
	for (int i = 0; i < sizeI; i++) {
		for (int j = 0; j < sizeJ; j++) {
			dots[i][j].setRadius(dotSize);
			dots[i][j].setOrigin(dots[i][j].getRadius(), dots[i][j].getRadius());
			dots[i][j].setFillColor(sf::Color::Black);
			dots[i][j].setPosition(points[i][j].getX(), points[i][j].getY());
		}
	}
	//Marks
	sf::Texture blue, red;
	blue.loadFromFile("P2.png");
	red.loadFromFile("P1.png");
	sf::Sprite marks[sizeI-1][sizeJ-1];
	for (int i = 0; i < sizeI-1; i++) {
		for (int j = 0; j < sizeJ-1; j++) {
			marks[i][j].setPosition(points[i][j].getX(), points[i][j].getY());
		}
	}
	//Hover lines
	sf::RectangleShape horLine(sf::Vector2f(rowStep, 5));
	sf::RectangleShape verLine(sf::Vector2f(5, colStep / 1.5));
	verLine.setFillColor(sf::Color(0, 0, 0, 0));
	horLine.setFillColor(sf::Color(0, 0, 0, 0));
	//Actual lines
	sf::RectangleShape horLines[sizeI][sizeI-1];
	sf::RectangleShape verLines[(sizeJ - 1)][sizeJ];
	for (int i = 0; i < sizeI; i++) {
		for (int j = 0; j < sizeI-1; j++) {
			horLines[i][j].setSize(sf::Vector2f(rowStep, 5));
			horLines[i][j].setFillColor(sf::Color(0, 0, 0, 0));
		}
		
	}
	for (int i = 0; i < (sizeJ - 1); i++) {
		for (int j = 0; j < sizeJ; j++) {
			verLines[i][j].setSize(sf::Vector2f(5, colStep / 1.5));
			verLines[i][j].setFillColor(sf::Color(0, 0, 0, 0));
		}
	}
		
	while (renderWindow.isOpen()) {
		
		if (sound.getStatus() != sf::Music::Status::Playing)
			sound.play();

		while (renderWindow.pollEvent(event)) {

			if (event.type == sf::Event::EventType::Closed) //Closing
				renderWindow.close();

			if (event.type == sf::Event::MouseMoved){ //Hover
				
				if (isBetweenPoints(sf::Mouse::getPosition(renderWindow).x, sf::Mouse::getPosition(renderWindow).y)) {
					
					if (startIndexI == endIndexI) {
						horLine.setPosition(sf::Vector2f(points[startIndexI][startIndexJ].getX(), points[startIndexI][startIndexJ].getY()));
						horLine.setFillColor(sf::Color(0, 0, 0, 100));
						verLine.setFillColor(sf::Color(0, 0, 0, 0));
						lastHoveredEndI = endIndexI; LastHoveredEndJ = endIndexJ; lastHoveredStartI = startIndexI; lastHoveredStartJ = startIndexJ;
					}
					else if (startIndexJ == endIndexJ) {
						verLine.setPosition(sf::Vector2f(points[startIndexI][startIndexJ].getX(), points[startIndexI][startIndexJ].getY()));
						verLine.setFillColor(sf::Color(0, 0, 0, 100));
						horLine.setFillColor(sf::Color(0, 0, 0, 0));
						lastHoveredEndI = endIndexI; LastHoveredEndJ = endIndexJ; lastHoveredStartI = startIndexI; lastHoveredStartJ = startIndexJ;
					}

				}
			}

			if (sf::Event::MouseButtonReleased) { //Click

				if (event.mouseButton.button == sf::Mouse::Button::Left) {
					
					if (lastHoveredStartI == lastHoveredEndI) {
						//Horizontal lines
						for (int i = 0; i < sizeI; i++) {
							for (int j = 0; j < sizeI-1; j++) {
								if (lastHoveredStartI == i && lastHoveredStartJ == j && lastHoveredEndI == i && LastHoveredEndJ == j + 1 && points[i][j].getRight() == false) {
									horLines[i][j].setPosition(sf::Vector2f(points[i][j].getX(), points[i][j].getY()));
									horLines[i][j].setFillColor(sf::Color(0, 0, 0, 225));
									points[i][j].rightOn(); points[i][j+1].leftOn();
									if (((i != 0 && j != sizeJ - 1) && points[i][j].getUp() && points[i][j + 1].getUp() && points[i - 1][j].getRight()) || ((i != sizeI - 1 && j != sizeJ - 1) && points[i][j].getDown() && points[i][j + 1].getDown() && points[i + 1][j].getRight())) {
										if ((i != 0 && j != sizeJ - 1) && points[i][j].getUp() && points[i][j + 1].getUp() && points[i - 1][j].getRight()) {
											if (currPlayer == 1) { p1Score++; currPlayer = 1; marks[i - 1][j].setTexture(blue); }
											else if (currPlayer == 2) { p2Score++; currPlayer = 2; marks[i - 1][j].setTexture(red); }
										}
										if ((i != sizeI - 1 && j != sizeJ - 1) && points[i][j].getDown() && points[i][j + 1].getDown() && points[i + 1][j].getRight()) {
											if (currPlayer == 1) { p1Score++; currPlayer = 1; marks[i][j].setTexture(blue); }
											else if (currPlayer == 2) { p2Score++; currPlayer = 2; marks[i][j].setTexture(red); }
										}
									}
									else {
										togglePlayer();
									}		
								}
							}
						}
					}
					else if (LastHoveredEndJ == lastHoveredStartJ) {
						//Vertical lines
						for (int i = 0; i < sizeJ - 1; i++) {
							for (int j = 0; j < sizeJ; j++) {
								if (lastHoveredStartI == i && lastHoveredStartJ == j && lastHoveredEndI == i + 1 && LastHoveredEndJ == j && points[i][j].getDown() == false) {
									verLines[i][j].setPosition(sf::Vector2f(points[i][j].getX(), points[i][j].getY()));
									verLines[i][j].setFillColor(sf::Color(0, 0, 0, 225));
									points[i][j].downOn(); points[i+1][j].upOn();
									if (((i != sizeI - 1 && j != sizeJ - 1) && points[i][j].getRight() && points[i + 1][j].getRight() && points[i][j + 1].getDown()) || ((i != sizeI - 1 && j != 0) && points[i][j].getLeft() && points[i + 1][j].getLeft() && points[i][j - 1].getDown())) {
										if ((i != sizeI - 1 && j != sizeJ - 1) && points[i][j].getRight() && points[i + 1][j].getRight() && points[i][j + 1].getDown()) {
											if (currPlayer == 1) { p1Score++; currPlayer = 1; marks[i][j].setTexture(blue); }
											else if (currPlayer == 2) { p2Score++; currPlayer = 2; marks[i][j].setTexture(red); }
										}
										if ((i != sizeI - 1 && j != 0) && points[i][j].getLeft() && points[i + 1][j].getLeft() && points[i][j - 1].getDown()) {
											if (currPlayer == 1) { p1Score++; currPlayer = 1; marks[i][j - 1].setTexture(blue); }
											else if (currPlayer == 2) { p2Score++; currPlayer = 2; marks[i][j - 1].setTexture(red); }
										}
									}
									else {
										togglePlayer();
									}
								}
							}
						}
					}
					
				}
			}
				
		}

		//Players' scores
		string s1 = "Player 1: " + std::to_string(p1Score);
		string s2 = " | Player 2: " + std::to_string(p2Score);
		sf::Font font;
		sf::Text p1Stats, p2Stats;
		if (font.loadFromFile("ELEPHNT.TTF")) {
			p1Stats.setFont(font);
			p1Stats.setString(s1);
			p1Stats.setCharacterSize(40);
			if(currPlayer == 1) p1Stats.setFillColor(sf::Color(0, 0, 225, 225));
			else p1Stats.setFillColor(sf::Color(0, 0, 0, 225));
			p1Stats.setPosition(60, 300);
			p2Stats.setFont(font);
			p2Stats.setString(s2);
			p2Stats.setCharacterSize(40);
			if (currPlayer == 2) p2Stats.setFillColor(sf::Color(225, 0, 0, 225));
			else p2Stats.setFillColor(sf::Color(0, 0, 0, 225));
			p2Stats.setPosition(270, 300);
		}

		renderWindow.clear();
		renderWindow.draw(BG);
		renderWindow.draw(horLine);
		renderWindow.draw(verLine);
		for (int i = 0; i < sizeI; i++) {
			for (int j = 0; j < sizeJ; j++) {
				renderWindow.draw(dots[i][j]);
			}
		}
		for (int i = 0; i < sizeI; i++) {
			for (int j = 0; j < sizeI-1; j++) {
				renderWindow.draw(horLines[i][j]);
			}
		}
		for (int i = 0; i < (sizeJ - 1); i++) {
			for (int j = 0; j < sizeJ; j++) {
				renderWindow.draw(verLines[i][j]);
			}
		}
		for (int i = 0; i < sizeI-1; i++) {
			for (int j = 0; j < sizeJ-1; j++) {
				renderWindow.draw(marks[i][j]);
			}
		}
		renderWindow.draw(p1Stats);
		renderWindow.draw(p2Stats);
		renderWindow.display();
	}
	
	return 0;
}

bool isBetweenPoints(int x, int y) {
	//startIndexI = -1; startIndexJ = -1; endIndexI = -1; endIndexJ = -1;
	for (int i = 0; i < sizeI; i++) {
		for (int j = 0; j < sizeJ; j++) {
			if (x >= points[i][j].getX() && y >= points[i][j].getY()) {
				startIndexI = i;
				startIndexJ = j;
			}
			if (x <= points[i][j].getX() && y <= points[i][j].getY() && ((i == startIndexI + 1 && j == startIndexJ) || (i == startIndexI && j == startIndexJ + 1))) {
				endIndexI = i;
				endIndexJ = j;
				return true;
			}
		}
	}
	return false;
}

void togglePlayer() {
	if (currPlayer == 1) currPlayer = 2;
	else if (currPlayer == 2) currPlayer = 1;
}