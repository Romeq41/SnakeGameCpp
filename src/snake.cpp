#include "snake.h"
#include "screen.h"
#include <time.h>
#include <vector>
#include <unistd.h>

CSnake::CSnake(CRect r, char _c /*=' '*/): CFramedWindow(r, _c){}

//state 2 = help
//state 1 = play
//state 0 = pause
//state -1 = game over
void CSnake::paint() {
    CFramedWindow::paint();

    if(game_state == 1){
        if (level_count > 1 && init == 0) {
            food = randomizeFood();
            init = 1;
        }
    }

    gotoyx(geom.topleft.y - 2, geom.topleft.x + 1);
    printl(" Level: %d ", level_count);

    //Help print and initializing first snake and food position
    if(game_state == 2){
        helpInfo();
        if (level_count == 1 && init == 0) {
            snake.head = CPoint(geom.topleft.x+2, geom.topleft.y+2);
            food.y = geom.topleft.y + 10;
            food.x = geom.topleft.x + 10;
            init = 1;
        }
    }
    
    printFood();
    printSnake();

    if(game_state == -1){
        gotoyx(geom.topleft.y + 4, geom.topleft.x + 20);
        printl("Game ");
        gotoyx(geom.topleft.y + 5, geom.topleft.x + 20);
        printl("Over");
        gotoyx(geom.topleft.y + 6, geom.topleft.x + 13);
        printl("Press R to restart");
    }
    refresh();
}

void CSnake::helpInfo(){
	gotoyx(geom.topleft.y + 4, geom.topleft.x + 1);
	printl("    h - toggle help information");
	gotoyx(geom.topleft.y + 5, geom.topleft.x + 1);
	printl("    p - toggle pause/play mode");
	gotoyx(geom.topleft.y + 6, geom.topleft.x + 1);
	printl("    r - restart game");
	gotoyx(geom.topleft.y + 7, geom.topleft.x + 1);
	printl("    arrows - move snake (in play mode) or");
	gotoyx(geom.topleft.y + 8, geom.topleft.x + 1);
	printl("             move window (in pause mode)");
}


void CSnake::restartAction(){
	snake.head.y = geom.topleft.y + 2;
	snake.head.x = geom.topleft.x + 2;
	game_state = 1;
	level_count = 1;
	snake.direction = RIGHT;
	init = 0;
	snake.body.clear();
	paint();
	refresh();
}

void CSnake::helpAction(){
	if(game_state != 2){
		game_state = 2;
	}else if(game_state != -1){
		game_state = 0;
	}
}

void CSnake::pauseAction(){
	if(game_state == 0 || game_state == 2 ){
		game_state = 1;
		paint();
	}else if(game_state != -1){
		game_state = 0;
		paint();
	}
}

void CSnake::foodHit(){
	snake.body.push_back(food);
	level_count++;
	food = randomizeFood();
	init = 0;	
}

void delaySnake(int level)
{
    if(level > 0 && level < 10){
        usleep(50000);
    }else if(level > 9 && level< 20){
        usleep(30000);
    }else if(level > 19 && level < 30){
        usleep(10000);
    }
}
//Keyboard Event Handling
bool CSnake::handleEvent(int c)
{
    //help
    if(c == 'h' || c== 'H') {
        helpAction();
        return true;
    }

    //restart
    if(c == 'r' || c == 'R'){
        restartAction();
        return true;
    }

    //pause
    if(c == 'p' || c == 'P'){
		pauseAction();
        return true;
    }

    //Snake play
    if(game_state == 1){
        delaySnake(level_count);

        if(c == KEY_UP){
            snake.changeDirection(UP);
        }else if(c == KEY_DOWN){
            snake.changeDirection(DOWN);
        }else if(c == KEY_LEFT){
            snake.changeDirection(LEFT);
        }else if(c == KEY_RIGHT){
            snake.changeDirection(RIGHT);
        }
        
        moveSnake();
        
        if(snake.head.x == food.x && snake.head.y == food.y){
            foodHit();
        }
        
        printSnake();
        refresh();
        return true;
    }

    //if window moved change snake placement
    if(CFramedWindow::handleEvent(c) && game_state != 1){
        if(c == KEY_UP){
            moveSnakeWithWindow(UP);
            moveFoodWithWindow(UP);
        }else if(c == KEY_DOWN){
            moveSnakeWithWindow(DOWN);
            moveFoodWithWindow(DOWN);
        }else if(c == KEY_LEFT){
            moveSnakeWithWindow(LEFT);
            moveFoodWithWindow(LEFT);
        }else if(c == KEY_RIGHT){
            moveSnakeWithWindow(RIGHT);
            moveFoodWithWindow(RIGHT);
        }
        paint();
        refresh();
        return true;
    }

    return false;
}

