#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "winsys.h"
#include "vector"
#include "cpoint.h"

enum dir {
	UP = 1,
	DOWN = 2,
	LEFT = 3,
	RIGHT = 4
};

class CSnake:public CFramedWindow
{
private:
	int level;
	CPoint head;
	vector<CPoint> body;
	int speed;
	CPoint food;
	bool isGameOver;
	bool isGameStarted;
	bool isGameStopped;
	bool isLost;
	bool isMenu;
	bool isPause;
	/*enum direction{
		UP = 1,
		DOWN = 2,
		RIGHT = 3,
		LEFT = 4
	};*/
	enum dir direction;
	int currMove;
	int counter;
public:
    CSnake(CRect r, char _c = ' ');
	void paint();
	bool handleEvent(int c);		
	CPoint placeFood();
	void move(const CPoint& delta);
	bool handleMove();
	void paintMenu(int start_x, int start_y);
	void paintBottomMenu(int start_x, int start_y);
	void drawSnake();
	void drawLogo(int start_x, int start_y);
	void resetGame();
};

#endif
