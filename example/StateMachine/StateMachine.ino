#include <Arduino.h>
#include "ButtonState.hpp"
#include "EmmaButton.hpp"
#include "SAM2695Synth.h"
#include "State.hpp"

SAM2695Synth synth = SAM2695Synth::getInstance();
EmmaButton button;
StateMachine stateMachine;

bool isPressed = false;

void setup() {
    //init serial
    Serial.begin(115200);
    delay(3000);
    //init synth
    synth.begin();
    //get state manager
    StateManager* manager = StateManager::getInstance();
    //regist button state
    manager->registerState(new ButtonState1());
    manager->registerState(new ButtonState2());
    manager->registerState(new ButtonState3());
    //regist error state
    ErrorState* errorState = new ErrorState();
    manager->registerState(errorState);
    //init state machine
    if(!(stateMachine.init(manager->getState(ButtonState1::ID), errorState)))
    {
        StateManager::releaseInstance();
        return ;
    }
    Serial.println("stateMachine ready!");
}

void loop()
{
    Event* event = getNextEvent();
    if(event != nullptr)
    {
        stateMachine.handleEvent(event);
        delete event;
    }
}

Event* getNextEvent()
{
    //Used to control long press and short press events. 
    //A short press is triggered when released, otherwise
    //it is considered a long press.
    if(button.A.pressed()==BtnAct::Pressed
        || button.B.pressed()==BtnAct::Pressed
        || button.C.pressed()==BtnAct::Pressed
        || button.D.pressed()==BtnAct::Pressed)
    {
        isPressed = true;
    }

    if(button.A.longPressed()==BtnAct::LongPressed)
    {
        isPressed = false;
        Event* e = new Event(EventType::BtnALongPressed);
        return e;
    }

    if(button.B.longPressed()==BtnAct::LongPressed)
    {
        isPressed = false;
        Event* e = new Event(EventType::BtnBLongPressed);
        return e;
    }

    if(button.C.longPressed()==BtnAct::LongPressed)
    {
        isPressed = false;
        Event* e = new Event(EventType::BtnCLongPressed);
        return e;
    }

    if(button.D.longPressed()==BtnAct::LongPressed)
    {
        isPressed = false;
        Event* e = new Event(EventType::BtnDLongPressed);
        return e;
    }

    if(button.A.released()==BtnAct::Released)
    {
        if(isPressed)
        {
            isPressed = false;
            Event* e = new Event(EventType::BtnAPressed);
            return e;
        }
    }
    if(button.B.released()==BtnAct::Released)
    {
        if(isPressed)
        {
            isPressed = false;
            Event* e = new Event(EventType::BtnBPressed);
            return e;
        }
    }
    if(button.C.released()==BtnAct::Released)
    {
        if(isPressed)
        {
            isPressed = false;
            Event* e = new Event(EventType::BtnCPressed);
            return e;
        }
    }
    if(button.D.released()==BtnAct::Released)
    {
        if(isPressed)
        {
            isPressed = false;
            Event* e = new Event(EventType::BtnDPressed);
            return e;
        }
    }

    return nullptr;
}


