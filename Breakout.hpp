#ifndef _BreakoutClockMode_HPP_
#define _BreakoutClockMode_HPP_

#include "AbstractClockMode.hpp"

class PositionalElement {
  public:
    PositionalElement(byte x, byte y);
    virtual void draw(ClockDisplay* clockDisplay) = 0;
  protected:
    byte x;
    byte y;
};

PositionalElement::PositionalElement(byte x, byte y) : x(x), y(y) {

}

class BreakoutPaddle : public PositionalElement {
  public:
    BreakoutPaddle(byte x, byte y, byte width);
    void draw(ClockDisplay* clockDisplay);
  private:
    byte width;
};


BreakoutPaddle::BreakoutPaddle(byte x, byte y, byte width): PositionalElement(x, y), width(width) { }


void BreakoutPaddle::draw(ClockDisplay* clockDisplay) {
  for (int i = 0; i < width; i++) {
    clockDisplay->setPixel(x+i - (width/2), y, 1);
  }
  clockDisplay->display();
}

class BreakoutBall : public Ball {
  public:
    BreakoutBall(byte x, byte y);
    bool checkCollisionWithWalls();
    bool checkCollisionWithPaddle(BreakoutPaddle* paddle);
  private:
};

BreakoutBall::BreakoutBall(byte x, byte y): Ball() {
  pos_x = x;
  pos_y = y;
}

bool BreakoutBall::checkCollisionWithPaddle(BreakoutPaddle* paddle) {
  return false;
}

bool BreakoutBall::checkCollisionWithWalls() {
  //check ball collision with top of screen and reverse the y velocity if hit
  if (pos_y <= 0) {
    vel_y *= -1;
    pos_y = 0; //make sure value goes no less that 0
    return true;
  }
  //check ball collision with bottom of screen and reverse the y velocity if hit
  if (pos_y >= 15) {
    vel_y *= -1;
    pos_y = 15; //make sure value goes no more than 15
    return true;
  }
  if (pos_x <= 0) {
    vel_x *= -1;
    pos_x = 0;
    return true;
  }
  if (pos_x >= 47) {
    vel_x *= -1;
    pos_x = 47;
    return true;
  }

  return false;
}


class Brick : public PositionalElement {
  public:
    Brick(byte x, byte y);
    void draw(ClockDisplay* clockDisplay);
    void setHit(bool hit);
  private:
    bool is_hit = false;
};

Brick::Brick(byte x, byte y): PositionalElement(x, y) { }

void Brick::setHit(bool hit) {
  is_hit = hit;
}

void Brick::draw(ClockDisplay* clockDisplay) {
  for (byte i = 0; i < 3; i++) {
    clockDisplay->setPixel(x + i, y    , !is_hit);
    clockDisplay->setPixel(x + i, y + 1, !is_hit);
  }
  clockDisplay->display();
}

/**
   Shows the game Breakout.
*/
class BreakoutClockMode : public AbstractClockMode {
  public:
    BreakoutClockMode(const String& name, ClockDisplay* clockDisplay);
    void action();

  private:
    Brick* bricks[12][5];
    BreakoutPaddle* paddle;
    BreakoutBall* ball;

    void setup();
    void redraw();
    void restoreHit();
};

BreakoutClockMode::BreakoutClockMode(const String& name, ClockDisplay* clockDisplay):
  AbstractClockMode(name, clockDisplay)
{
  // bricks
  for (byte row = 0; row < 5; row++) {
    for (byte i = 0; i < 12; i++) {
      bricks[i][row] = new Brick((i * 4) + row % 2, row * 2);
    }
  }
  // paddle
  paddle = new BreakoutPaddle(10, 15, 6);
  // ball
  ball = new BreakoutBall(23, 12);
  ball->setRandomStartPosition();
}


void BreakoutClockMode::action() {
  ENTER("BreakoutClockMode");
  bool ampm = clockDisplay->getConfigurationAmPm();

  setup();

  byte line = 0;
  byte secs = 100;

  // run clock main loop as long as keepModeRunning returns true
  while (clockDisplay->keepModeRunning()) {
    byte newSecs = clockDisplay->getClock()->getSeconds();
    if (newSecs == 0) {
      restoreHit();
    }
    if (secs != newSecs) {
      secs = newSecs;
      byte index = 60 - secs;
      bricks[index % 12][index / 12]->setHit(true);

      redraw();
    }
    ball->updatePosition();
    ball->checkCollisionWithWalls();
    ball->checkCollisionWithPaddle(paddle);
    ball->draw(clockDisplay);

    delay(100);
  }
  for (int row = 0; row < 5; row++) {
    delete[] bricks[row];
  }
  delete[] bricks;
  LEAVE("DigitsClockMode");
}

void BreakoutClockMode::restoreHit() {
  for (byte row = 0; row < 5; row++) {
    for (byte i = 0; i < 12; i++) {
      bricks[i][row]->setHit(false);
    }
  }
}


void BreakoutClockMode::redraw() {
  clockDisplay->cls();
  //  for (byte row = 0; row < 5; row++) {
  //    for (byte i = 0; i < 12; i++) {
  //      bricks[i][row]->draw(clockDisplay);
  //    }
  //  }
  paddle->draw(clockDisplay);
  ball->draw(clockDisplay);
}

void BreakoutClockMode::setup() {
  redraw();
  delay(1000);
}

#endif
