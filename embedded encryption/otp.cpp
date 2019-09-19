#include <iostream>
#include<cstdlib>
#include<ctime>
#include <fstream>
#include "TMP36.h"

TMP36 myTMP36(p15);
class Otp {
    int rand;
    float thrand;
public:
    int getRand ();
    int getThrand ();
};

int Otp::getRand () {
    int random = std::rand();
    return random;
}

int Otp::getThrand () {
    Otp::rand = Otp::getRand();
    //take temps
    float tempC, diffC;
    tempC = myTMP36.read();
    diffC = tempC - myTMP36.read();
    Otp::thrand = 1000*diffC;
    return (int) (Otp::rand + Otp::thrand);
}