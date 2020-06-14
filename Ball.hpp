#ifndef _Ball_HPP_
#define _Ball_HPP_


/**
   Ball.
*/
class Ball {
  public:
    Ball();

    void setRandomStartPosition();
    void updatePosition();
    void erase(ClockDisplay* clockDisplay);
    void draw(ClockDisplay* clockDisplay);

    bool checkCollisionWithWalls();
    //bool checkCollisionWithPaddle(Paddle *paddle);

    byte getEndPoint(int leftBorder, int rightBorder);
    float getX();
    float getY();
    int getDirection();
    bool isMissed();

    float getVelocityX();
    float getVelocityY();

    void bounceWall();
    void bouncePaddle();
    void flickUp();
    void flickDown();

  protected:
    float pos_x;
    float pos_y;
    float vel_x;
    float vel_y;

    byte erase_x = 10;
    byte erase_y = 10;

    bool missed = false;
};

Ball::Ball() {
}

bool Ball::isMissed() {
  return ((int)pos_x) == 0 || ((int) pos_x) == 47;
}

bool Ball::checkCollisionWithWalls() {
  // check ball collision with top of screen and reverse the y velocity if hit
  if (pos_y <= 0) {
    bounceWall();
    pos_y = 0; //make sure value goes no less that 0
    return true;
  }
  // check ball collision with bottom of screen and reverse the y velocity if hit
  if (pos_y >= 15) {
    bounceWall();
    pos_y = 15; //make sure value goes no more than 15
    return true;
  }

  return false;
}

//bool Ball::checkCollisionWithPaddle(Paddle *paddle) { }

void Ball::setRandomStartPosition() {
  pos_x = 23;
  pos_y = random(4, 12);

  //pick random ball direction
  if (random(0, 2) > 0) {
    vel_x = 1;
  } else {
    vel_x = -1;
  }
  if (random(0, 2) > 0) {
    vel_y = 0.5;
  } else {
    vel_y = -0.5;
  }
}

float Ball::getX() {
  return pos_x;
}
float Ball::getY() {
  return pos_y;
}

float Ball::getVelocityX() {
  return vel_x;
}
float Ball::getVelocityY() {
  return vel_y;
}

int Ball::getDirection() {
  if (vel_x < 0) {
    return -1;
  } else if (vel_x > 0) {
    return 1;
  }
  return 0;
}

void Ball::bounceWall() {
  vel_y *= -1;
}

void Ball::bouncePaddle() {
  vel_x *= -1;
}

void Ball::flickUp() {
  if (vel_y < 2) {
    vel_y += 0.2;
  }
}
void Ball::flickDown() {
  if (vel_y > 0.2) {
    vel_y -= 0.2;
  }
}

void Ball::updatePosition() {
  pos_x += vel_x;
  pos_y += vel_y;
}

void Ball::draw(ClockDisplay* clockDisplay) {
  byte plot_x = (int)(pos_x + 0.5f);
  byte plot_y = (int)(pos_y + 0.5f);

  // if the led at the ball pos is on already, dont bother printing the ball.
  if (clockDisplay->getPixel(plot_x, plot_y)) {
    // erase old point, but don't update the erase positions, so next loop the same point will be erased rather than this point which shouldn't be
    erase(clockDisplay);
  } else {
    // else plot the ball and erase the old position
    clockDisplay->plot(plot_x, plot_y, 1);
    erase(clockDisplay);
    // reset erase to new pos
    erase_x = plot_x;
    erase_y = plot_y;
  }
}

void Ball::erase(ClockDisplay* clockDisplay) {
  clockDisplay->plot(erase_x, erase_y, 0);
}

byte Ball::getEndPoint(int leftBorder, int rightBorder) {
  float temp_pos_x = pos_x;
  float temp_pos_y = pos_y;
  float temp_vel_x = vel_x;
  float temp_vel_y = vel_y;

  //run calculations until ball hits bat
  while (temp_pos_x > leftBorder && temp_pos_x < rightBorder) {
    temp_pos_x += temp_vel_x;
    temp_pos_y += temp_vel_y;

    // check for collisions with top / bottom
    if (temp_pos_y <= 0) {
      temp_vel_y *= -1;
      temp_pos_y = 0; //make sure value goes no less than 0
    }

    if (temp_pos_y >= 15) {
      temp_vel_y *= -1;
      temp_pos_y = 15; //make sure value goes no more than 15
    }
  }
  return temp_pos_y;
}

#endif
