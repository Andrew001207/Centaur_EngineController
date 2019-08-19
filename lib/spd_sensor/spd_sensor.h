#ifndef SPD_SENSOR_H
#define SPD_SENSOR_H

#include <mbed.h>

class Spd_sensor
{
private:
    InterruptIn *A;
    DigitalIn *B;
    double ratio;
    long value;
public:
    Spd_sensor(PinName _A, PinName _B, double _ratio);
    ~Spd_sensor();
    void ISR();
    double getValue();
};

Spd_sensor::Spd_sensor(PinName _A, PinName _B, double _ratio):ratio(_ratio)
{
    A = new InterruptIn(_A);
    A->rise(this, &Spd_sensor::ISR);
    B = new DigitalIn(_B);
}

void Spd_sensor::ISR()
{
    value += B->read() ? -1 : 1;
}

double Spd_sensor::getValue()
{
    double result = value * ratio;
    value = 0;
    return result;
}

Spd_sensor::~Spd_sensor()
{
    delete A;
    delete B;
}


#endif