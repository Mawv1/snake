#include "snake.h"
#include "cpoint.h"
#include "screen.h"
#include <time.h>
#include <stdlib.h>

CSnake::CSnake(CRect r, char _c):
  CFramedWindow(r, _c)
{		
	level = 0;
	direction = RIGHT;
	head.x = geom.topleft.x + 3;
	head.y = geom.topleft.y + 1; 
	body.push_back(head);
	placeFood();
	isGameOver = false;
	isMenu = true;
	isGameStarted = false;
	isPause = false;
	currMove = 0;
	counter = 0;
	speed = 40;
}

void CSnake::paintMenu(int start_x, int start_y){
		drawSnake();
		gotoyx(start_y, start_x);
		printl("h - toggle help information");
		gotoyx(start_y + 1, start_x);
		printl("p - toggle pause/play mode");
		gotoyx(start_y + 2, start_x);
		printl("r - restart game");
		gotoyx(start_y + 3, start_x);
		printl("arrows - move snake (in play mode) or");
		gotoyx(start_y + 4, start_x + 9);
		printl("move window (in pause mode)");
}

void CSnake::paintBottomMenu(int start_x, int start_y){
	for (int y = start_y; y < start_y + 6; y++){
				if((y == start_y + 5)){
					for (int x = start_x; x < start_x + geom.size.x; x++){
						gotoyx(y,x);
						if (x == start_x || x == start_x + geom.size.x - 1)
							printl("+");
						else
							printl("-");
					}
				}
				else{
					for (int x = start_x; x <= start_x + geom.size.x - 1; x++){
						gotoyx(y, x);
						if(x == start_x || x == start_x + geom.size.x - 1)
							printl ("|");
						else if (x == start_x  + 3 && y == start_y){
							printl("h - toggle help information");
							x += 26;
						}
						else if (x == start_x  + 3 && y == start_y + 1){
							printl("p - toggle pause/play mode");
							x += 25;
						}
						else if (x == start_x  + 3 && y == start_y + 2){
							printl("r - restart game");
							x += 15;
						}	
						else if (x == start_x  + 3 && y == start_y + 3){
							printl("arrows - move snake (in play mode) or");
							x += 36;
						}
						else if (x == start_x  + 3 && y == start_y + 4){
							printl("move window (in pause mode)");
							x += 26;
						}
						else{
							printl("%c",c);
						}
					}
				}
			}
}

void CSnake::drawLogo(int start_x, int start_y){
	gotoyx(start_y, start_x);
	printl("##    #  #    ##    #  #    ###");
	start_y += 1;
	start_x -= 1;
	gotoyx(start_y, start_x);
	printl("#  #   ## #   #  #   # ##   ##");
	start_y += 1;
	gotoyx(start_y, start_x);
	printl("#      ####   #  #   ###    #");
	start_y += 1;
	gotoyx(start_y, start_x);
	printl(" #     # ##   ####   ##     ####");
	start_y +=1;
	gotoyx(start_y, start_x);
	printl("  #    #  #   #  #   ###    #");
	start_y += 1;
	gotoyx(start_y, start_x);
	printl("#  #   #  #   #  #   # ##   ##");
	start_y += 1;
	gotoyx(start_y, start_x);
	printl(" ##    #  #   #  #   #  #    ###");
}

void CSnake::paint(){
	CFramedWindow::paint();
	gotoyx(geom.topleft.y - 1, geom.topleft.x);
	printl("| LEVEL : %d |", level);
	
	if (isGameStarted == true){
		if(isMenu){
			int start_x = geom.topleft.x;
			int start_y = geom.topleft.y + geom.size.y;
			paintBottomMenu(start_x, start_y);
		}
		if (isPause){
			int start_x = geom.topleft.x + 3;
			int start_y = geom.topleft.y + 3;
			start_y += 3;
			start_x += 4;
			drawLogo(start_x, start_y);			
		}
		drawSnake();
		gotoyx(food.y,food.x);
		printl("O");
	}
	
	else{
		if (isMenu){
			drawSnake();
			int start_x = geom.topleft.x + 3;
			int start_y = geom.topleft.y + 3;
			paintMenu(start_x, start_y);
		}
		
	}
	if (isGameOver){
		int start_x = geom.topleft.x + 3;
		int start_y = geom.topleft.y + 3;
		paintMenu(start_x, start_y);
		start_y += 7;
		gotoyx(start_y, start_x);
		printl("GAME OVER ! YOUR LEVEL : %d", level);
	}
}

