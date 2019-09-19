#include "encryption.h"

//default constructor
encryptedMessage::encryptedMessage(){}

void encryptedMessage::setCurrentKey(char newKey)
{
    this->currentKey = newKey;
}

char encryptedMessage::getCurrentKey()
{
    return this->currentKey;
}

void encryptedMessage::setMessage(char newMessage)
{
    this->message = newMessage;
}

char encryptedMessage::getMessage()
{
    return this->message;
}

char encryptedMessage::makeEncryption()
{
    char k = getCurrentKey();
    char m = getMessage();
    char e;

    
    int temp = (k - 'A') + (m - 'A') ; 
    temp = (temp + 52)%26 + 'A';
    e = (char)temp;
    
    return(e);
}
