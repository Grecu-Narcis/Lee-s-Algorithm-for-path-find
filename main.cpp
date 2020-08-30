//Created by Narcis Grecu

#include <SFML/Graphics.hpp>
#include <iostream>
#include <Windows.h>
#include <queue>
#include <string>
#include <stack>

using namespace std;
using namespace sf;

int N = 14, M = 10;
int width = N * 50 + 5, height = M * 50 + 5;

RenderWindow window(VideoMode(width, height + 50), "Lee's Algorithm");

Font font;
Text text;


pair<int, int> startPoint, stopPoint;
bool hasSetedStartPoint = 0, hasSetedStopPoint = 0;

bool hasFinishedAlgorithm = 0;

int Lee[50][50];


//		 FOR Knight directions (Chess)
//int dx[] = { -2, -2, -1, 1, 2,  2,  1, -1 };
//int dy[] = { -1,  1,  2, 2, 1, -1, -2, -2 };


int dx[] = { -1, 0, 1,  0, -1, 1,  1, -1 };
int dy[] = {  0, 1, 0, -1,  1, 1, -1, -1 };
const int directionsNumber = 8;

queue<pair<int, int>> myQueue;



void drawTable();
void displayRoad();


bool isOkForLee(pair<int, int> point) {
	if (point.first >= 0 && point.first < M && point.second >= 0 && point.second < N && !Lee[point.first][point.second])
		return true;
	return false;
}


bool isOkForRoad(pair<int, int> point) {
	if (point.first >= 0 && point.first < M && point.second >= 0 && point.second < N)
		return true;
	return false;
}

void LeeAlgorithm() {
	Lee[startPoint.first][startPoint.second] = 1;
	myQueue.push(startPoint);

	bool ok = 0;

	pair<int, int> currentPoint, nextPoint;

	while (!myQueue.empty() && !ok) {
		currentPoint = myQueue.front();
		myQueue.pop();
		Sleep(25);
		drawTable();

		for (int i = 0; i < directionsNumber; i++) {
			nextPoint.first = currentPoint.first + dx[i];
			nextPoint.second = currentPoint.second + dy[i];

			if (isOkForLee(nextPoint)) {
				Lee[nextPoint.first][nextPoint.second] = Lee[currentPoint.first][currentPoint.second] + 1;
				myQueue.push(nextPoint);

				if (nextPoint == stopPoint)
					ok = 1;
			}
		}
		//Sleep(5000);
	}

	int stepsNumber = Lee[stopPoint.first][stopPoint.second] - 1;

	if (Lee[stopPoint.first][stopPoint.second] == 0)
		text.setString("It's impossible to reach stop point!");

	else {
		displayRoad();
		text.setString("You need " + to_string(stepsNumber) + " steps to reach stop point!");
	}

	hasFinishedAlgorithm = 1;
}

void displayRoad() {
	text.setString("Generating road...");

	stack<pair<int, int>> road;
	road.push(stopPoint);

	pair<int, int> point;

	while (road.top().first != startPoint.first || road.top().second != startPoint.second) {
		for (int i = 0; i < directionsNumber; i++) {
			point.first = road.top().first + dx[i];
			point.second = road.top().second + dy[i];

			if (isOkForRoad(point) && Lee[point.first][point.second] == Lee[road.top().first][road.top().second] - 1) {
				road.push(point);
				break;
			}
		}
	}

	while (!road.empty()) {
		Lee[road.top().first][road.top().second] = -2;
		Sleep(25);
		drawTable();
		road.pop();
	}
}


void drawHorizontalLine(int x, int y) {
	RectangleShape line(Vector2f(width, 5));

	line.setPosition(x, y);
	line.setFillColor(Color::Red);

	window.draw(line);
}

void drawVerticalLine(int x, int y) {
	RectangleShape line(Vector2f(5, height));

	line.setPosition(x, y);
	line.setFillColor(Color::Red);

	window.draw(line);
}


void drawSquare(int x, int y, Color color) {
	RectangleShape square(Vector2f(45, 45));
	square.setFillColor(color);
	square.setPosition(x, y);

	window.draw(square);
}


void drawTable() {
	window.clear();
	std::string s = text.getString();

	window.draw(text);

	for (int i = 0; i <= N; i++)
		drawVerticalLine(50 * i, 0);

	for (int i = 0; i <= M; i++)
		drawHorizontalLine(0, 50 * i);

	int i, j;
	pair<int, int> current;

	for (i = 0; i <= M; i++)
		for (j = 0; j <= N; j++) {
			current = make_pair(i, j);
			if (current == startPoint)
				drawSquare(j * 50 + 5, i * 50 + 5, Color::Green);

			else if (current == stopPoint)
				drawSquare(j * 50 + 5, i * 50 + 5, Color::Yellow);

			else if (Lee[i][j] == -1)
				drawSquare(j * 50 + 5, i * 50 + 5, Color::Magenta);

			else if (Lee[i][j] == -2)
				drawSquare(j * 50 + 5, i * 50 + 5, Color(0, 255, 234));

			else if (Lee[i][j])
				drawSquare(j * 50 + 5, i * 50 + 5, Color::Blue);
		}
	window.display();
}


pair<int, int> getPosition(int x, int y) {
	int row, column;
	row = y / 50;
	column = x / 50;

	return make_pair(row, column);
}


void setup() {
	window.setFramerateLimit(30);

	startPoint = stopPoint = make_pair(-1, -1);

	font.loadFromFile("arial.ttf");
	text.setFont(font);
	text.setFillColor(Color::White);
	text.setPosition(0, height + 5);
	text.setCharacterSize(30);
	text.setString("Select start point!");
}


int main() {
	setup();

	while (window.isOpen()) {
		Event event;

		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();

			else if (event.type == Event::MouseMoved);

			else if (event.type == Event::MouseButtonPressed && !hasFinishedAlgorithm) {
				if (event.mouseButton.button == Mouse::Left) {
					Vector2i position = Mouse::getPosition(window);

					if (position.x < width && position.y < height) {
						if (!hasSetedStartPoint) {
							startPoint = getPosition(position.x, position.y);
							hasSetedStartPoint = 1;

							text.setString("Select stop point!");

						}

						else if (!hasSetedStopPoint) {
							stopPoint = getPosition(position.x, position.y);
							hasSetedStopPoint = 1;

							text.setString("Select walls and press F1 to start!");
						}

						else {
							pair<int, int> point = getPosition(position.x, position.y);

							while (Mouse::isButtonPressed(Mouse::Button::Left) && position.x < width && position.y < height) {
								drawTable();
								Lee[point.first][point.second] = -1;
								position = Mouse::getPosition(window);
								point = getPosition(position.x, position.y);
							}
						}
					}
				}

				else if (event.mouseButton.button == Mouse::Right) {
					Vector2i position = Mouse::getPosition(window);
					pair<int, int> point = getPosition(position.x, position.y);
					Lee[point.first][point.second] = 0;
				}
			}

			else if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::F1) {
					text.setString("Computing...");
					window.clear();
					drawTable();

					LeeAlgorithm();
				}
			}

		}

		window.clear();
		drawTable();
	}

	return 0;
}
