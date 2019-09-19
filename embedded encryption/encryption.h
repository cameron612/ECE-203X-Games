#ifndef ENCRYPT_H
#define ENCRYPT_H
#include "mbed.h"
#include <string>
#include <stdio.h>    
#include <stdlib.h>     
#include <time.h>  
#include <iostream>
#include <fstream>

class encryptedMessage
{
    public:
        //constructor
        encryptedMessage();
        //key getter and setter
        void setCurrentKey(char);
        char getCurrentKey();
        //message getter and setter
        void setMessage(char);
        char getMessage();
        char makeEncryption();

    
    private:
        char currentKey;
        char message;
};

#endif