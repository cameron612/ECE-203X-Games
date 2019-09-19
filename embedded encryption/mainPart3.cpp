#include "mbed.h"
#include "SDFileSystem.h"
#include "uLCD_4DGL.h"
#include<iostream>
#include<fstream>
#include "PinDetect.h"
#include "decryption.h"
using namespace std;
SDFileSystem sd(p5, p6, p7, p8, "sd");
uLCD_4DGL uLCD(p9, p10, p11);

PinDetect pb1(p21);
PinDetect pb2(p22);
PinDetect pb3(p23);

enum InputType {FWD, BACK, ENC, STAY};
enum StateType {none, next, back, decrypt};
InputType input = STAY;
StateType state = none;
// Callback routine is interrupt activated by a debounced pb3 hit
void pb3_hit_callback (void)
{
    input = ENC;
}
// Callback routine is interrupt activated by a debounced pb1 hit
void pb1_hit_callback (void)
{
    input = FWD;
}
// Callback routine is interrupt activated by a debounced pb2 hit
void pb2_hit_callback (void)
{
    input = BACK;
}

decryptedMessage c;

int main()
{
    FILE *fp = fopen("/sd/mydir/OTP.txt", "r");
    if(fp == NULL) {
        uLCD.printf("Open Error!!!\n"); 
    }
    else
    {
    pb1.mode(PullUp);
    pb2.mode(PullUp);
    pb3.mode(PullUp);
    wait(.01);
    pb1.attach_deasserted(&pb1_hit_callback);
    pb2.attach_deasserted(&pb2_hit_callback);
    pb3.attach_deasserted(&pb3_hit_callback);
    // Start sampling pb inputs using interrupts
    pb1.setSampleFrequency(); //default is 20KHz sampling
    pb2.setSampleFrequency();
    pb3.setSampleFrequency();
    
    //GET KEY
    string key;
    fscanf(fp, "%s", key);
    fclose(fp);                   
    
    int initPos;
    FILE *fp = fopen("/sd/mydir/positionCipherReceiver.txt", "r");
    if(fp == NULL) {
        uLCD.printf("Error Opening Sender \n");
    }else {
        fscanf(fp, "%d", &initPos);
        fclose(fp);
    }

    string alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char letter;
    int pos = 0;

    
    
    while(1) {

    switch(state)
    {
        case(none):
            letter = alpha[pos];
            
            uLCD.locate(0,0);
            uLCD.text_width(5);
            uLCD.text_height(5);
            uLCD.printf("%c",letter);
            if (input == FWD)
                state = next;
            else if (input == BACK)
                state = back;
            else if(input == ENC)//input should be stay
                state = decrypt;
            else
                state = none;
            break;

        case (next):
            pos = (++pos)%(alpha.length());
            letter = alpha[pos];
            uLCD.locate(0,0);
            uLCD.text_width(5);
            uLCD.text_height(5);
            uLCD.printf("%c     ",letter);
            
            input = STAY;
            state = none;
            break;


        case (back):
            pos = (--pos+(alpha.length()))%(alpha.length());
            letter = alpha[pos];
            uLCD.locate(0,0);
            uLCD.text_width(5);
            uLCD.text_height(5);
            uLCD.printf("%c      ",letter);
            input = STAY;
            state = none;
            break;
            
            
        case (decrypt):
            c.setCurrentKey(key[initPos-1]);
            c.setMessage( alpha[pos]);
            FILE *fp2 = fopen("/sd/mydir/positionCipherReceiver.txt", "w");
            if(fp2 == NULL) {
                uLCD.printf("Error Opening Sender \n");
            }else {
                initPos++;
                freopen(NULL,"w",fp2);
                fprintf(fp2,"%d", initPos );
                fclose(fp2);
            }
            char decryptedMessage = c.makeDecryption();
            letter = alpha[pos];
        
            uLCD.locate(0,0);
            uLCD.text_width(5);
            uLCD.text_height(5);
            uLCD.printf("%c %c",letter, decryptedMessage);
            //calculate next state
            input = STAY;
            state = none;
            break;

    }//end switch
    wait (0.5);
    }
    }
}