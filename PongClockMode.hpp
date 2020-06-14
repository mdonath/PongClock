#ifndef _PongClockMode_HPP_
#define _PongClockMode_HPP_

#include "AbstractClockMode.hpp"
#include "PongAudio.hpp"
#include "Ball.hpp"
#include "Paddle.hpp"

#define BAT1_X 2
#define BAT2_X 45



/**
   Play pong - using the score as the time.
*/
class PongClockMode : public AbstractClockMode {
  public:
    PongClockMode(const String& name, ClockDisplay* clockDisplay);
    void action();

  private:
    PongAudio* pongAudio;

    Ball* ball;
    Paddle* bat1;
    Paddle* bat2;

    void setup();
    void updateScore(bool ampm);
};

PongClockMode::PongClockMode(const String& name, ClockDisplay* clockDisplay):
  AbstractClockMode(name, clockDisplay)
{
  pongAudio = new PongAudio();
  pongAudio->setup();

  ball = new Ball();
  bat1 = new Paddle("left", BAT1_X);
  bat2 = new Paddle("right", BAT2_X);
}

/**
   Main loop for PongClockMode.
*/
void PongClockMode::action() {
  ENTER("PongClockMode");

  bool restart = true;

  boolean soundWall = false;
  boolean soundPaddle = false;
  boolean soundPoint = false;

  bool ampm = clockDisplay->getConfigurationAmPm();
  int volume = clockDisplay->getConfigurationVolume();
  pongAudio->setVolume(volume);

  //show "play pong" message and draw pitch line
  setup();

  // run clock main loop as long as keepModeRunning returns true
  while (clockDisplay->keepModeRunning()) {

    // if restart flag is true, setup a new game
    if (restart) {

      // check whether it's time to automatically display the date
      if (clockDisplay->check_show_date()) {
        //if date was displayed, we need to redraw the entire display, so run the setup routine
        setup();
      }

      updateScore(ampm);

      // erase ball pos
      ball->erase(clockDisplay);
      //set ball start pos
      ball->setRandomStartPosition();

      //draw bats in initial positions
      bat1->init();
      bat2->init();

      //reset game restart flag
      restart = false;

      //short wait before we launch the ball for effect
      delay(400);
    }

    byte secs = clockDisplay->getClock()->getSeconds();
    byte mins = clockDisplay->getClock()->getMinutes();

    //very basic AI...
    if (secs == 59 && mins < 59) {
      bat1->makeItMissTheBall();
    } else if (secs == 59 && mins == 59) {
      bat2->makeItMissTheBall();
    }

    bat1->setNewTarget(ball, random(30, 41), 23 - 8, -1, BAT1_X, BAT2_X);
    bat1->moveTowardsTarget();
    bat1->draw(clockDisplay);

    bat2->setNewTarget(ball, random(8, 19), 25 + 8, 1, BAT1_X, BAT2_X);
    bat2->moveTowardsTarget();
    bat2->draw(clockDisplay);

    // update the ball position using the velocity
    ball->updatePosition();

    // detect collision with walls
    soundWall = ball->checkCollisionWithWalls();

    // detect ball collision with bat1 & bat2
    soundPaddle = bat1->checkCollisionWithBall(ball);
    soundPaddle |= bat2->checkCollisionWithBall(ball);

    // plot the ball on the screen
    ball->draw(clockDisplay);

    // check if a bat missed the ball. if it did, reset the game.
    if (ball->isMissed()) {
      restart = true;
      soundPoint = true;
    }

    int corrected_delay = 10;

    if (soundPoint) {
      pongAudio->soundPoint();
      soundPoint = false;
    }
    if (soundWall) {
      pongAudio->soundWall();
      soundWall = false;
      corrected_delay = 4;
    }
    if (soundPaddle) {
      pongAudio->soundPaddle();
      soundPaddle = false;
      corrected_delay = 0;
    }
    if (corrected_delay > 0) {
      delay(corrected_delay);
    }

  }
  LEAVE("PongClockMode");
}


/**
   play pong message and draw centre line.
*/
void PongClockMode::setup() {
  clockDisplay->cls();

  byte i = 0;

  // Display "Ready? Play Pong!" Message
  const char intro1[6] = "Ready";
  while (intro1[i]) {
    delay(25);
    clockDisplay->putTinyChar((i * 4) + 12, 4, intro1[i]);
    i++;
  }

  // Flashing question mark
  for (byte i = 0; i < 2; i++) {
    clockDisplay->putTinyChar(34, 4, '?');
    delay(200);
    clockDisplay->putTinyChar(34, 4, ' ');
    delay(400);
  }

  clockDisplay->cls();
  i = 0;
  const char intro2[11] = "Play Pong!";
  while (intro2[i]) {
    clockDisplay->putTinyChar((i * 4) + 6, 4, intro2[i]);
    i++;
  }
  delay(800);

  clockDisplay->cls();

  // Draw pitch centre line
  // offset top centre line randomly, so different led's get used.
  byte offset = random(0, 2);
  for (byte i = 0; i < 16; i++) {
    if (i % 2 == 0) {
      clockDisplay->plot(24, i + offset, 1);
      delay(30);
    }
  }

  delay(120);
}


void PongClockMode::updateScore(boolean ampm) {
  char buffer[3];
  byte hours = clockDisplay->getClock()->getHours();
  formatHours(buffer, hours, ampm);
  clockDisplay->putTinyChar(14, 0, buffer[0]);
  clockDisplay->putTinyChar(18, 0, buffer[1]);

  byte mins = clockDisplay->getClock()->getMinutes();
  formatNumber(buffer, mins);
  clockDisplay->putTinyChar(28, 0, buffer[0]);
  clockDisplay->putTinyChar(32, 0, buffer[1]);
}


#endif
