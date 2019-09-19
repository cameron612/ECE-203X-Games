#include <iostream>
#include<cstdlib>
#include<ctime>
#include <fstream>
#include "mbed.h"
#include "uLCD_4DGL.h"
#include "otp.h"
#include "Speaker.h"
#include "PinDetect.h"
#include "SDFileSystem.h"

//declare array
char cipherText [1001];

//declare SD
SDFileSystem sd(p5, p6, p7, p8, "sd");

//declare objects for pins used with pushbuttons
PinDetect pb1(p21);
PinDetect pb2(p22);
PinDetect pb3(p23);
//declare a speaker object
Speaker mySpeaker(p26);
enum InputType {YES, NO, STAY};
enum StateType {Q0, Q1, Q2, Q3, Q4};
InputType input = STAY;
StateType state = Q0;

// Callback routine is interrupt activated by a debounced pb3 hit
void pb3_hit_callback (void)
{
    // ADD CODE HERE THAT YOU WHAT TO RUN WHEN INTERUPT IS GENERATED
    input = STAY;
}

// Callback routine is interrupt activated by a debounced pb1 hit
void pb1_hit_callback (void)
{
    // ADD CODE HERE THAT YOU WHAT TO RUN WHEN INTERUPT IS GENERATED
    input = YES;
}

// Callback routine is interrupt activated by a debounced pb2 hit
void pb2_hit_callback (void)
{
    input = NO;
}

uLCD_4DGL uLCD(p9, p10, p11); // serial tx, serial rx, reset pin;

int run() {
    uLCD.printf("Save file to SD?\n");
    pb1.mode(PullUp);
    pb2.mode(PullUp);
    pb3.mode(PullUp);
    // Delay for initial pullup to take effect
    wait(.01);
    // Setup Interrupt callback functions for a pb hit
    pb1.attach_deasserted(&pb1_hit_callback);
    pb2.attach_deasserted(&pb2_hit_callback);
    pb3.attach_deasserted(&pb3_hit_callback);
    // Start sampling pb inputs using interrupts
    pb1.setSampleFrequency(); //default is 20KHz sampling
    pb2.setSampleFrequency();
    pb3.setSampleFrequency();
    // pushbuttons now setup and running
    
    while(1) 
    {
        switch(state) 
        {
            case(Q0)://neutral
                //Produce output for this state
                //calculate next state
                if (input == YES){
                    FILE *fp = fopen("/sd/mydir/OTP.txt", "w");
                    if(fp == NULL) {
                        uLCD.printf("Error Open \n");
                    }else {
                        fprintf(fp, "%s", cipherText);
                        uLCD.printf("Done! \n");
                        fclose(fp);
                    }
                    uLCD.printf("Save file to another SD?\n");
                    mySpeaker.PlayNote(200.0,0.5,0.05);
                    input = STAY;
                    state = Q1;
                }
                else if (input == NO) {
                    mySpeaker.PlayNote(200.0,0.5,0.05);
                    state = Q2;
                }
                else //input should be stay
                state = Q0;
                break;
            case (Q1)://1 yes
                //Produce output for this state
                
                //calculate next state
                if (input == YES) {
                    state = Q3;
                    mySpeaker.PlayNote(300.0,0.5,0.05);
                }
                else if (input == NO)
                state = Q2;
                else //input should be stay
                state = Q1;
                break;
            case (Q2)://1 no
                //Produce output for this state
                mySpeaker.PlayNote(400.0,0.5,0.05);
                uLCD.printf("Exiting \n");
                return 0;
            case (Q3)://2 yes
                //Produce output for this state
                FILE *fp = fopen("/sd/mydir/OTP.txt", "w");
                    if(fp == NULL) {
                        uLCD.printf("Error Open \n");
                    }else {
                        fprintf(fp, "%s", cipherText);
                        uLCD.printf("Done! \n");
                        fclose(fp);
                    }
                mySpeaker.PlayNote(500.0,0.5,0.05);
                return 2;
            case (Q4)://yesno
                //Produce output for this state
                mySpeaker.PlayNote(500.0,0.5,0.05);
                return 1;
        }//end switch
    wait (0.5);
    }
}

int main () {
    Otp myOtp;
    std::srand((unsigned int)std::time(0));
    int res;
    char alphaRes;
    int num;
    
    for(int i = 0; i < 999; ++i) {
        res = myOtp.getThrand();
        alphaRes = 'A' + res%27;
        if (alphaRes == '[') {
            alphaRes = 'A';
        }
        cipherText[i] = alphaRes;
    }
    cipherText[1000] = NULL;
    int val = run();
    
    FILE *fp = fopen("/sd/mydir/positionCipherReceiver.txt", "w");
    if(fp == NULL) {
        uLCD.printf("Error Opening Reciever \n");
    }else {
        fprintf(fp, "%d", 1);
        fclose(fp);
    }
    
    FILE *fp2 = fopen("/sd/mydir/positionCipherSender.txt", "w");
    if(fp2 == NULL) {
        uLCD.printf("Error Opening Sender \n");
    }else {
        fprintf(fp2, "%d", 1);
        fclose(fp2);
    }
    
    return 0;
}