#pragma once
class Point {
	int x, y;
	bool right, left, up, down;
public:
	Point(int X = 0, int Y = 0) {
		x = X; y = Y;
		right = false; left = false; up = false; down = false;
	}

	void setPosition(int X, int Y) { x = X; y = Y; }

	void rightOn() { right = true; }
	void leftOn() { left = true; }
	void upOn() { up = true; }
	void downOn() { down = true; }

	void rightOff() { right = false; }
	void leftOff() { left = false; }
	void upOff() { up = false; }
	void downOff() { down = false; }

	int getX() { return x; }
	int getY() { return y; }

	bool getRight() { return right; }
	bool getLeft() { return left; }
	bool getUp() { return up; }
	bool getDown() { return down; }
};
