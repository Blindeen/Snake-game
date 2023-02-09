#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <vector>

#include "winsys.h"

class CSnake:public CFramedWindow
{
public:
  CSnake(CRect r, char _c = ' ');

  void paint();
  bool handleEvent(int key);

private:
  vector <CPoint> snake;
  CPoint food;
  bool pause;
  bool help;
  bool died;
  int level;
  int direction;

  void printUI();
  void printPause();
  void printGameOver();
  void printHelp();
  void printSnake();
  void printFood();

  void spawnFood();
  void resetGame();
  void moveSnake();
  bool isAte();
};

#endif
