#include "mbed.h"

DigitalOut rLED(p6);
DigitalOut yLED(p7);
DigitalOut bLED(p8);
InterruptIn button(p9);


Timeout TO;
Ticker tic;

enum STATE
{
  OFF, ALERT, R, Y, B, ON, START
};

int stateatu;
int stateant;
int timer;
int buttState;
int i;


void redON();
void yellowON();
void blueON();
void alertON();
void ledOFF();
void timerInc();
void timerStart();
void timerEnd();
void startTlight();
void cycleTlight();
void bPress();
void changeLED();
void defaultS();



void redON()
{
    printf("red\n");
    rLED = 1;
    stateant = stateatu;
    stateatu = R;
    TO.attach(&changeLED, 10);
}

void yellowON()
{
    printf("yellow\n");
    yLED = 1;
    stateant = stateatu;
    stateatu = Y;
    TO.attach(&changeLED, 4);
}

void blueON()
{
    printf("blue\n");
    bLED = 1;
    stateant = stateatu;
    stateatu = B;
    TO.attach(&changeLED, 20);
}

void alertBlink(){
    yLED = !yLED;
    tic.attach(alertBlink, 0.5);
}

void alertON(){
    printf("alert\n");
    stateant = stateatu;
    stateatu = ALERT;
    alertBlink();
}

void ledOFF(){
    printf("off\n");
    stateant = stateatu;
    stateatu = OFF;
    defaultS();
}

void timerInc(){
    timer++;
}

void timerStart()
{
    TO.detach();
    timer = 0;
    tic.attach(timerInc,1);
}

void timerEnd()
{
    tic.detach();
    buttState = ON;
    changeLED();
}



void startTlight()
{
        printf("red\n");
        rLED = 1;
        stateant = R;
        stateatu = R;
        TO.attach(&changeLED, 10);
}

void cycleTlight()
{
    if(stateatu == R)
    {
        blueON();
    }
    else if(stateatu == B)
    {
        yellowON();
    }
    else if(stateatu == Y)
    {
        redON();
    }
}

void bPress()
{
    buttState = OFF;
    if(stateatu == R)
    {
        if(timer>=3 && timer<=10)
            alertON();
            
        else if(timer > 10)
            ledOFF();
            
        else
            redON();
    }
    else if(stateatu == Y)
    {
        if(timer>=3 && timer<=10)
            alertON();
            
        else if(timer > 10)
            ledOFF();
            
        else
            yellowON();
    }
    else if(stateatu == B){
        if(timer>=3 && timer<=10)
            alertON();
            
        else if(timer<=3)
            yellowON();
            
        else if(timer>10)
            ledOFF();
        
        else
            blueON();
        
    }
    else if(stateatu == ALERT)
    {
        if(timer>=3 && timer<=10)
        {
            if(stateant == R)
                redON();
            
            else if(stateant == Y)
                yellowON();
            
            else
                blueON();
            
        }
        else if(timer > 10)
            ledOFF();
        
        else
        {
            stateatu = stateant;
            alertON();
        }
    }
    else if(stateatu == OFF)
    {
        if(timer > 10)
            redON();
        
        else
            ledOFF();
        
    }
}


void changeLED()
{
    defaultS();
    if(stateatu == START && stateant == START)
        startTlight();

    else if(buttState == OFF)
        cycleTlight();
        
    else if(buttState == ON)
        bPress();

}



void defaultS()
{
  rLED = 0;
  yLED = 0;
  bLED = 0;
  
  TO.detach();
  tic.detach();
}

int main() 
{
    stateatu = START;
    stateant = START;
    changeLED();
    
    button.rise(&timerStart);
    button.fall(&timerEnd);
    
    while(1) 
    {
        wait(1);
        if(timer>0 && timer!=i)
        printf("Timer = %d\n",timer);
        i=timer;
    }
}
