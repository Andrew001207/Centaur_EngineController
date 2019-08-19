#ifndef PWR_CTRL_H
#define PWR_CTRL_H

#include <mbed.h>

class DCV_meas
{
private:
    AnalogIn *pin;
    float ratio;
    float value;
public:
    DCV_meas(PinName _pin, float _ratio);
    float measure();
    operator float()
    {
        return measure();
    }
    ~DCV_meas();
};

DCV_meas::DCV_meas(PinName _pin, float _ratio = 1.0)
{
    this->ratio = _ratio;
    pin = new AnalogIn(_pin);
}

float DCV_meas::measure()
{
    value = pin->read() * 3.3f * ratio;
    return value;
}

DCV_meas::~DCV_meas()
{
    delete pin;
}


#endif