#include "decryption.h"

//default constructor
decryptedMessage::decryptedMessage(){}

void decryptedMessage::setCurrentKey(char newKey)
{
    this->currentKey = newKey;
}

char decryptedMessage::getCurrentKey()
{
    return this->currentKey;
}

void decryptedMessage::setMessage(char newMessage)
{
    this->message = newMessage;
}

char decryptedMessage::getMessage()
{
    return this->message;
}

char decryptedMessage::makeDecryption()
{
    char k = getCurrentKey();
    char m = getMessage();
    char e;

    
    int temp = m - k + 26; 
    temp = (temp)%26 + 'A';
    e = (char)temp;
    
    return(e);
}
