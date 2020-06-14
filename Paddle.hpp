#ifndef _Paddle_HPP_
#define _Paddle_HPP_

/**
   Paddle.
*/
class Paddle {
  public:
    Paddle(const char* name, int x);

    void init();
    void makeItMissTheBall();

    void setNewTarget(Ball *ball, long remoteBallX, int limit, int direction, int leftBorder, int rightBorder);
    void moveTowardsTarget();
    void draw(ClockDisplay *clockDisplay);
    bool checkCollisionWithBall(Ball* ball);

  private:
    const char* name;
    int x;
    int y;
    int target_y;
    bool miss = false;
    bool update = true;

};

Paddle::Paddle(const char* name, int x) :
  name(name),
  x(x)
{
  y = 5;
  target_y = y;
}

void Paddle::init() {
  miss = false;
}

void Paddle::makeItMissTheBall() {
  miss = true;
}

void Paddle::setNewTarget(Ball *ball, long remoteBallX, int limit, int direction, int leftBorder, int rightBorder) {
  // y range that bat can be in so it hits the ball - used
  // when we know we need to hit the ball, but want some randomness as to which part of the 6 pixel bat is hit
  byte bat_start_range;
  byte bat_end_range;

  // Make the paddle move when the ball is still on the other side, but not always, hence the randomly generated remoteBallX
  if (ball->getX() == remoteBallX) {
    target_y = ball->getY();
  }

  // when the ball is close to the bat and coming towards the paddle, run the ball maths to find out where the ball will land
  if (((int)ball->getX()) == limit && direction == ball->getDirection()) {

    byte end_ball_y = ball->getEndPoint(leftBorder, rightBorder);

    if (miss) {

      // if the miss flag is set, then the bat needs to miss the ball when it gets to end_ball_y

      miss = false;
      // move paddle out of the way
      if (end_ball_y > 8) {
        // set paddle to upper part of the screen
        target_y = random(0, 3);
      } else {
        // set paddle to lower part of the screen
        target_y = 8 + random(0, 3);
      }

    } else {

      // if the miss flag isn't set,  set bat target to ball end point with some randomness so its not always hitting top of bat

      if (end_ball_y <= 5) {
        bat_start_range = 0;
      } else {
        bat_start_range = end_ball_y - 5;
      }

      //bat end range. if ball <= 10 end range = ball end. else ball end = 10
      if (end_ball_y <= 10) {
        bat_end_range = end_ball_y;
      } else {
        bat_end_range = 10;
      }

      // calculate new target_y with some randomness and within range to hit the ball
      target_y = random(bat_start_range, bat_end_range + 1);
    }
  }
}

void Paddle::moveTowardsTarget() {
  // if bat y greater than target y move down until hit 0 (dont go any further or bat will move off screen)
  if (y > target_y && y > 0) {
    y--;
    update = true;
  }
  // if bat y less than target y move up until hit 10 (as bat is 6)
  if (y < target_y && y < 10) {
    y++;
    update = true;
  }
}

bool Paddle::checkCollisionWithBall(Ball *ball) {
  // and also check if bally lies within width of bat i.e. baty to baty + 6. We can use the exp if(a < b && b < c)
  if ((int)ball->getX() == x && (y <= (int)ball->getY() && (int)ball->getY() <= y + 5) ) {

    //random if bat flicks ball to return it - and therefor changes ball velocity
    if (!random(0, 3)) { //not true == no flick - just straight rebound and no change to ball y vel
      ball->bouncePaddle();
    } else {
      update = true;
      byte flick;  //0 = up, 1 = down.

      if (y > 1 || y < 8) {
        flick = random(0, 2);  //pick a random dir to flick - up or down
      }

      //if bat 1 or 2 away from top only flick down
      if (y <= 1) {
        flick = 0;   //move bat down 1 or 2 pixels
      }
      //if bat 1 or 2 away from bottom only flick up
      if (y >=  8) {
        flick = 1;  //move bat up 1 or 2 pixels
      }

      switch (flick) {
        //flick up
        case 0:
          target_y += random(1, 3);
          ball->bouncePaddle();
          ball->flickUp();
          break;

        //flick down
        case 1:
          target_y -= random(1, 3);
          ball->bouncePaddle();
          ball->flickDown();
          break;
      }
    }
    return true;
  }
  return false;
}

void Paddle::draw(ClockDisplay* clockDisplay) {
  if (update) {
    // closest to the inside of the field
    byte inner = x < 24 ? x - 1 : x + 1;
    // closest to the outer wall (left/right)
    byte outer = x < 24 ? x - 2 : x + 2;
    for (byte i = 0; i < 16; i++) {
      bool mode = i - y > -1 &&  i - y < 6;
      clockDisplay->plot(inner, i , mode);
      clockDisplay->plot(outer, i , mode);
    }
  }
}

#endif

