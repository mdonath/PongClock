#ifndef _PongAudio_HPP_
#define _PongAudio_HPP_

class PongAudio {
  public:
    PongAudio();
    void setup();

    void setVolume(int volume);
    int getVolume();

    void soundWall();
    void soundPaddle();
    void soundPoint();
  private:
    int channel = 0;
    int volume;
    void sound(int freq, long duration, int volume);
};

#endif
