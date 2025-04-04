#include <Arduino.h>
#include "ButtonState.hpp"
#include "EmmaButton.hpp"
#include "SAM2695Synth.h"
#include "State.hpp"

#define STATE_1_LED_TIME 800
#define STATE_2_LED_TIME 400
#define STATE_3_LED_TIME 200

SAM2695Synth synth = SAM2695Synth::getInstance();
EmmaButton button;
StateMachine stateMachine;

bool isPressed = false;
unsigned long previousMillis = 0;   // Record the time of the last sent MIDI signal.
int noteType = QUATER_NOTE;         // Note type selection: 0 (quarter note), 1 (eighth note), 2 (sixteenth note)
uint8_t drupCount = 0;              // Count the beats and play different notes
uint8_t voice = 50;
uint8_t  modeID = 0;                // current mode id
int ledTime = 0;                    // LED Interval time
unsigned long previousMillis2 = 0;  // Record the time of the last LED change


void setup() {
    //init serial
    Serial.begin(115200);
    //init led
    pinMode(LED_BUILTIN, OUTPUT);
    delay(3000);
    //init synth
    synth.begin();
    //get state manager
    StateManager* manager = StateManager::getInstance();
    //regist button state
    manager->registerState(new State1());
    manager->registerState(new State2());
    manager->registerState(new State3());
    //regist error state
    ErrorState* errorState = new ErrorState();
    manager->registerState(errorState);
    //init state machine
    if(!(stateMachine.init(manager->getState(State1::ID), errorState)))
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
    multiTrackPlay();
    ledShowByState();
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

void multiTrackPlay()
{
    unsigned long interval = (BASIC_TIME / synth.getBpm()) / (noteType + 1);
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;
        if(channel_1_on_off_flag)
        {
            uint8_t pitch = synth.getPitch();
            synth.setNoteOn(CHANNEL_0,synth.getPitch(),voice);
        }
        if(channel_2_on_off_flag)
        {
            synth.setNoteOn(CHANNEL_1,synth.getPitch(),voice);
        }
        if(channel_3_on_off_flag)
        {
            synth.setNoteOn(CHANNEL_2,synth.getPitch(),voice);
        }
        if(channel_4_on_off_flag)
        {
            synth.setNoteOn(CHANNEL_3,synth.getPitch(),voice);
        }
        if(drum_on_off_flag)
        {
            if(drupCount % 4 == 0)
            {
                synth.setNoteOn(CHANNEL_9,NOTE_C2,voice);
                synth.setNoteOn(CHANNEL_9,NOTE_FS2,voice);
            }
            else if(drupCount % 4 == 1)
            {
                synth.setNoteOn(CHANNEL_9,NOTE_FS2,voice);
            }
            else if(drupCount % 4 == 2)
            {
                synth.setNoteOn(CHANNEL_9,NOTE_D2,voice);
                synth.setNoteOn(CHANNEL_9,NOTE_FS2,voice);
            }
            else if(drupCount % 4 == 3)
            {
                synth.setNoteOn(CHANNEL_9,NOTE_FS2,voice);
            }
            drupCount++;
        }
    }
}

void ledShowByState()
{
    modeID = stateMachine.getCurrentState()->getID();
    if(modeID == State1::ID)
    {
        ledTime = STATE_1_LED_TIME;
    }
    else if(modeID == State2::ID)
    {
        ledTime = STATE_2_LED_TIME;
    }
    else if(modeID == State3::ID)
    {
        ledTime = STATE_3_LED_TIME;
    }
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis2 >= ledTime)
    {
        previousMillis2 = millis();
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
}
