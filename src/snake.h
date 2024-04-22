#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "winsys.h"
#include <vector>

enum Direction {UP, DOWN, LEFT, RIGHT};

class CSnake:public CFramedWindow
{
public:
  CSnake(CRect r, char _c = ' ');
  int getEvent();

  bool handleEvent(int key);
  int level_count = 1;
  CPoint randomizeFood();
  CPoint food;
  int game_state = 2;
  void paint();

  void moveSnake();
  void snakeAction();
  int init = 0;

  void moveSnakeWithWindow(int c);
  void moveFoodWithWindow(int c);

  void printSnake();
  void printHead();
  void printBody();
  void printFood();
  void helpInfo();

  void assignBodyAndHead();
  void restartAction();
  void pauseAction();
  void helpAction();
  void foodHit();
  
  
    class Snake
    {
    public:
        CPoint head;
        std::vector<CPoint> body;
        CPoint createHead();
        std::vector<CPoint> createBody();
        int direction = RIGHT;

        void grow();
        bool changeDirection(int dir);
        void bodyMove();

        bool headCheck(int x,int y);
        bool bodyCheck(int x,int y);

        bool collisionCheck();

    };

    Snake snake;

};











#endif
