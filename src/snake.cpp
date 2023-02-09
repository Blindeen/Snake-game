#include <ncurses.h>

#include "snake.h"
#include "winsys.h"
#include "screen.h"

#define SNAKE_BODY '+'
#define SNAKE_HEAD '*'
#define FOOD 'O'
#define BLANK_FIELD ' '
#define BASE_TIME 1200

CSnake::CSnake(CRect r, char _c) : CFramedWindow(r, _c)
{
  srand(time(NULL));

  resetGame();
  pause = true;
}

void CSnake::paint()
{
  timeout(BASE_TIME/level);

  CFramedWindow::paint();
  printUI();
}

bool CSnake::handleEvent(int key)
{
  key = tolower(key);

  if((pause || help || died) && (key == KEY_UP || key == KEY_DOWN || key == KEY_LEFT || key == KEY_RIGHT))
  {
    return CFramedWindow::handleEvent(key);
  }

  if(!died && !pause && (key == KEY_UP || key == KEY_DOWN || key == KEY_LEFT || key == KEY_RIGHT))
  {
    if(key != direction)
    {
      if ((key == KEY_UP && direction != KEY_DOWN) || (key == KEY_DOWN && direction != KEY_UP) || (key == KEY_LEFT && direction != KEY_RIGHT) || (key == KEY_RIGHT && direction != KEY_LEFT))
      {
        direction = key;
        return true;
      }
    }

    return false;
  }

  if(!died && key == 'p')
  {
    if(pause)
    {
      pause = false;
      help = false;
    }
    else
    {
      pause = true;
    }

    return true;
  }

  if(pause && key == 'h')
  {
    help = true;

    return true;
  }

  if(key == 'r')
  {
    resetGame();

    return true;
  }

  return true;
}

void CSnake::printUI()
{
  if(!died)
  {
    gotoyx(geom.topleft.y - 1, geom.topleft.x);
    printl("| LEVEL: %d |", level);

    if(pause)
    {
      if (help)
      {
        CFramedWindow::paint();
        printHelp();
      }
      else
      {
        CFramedWindow::paint();
        printPause();
      }
    }
    else
    {
      moveSnake();
      printFood();
      printSnake();
    }
  }
  else
  {
    printGameOver();
  }
}

void CSnake::printPause()
{
  int x = geom.topleft.x, y = geom.topleft.y;

  gotoyx(y + 3, x + 3);
  printl("h - toggle help information");
  gotoyx(y + 4, x + 3);
  printl("p - toggle play/pause mode");
  gotoyx(y + 5, x + 3);
  printl("r - restart game");
  gotoyx(y + 6, x + 3);
  printl("q - quit");
  gotoyx(y + 7, x + 3);
  printl("arrows - move snake (in play mode) or");
  gotoyx(y + 8, x + 3);
  printl("move window (in pause mode)");
}

void CSnake::printGameOver()
{
  int x = geom.topleft.x, y = geom.topleft.y;

  gotoyx(y + 8, x + 12);
  printl("GAME OVER, result: %d", level);
}

void CSnake::printHelp()
{
  int x = geom.topleft.x, y = geom.topleft.y;

  gotoyx(y + 3, x + 3);
  printl("Use arrows to move snake");
  gotoyx(y + 4, x + 3);
  printl("Eat as much food as you can.");
  gotoyx(y + 5, x + 3);
  printl("Also, dont die.");
  gotoyx(y + 6, x + 3);
  printl("press 'p' or 'r' to play or 'h' for help");
}

void CSnake::printSnake()
{
  for(unsigned int i = 0; i < snake.size(); ++i)
  {
    gotoyx(snake[i].y + geom.topleft.y, snake[i].x + geom.topleft.x);
    if(!i)
    {
      printc(SNAKE_HEAD);
    }
    else
    {
      printc(SNAKE_BODY);
    }
  }
}

void CSnake::printFood()
{
  gotoyx(food.y + geom.topleft.y, food.x + geom.topleft.x);
  printc(FOOD);
}

void CSnake::spawnFood()
{
  bool correctCoordinates;
  int x, y;

  while(true)
  {
    correctCoordinates = true;

    x = rand() % (geom.size.x - 3) + 1;
    y = rand() % (geom.size.y - 3) + 1;

    for(unsigned int i = 0; i < snake.size(); ++i)
    {
      if(snake[i].x == x && snake[i].y == y)
      {
        correctCoordinates = false;
        break;
      }
    }

    if(correctCoordinates)
    {
      food = CPoint(x, y);
      return;
    }
  }
}

void CSnake::resetGame()
{
  int x = rand() % (geom.size.x - 5) + 1;
  int y = rand() % (geom.size.y - 3) + 1;

  snake.clear();
  snake.push_back(CPoint(x, y));
  snake.push_back(CPoint(x - 1, y));
  snake.push_back(CPoint(x - 2, y));

  spawnFood();

  pause = false;
  help = false;
  died = false;
  level = 1;
  direction = KEY_RIGHT;

  paint();
}

void CSnake::moveSnake()
{
  CPoint tail = snake[snake.size() - 1];

  for(unsigned int i = snake.size() - 1; i > 0; --i)
  {
    snake[i] = snake[i - 1];
  }

  switch(direction)
  {
  case KEY_RIGHT:
    snake[0] += CPoint(1, 0);
    break;
  case KEY_LEFT:
    snake[0] += CPoint(-1, 0);
    break;
  case KEY_UP:
    snake[0] += CPoint(0, -1);
    break;
  case KEY_DOWN:
    snake[0] += CPoint(0, 1);
    break;
  default:
    break;
  }

  for(unsigned int i = snake.size() - 1; i > 0; --i)
  {
    if(snake[0].x == snake[i].x && snake[0].y == snake[i].y)
    {
      died = true;
      return;
    }
  }

  if(snake[0].x == geom.size.x - 1)
  {
    snake[0].x = 1;
  }
  if(snake[0].x == 0)
  {
    snake[0].x = geom.size.x - 2;
  }
  if(snake[0].y == geom.size.y - 1)
  {
    snake[0].y = 1;
  }
  if(snake[0].y == 0)
  {
    snake[0].y = geom.size.y - 2;
  }

  if(isAte())
  {
    snake.push_back(tail);
    level++;
    spawnFood();
  }
}

bool CSnake::isAte()
{
  return (snake[0].x == food.x && snake[0].y == food.y);
}
