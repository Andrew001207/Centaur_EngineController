#ifndef DC_ENGINE_H
#define DC_ENGINE_H

#include <mbed.h>

class DC_engine
{
private:
    PwmOut *EN;
    DigitalOut *IN_1, *IN_2;
    bool reverse;
    float speed;
public:
    DC_engine(PinName _EN, PinName _IN_1, PinName _IN_2, bool _reverse);
    void write(float value);
    void operator= (float value)
    {
    this->write(value);
    }
    operator float()
    {
        return speed;
    }
    ~DC_engine();
};

DC_engine::DC_engine(PinName _EN, PinName _IN_1, PinName _IN_2, bool _reverse = false)
{
    EN = new PwmOut(_EN);
    IN_1 = new DigitalOut(_IN_1);
    IN_2 = new DigitalOut(_IN_2);
}

void DC_engine::write(float value)
{
    value = value > 1.0 ? 1 : value < -1.0 ? -1.0 : value;
    speed = value;
    if (value > 0)
    {
        IN_1->write(!reverse ? 1 : 0);
        IN_2->write(!reverse ? 0 : 1);
        EN->write(fabs(value));
    } else if (value < 0)
    {
        IN_1->write(!reverse ? 0 : 1);
        IN_2->write(!reverse ? 1 : 0);
        EN->write(fabs(value));
    } else
    {
        IN_1->write(0);
        IN_2->write(0);
        EN->write(0.0);
    }
    
}

DC_engine::~DC_engine()
{
    delete EN;
    delete IN_1,
    delete IN_2;
}


#endif