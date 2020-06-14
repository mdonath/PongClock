#ifndef _ClockMode_HPP_
#define _ClockMode_HPP_

/**
   Abstract class for ClockModes.
*/
class ClockMode {
  public:
    ClockMode(const String& name);
    virtual void action() = 0;

    String getName();
    void setNext(ClockMode *nextMode);
    ClockMode* getNext();

  protected:
    String name;

  private:
    ClockMode* next;
};

#endif
