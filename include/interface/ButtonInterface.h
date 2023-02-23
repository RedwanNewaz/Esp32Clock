#pragma once 

#include <Arduino.h>

#define RESET_PIN   (D3)
#define MINIUTE_PIN (D1)
#define HOUR_PIN    (D2)
#define POWER_PIN   (D0)

#define LONG_DEBOUNCE (2000)
#define SHORT_DEBOUNCE (75)

class Button{
public:
    Button(const String name, int pin, int cycle, int delay, bool reversed):
    pin_(pin), cycle_(cycle), delay_(delay), name_(name), reversed_(reversed)
    {
        // the button is reversed when it goes from high state to low state, e.g., power button 

    }
    void execute()
    {
        int state = 1 - reversed_;
        for(int i = 0; i < cycle_; ++i)
        {
        // forward 
        digitalWrite(pin_, state);
        delay(delay_);
        // reverse 
        digitalWrite(pin_, 1 - state);
        delay(delay_);
        }
    }
private:
    int pin_, cycle_, delay_;
    bool reversed_;
    String name_;
};

class ButtonInterface{
public:
    ButtonInterface():
    power_pin_(POWER_PIN), reset_pin_(RESET_PIN), hour_pin_(HOUR_PIN), minute_pin_(MINIUTE_PIN)
    {
        this->reset_time_ = LONG_DEBOUNCE;
        this->debounce_time_ = SHORT_DEBOUNCE; 

        pinMode(hour_pin_, OUTPUT);
        pinMode(minute_pin_, OUTPUT);
        pinMode(reset_pin_, OUTPUT);
        pinMode(power_pin_, OUTPUT);

        // this logic is based on NPN transistors 
        digitalWrite(hour_pin_, LOW);
        digitalWrite(minute_pin_, LOW);
        digitalWrite(reset_pin_, LOW);
        digitalWrite(power_pin_, HIGH);
    }

    void setTime(int num_hour, int num_minute)
    {
        // cycle power 
        Button power("power", power_pin_, 1, reset_time_, true);
        Button memory("memory", reset_pin_, 1, reset_time_, false);
        Button hour("hour", hour_pin_, num_hour, debounce_time_, false);
        Button minute("minute", minute_pin_, num_minute, debounce_time_, false);
        Button finish("finish", reset_pin_, 1, debounce_time_, false);
        
        // execute buttons
        Serial.println("[ButtonClock]: setting up clock time by simulating buttons"); 
        power.execute();
        memory.execute();
        hour.execute();
        minute.execute();
        finish.execute();

        
    }

private:
    int power_pin_, reset_pin_, hour_pin_, minute_pin_;
    int reset_time_, debounce_time_;


};