bool CSnake::Snake::collisionCheck() {
    for (int i = 1; i < (int)body.size(); ++i) {
        if(head.x == body[i].x && head.y == body[i].y){
            return true;
        }
    }
    return false;
}

//MOVEMENT DONE
void CSnake::assignBodyAndHead(){
    if((int)snake.body.size() > 0){

        for (int i = (int)snake.body.size() - 1; i > 0; --i) {
            snake.body[i].x = snake.body[i - 1].x;
            snake.body[i].y = snake.body[i - 1].y;
        }

        snake.body[0].x = snake.head.x;
        snake.body[0].y = snake.head.y;
    }
}

void CSnake::moveSnake(){
    switch (snake.direction) {
        case UP:
            assignBodyAndHead();
            snake.head.y--;

            if(snake.collisionCheck()){
                game_state = -1;
            }

            if(snake.head.y == geom.topleft.y){
                snake.head.y = geom.topleft.y + geom.size.y - 2;
            }
            break;
        case DOWN:
            assignBodyAndHead();
            snake.head.y++;
            if(snake.collisionCheck()){
                game_state = -1;
            }

            if(snake.head.y == geom.topleft.y + geom.size.y-1){
                snake.head.y = geom.topleft.y + 1;
            }
            break;
        case LEFT:
            assignBodyAndHead();
            snake.head.x--;
            if(snake.collisionCheck()){
                game_state = -1;
            }

            if(snake.head.x == geom.topleft.x){
                snake.head.x = geom.topleft.x + geom.size.x - 2;
            }
            break;
        case RIGHT:

            assignBodyAndHead();
            snake.head.x++;
            if(snake.collisionCheck()){
                game_state = -1;
            }
            if(snake.head.x == geom.topleft.x + geom.size.x-1){
                snake.head.x = geom.topleft.x + 1;
            }
            break;
    }
}

bool CSnake::Snake::changeDirection(int dir) {
    if (dir == UP && direction != DOWN) {
        direction = UP;
        return true;
    } else if (dir == DOWN && direction != UP) {
        direction = DOWN;
        return true;
    } else if (dir == LEFT && direction != RIGHT) {
        direction = LEFT;
        return true;
    } else if (dir == RIGHT && direction != LEFT) {
        direction = RIGHT;
        return true;
    }

    return false;
}
void CSnake::moveSnakeWithWindow(int c){

    switch (c){
        case UP:
            snake.head.y -= 1;
            for (int i = 0; i < (int)snake.body.size(); ++i) {
                snake.body[i].y -= 1;
            }
            break;
        case DOWN:
            snake.head.y += 1;
            for (int i = 0; i < (int)snake.body.size(); ++i) {
                snake.body[i].y += 1;
            }
            break;
        case RIGHT:
            snake.head.x += 1;
            for (int i = 0; i < (int)snake.body.size(); ++i) {
                snake.body[i].x += 1;
            }
            break;
        case LEFT:
            snake.head.x -= 1;
            for (int i = 0; i < (int)snake.body.size(); ++i) {
                snake.body[i].x -= 1;
            }
            break;
    }
}
void CSnake::moveFoodWithWindow(int c){
    switch (c){
        case UP:
            food.y -= 1;
            break;
        case DOWN:
            food.y += 1;
            break;
        case RIGHT:
            food.x += 1;
            break;
        case LEFT:
            food.x -= 1;
            break;
    }
}

//FOOD DONE
bool CSnake::Snake::headCheck(int x,int y){
    if(x == head.x && y == head.y){
        return false;
    }
    return true;
}
bool CSnake::Snake::bodyCheck(int x,int y){
    for (int i = 0; i < (int)body.size(); ++i) {
        if(body[i].x == x && body[i].y == y){
            return false;
        }
    }
    return true;
}
CPoint CSnake::randomizeFood()
{
    srand(time(NULL));
    int x = rand() % (geom.size.x-5) + geom.topleft.x+1;
    int y = rand() % (geom.size.y-5) + geom.topleft.y+1;

    while(!snake.headCheck(x,y) && !snake.bodyCheck(x,y)){
        x = rand() % (geom.size.x-5) + geom.topleft.x+1;
        y = rand() % (geom.size.y-5) + geom.topleft.y+1;
    }

    CPoint food = CPoint(x, y);
    return food;
}

//PRINTS
void CSnake::printSnake() {
    printHead();
    printBody();
}

void CSnake::printHead() {
    gotoyx(snake.head.y, snake.head.x);
    printc('*');
}

void CSnake::printBody() {
    for (int i = 0; i < (int)snake.body.size(); ++i) {
        gotoyx(snake.body[i].y,snake.body[i].x);
        printc('*');
    }
}

void CSnake::printFood() {
    gotoyx(food.y ,food.x);
    printl("O");
}

int CSnake::getEvent()
{
    return ngetch();
}



