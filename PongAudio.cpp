#ifndef _PongAudio_CPP_
#define _PongAudio_CPP_

#include <Arduino.h>
#include "PongAudio.hpp"

PongAudio::PongAudio() {
  // NOP
}

void PongAudio::setup() {
  ledcSetup(0, 2000, 8);
  ledcAttachPin(12, channel);
}

void PongAudio::setVolume(int newVolume) {
  volume = newVolume;
}

void PongAudio::soundWall() {
  sound(226, 16, volume);
}
void PongAudio::soundPaddle() {
  sound(459, 96, volume);
}
void PongAudio::soundPoint() {
  sound(490, 257, volume);
}

void PongAudio::sound(int freq, long duration, int volume) {
  ledcWriteTone(channel, freq);
  ledcWrite(channel, volume);
  delay(duration);
  ledcWriteTone(channel, 0);
}

#endif

