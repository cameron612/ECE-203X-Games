#ifndef ENCRYPT_H
#define ENCRYPT_H
#include "mbed.h"
#include <string>
#include <stdio.h>    
#include <stdlib.h>     
#include <time.h>  
#include <iostream>
#include <fstream>

class decryptedMessage
{
    public:
        //constructor
        decryptedMessage();
        //key getter and setter
        void setCurrentKey(char);
        char getCurrentKey();
        //message getter and setter
        void setMessage(char);
        char getMessage();
        char makeDecryption();

    
    private:
        char currentKey;
        char message;
};

#endif