bool CSnake::handleEvent(int c){
	if (!isGameStarted || isPause || isGameOver){
		if (CWindow::handleEvent(c)) return true;
		
		else {
			if (c == 'p'){
				if (isGameOver) level = 0;
				isGameStarted = true;
				isPause = false;
				isGameOver = false;
				isMenu = true;
				return true;
			}
			else if (c == 'h'){
				if (!isMenu) isMenu = true;
				else isMenu = false;
				return true;
			}
			else if (c == 'r'){
				resetGame();
				return true;
			}
		}
		return false;
	}
	
	else if (isGameStarted){
		
		if (c == 'p'){
				if (!isPause) isPause = true;
				else isPause = false;
				return true;
			}
		else if (c == 'h'){
			if (!isMenu) isMenu = true;
			else isMenu = false;
			return true;
		}
		else if (c == 'r'){
			resetGame();
			return true;
		}
		else if (c == KEY_UP){
			if (direction != DOWN) {
				direction = UP;
			}
		}
		else if (c == KEY_DOWN) {
			if (direction != UP){
				direction = DOWN;
			}
		}
		else if (c == KEY_LEFT){
			if (direction != RIGHT){
				direction = LEFT;
			}
		}
		else if (c == KEY_RIGHT){
			if (direction != LEFT){
				direction = RIGHT;
			}
		}
		else if (c == ERR){
			counter++;
			if (!handleMove()) return false;
		}
	}
	
	switch (direction) {
        case UP:
            head.y--;
            break;
        case DOWN:
            head.y++;
            break;
        case RIGHT:
            head.x++;
            break;
        case LEFT:
            head.x--;
            break;
    }
	
	for (auto it = body.begin(); it != body.end(); it++) {
        if (it->x == head.x && it->y == head.y && it != body.begin()) {
			resetGame();
            return true;
        }
    }
    
    if (head.x == geom.topleft.x + geom.size.x - 1) {
        head.x = geom.topleft.x + 1;
    }
    if (head.x == geom.topleft.x) {
        head.x = geom.topleft.x + geom.size.x - 2;
    }
    if (head.y == geom.topleft.y + geom.size.y - 1) {
        head.y = geom.topleft.y + 1;
    }
    if (head.y == geom.topleft.y) {
        head.y = geom.topleft.y + geom.size.y - 2;
    }
	
	body.push_back(head);
	if (head.x == food.x && head.y == food.y) {
        placeFood();
        level++;
    } else {
        body.erase(body.begin());
    }
	
	return true;
}		

void CSnake::move(const CPoint& delta){
	geom.topleft += delta;
	for (auto &it: body){
		it += delta;
	}
	food += delta;
	head += delta;
}

CPoint CSnake::placeFood(){
	srand(time(NULL));
	while(1){
		food.x = (rand() % (geom.size.x - 2) + geom.topleft.x + 1);
		food.y = (rand() % (geom.size.y - 2) + geom.topleft.y + 1);
		bool flagOnSnake = false;
		for (auto &it: body) {
			if (it.x == food.x && it.y == food.y) {
				flagOnSnake = true;
				break;
			}
		}
		if (!flagOnSnake) break;
	}
	return food;
}

void CSnake::drawSnake(){
	for (auto &it: body){
		gotoyx(it.y, it.x);
		if (it.x == head.x && it.y == head.y){
			printl("*");
		}
		else{
			printl("+");
		}
	}
}

bool CSnake::handleMove(){
	if(counter > speed / (level + 1) ){
		counter = 0;
		return true;
	}
	return false;
}

void CSnake::resetGame(){
	body.clear();
	direction = RIGHT;
	head.x = geom.topleft.x + 3;
	head.y = geom.topleft.y + 1; 
	body.push_back(head);
	placeFood();
	isGameOver = true;
	isMenu = true;
	isGameStarted = false;
	isPause = false;
	currMove = 0;
	counter = 0;
	speed = 40;
}
