#include <iostream>
#include <vector>
#include <windows.h>

using namespace std;

#define START 170
#define END   85
#define PREAMBLE 204
#define MAX_PAYLOAD 255

class ProtocolDecoder
{
private:
vector<uint8_t> EncodedMessage;
vector<uint8_t> data;
string decodedMessage;
uint8_t checksum, checklength;


void ExtractData()
{
    data.clear();

    for(int i = 2; i < EncodedMessage.size() - 2; i++)
    {
        data.push_back(EncodedMessage[i]);
    }

    checksum = EncodedMessage[EncodedMessage.size() - 2];
    checklength = EncodedMessage[1];
}


public:

void receivePacket(const vector<uint8_t>& packet)
{
    EncodedMessage = packet;
}

void decode()
{
    decodedMessage.clear();

    ExtractData();

    if(checklength != data.size())
    {
        cout << "Length mismatch\n";
        return;
    }

    int sum = 0;

    for(auto byte : data)
    {
        decodedMessage.push_back((char)byte);
        sum += byte;
    }

    sum = sum % 256;

    if(sum != checksum)
    {
        cout << "Checksum error\n";
        decodedMessage.clear();
    }
}

void displayDecodedMessage()
{
    if(!decodedMessage.empty())
    {
        cout << "Decoded Message: " << decodedMessage << endl;
    }
}


};

int main()
{
HANDLE hSerial;

hSerial = CreateFile("\\\\.\\COM5",
                     GENERIC_READ,
                     0,
                     NULL,
                     OPEN_EXISTING,
                     FILE_ATTRIBUTE_NORMAL,
                     NULL);

if(hSerial == INVALID_HANDLE_VALUE)
{
    cout << "Error opening serial port\n";
    return 1;
}

DCB dcbSerialParams = {0};
dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

GetCommState(hSerial, &dcbSerialParams);

dcbSerialParams.BaudRate = CBR_2400;
dcbSerialParams.ByteSize = 8;
dcbSerialParams.StopBits = ONESTOPBIT;
dcbSerialParams.Parity   = NOPARITY;

SetCommState(hSerial, &dcbSerialParams);

ProtocolDecoder decoder;

vector<uint8_t> packet;
uint8_t byte;
DWORD bytesRead;

int preambleCount = 0;

while(true)
{
    ReadFile(hSerial, &byte, 1, &bytesRead, NULL);

    // detect preamble
    if(byte == PREAMBLE)
    {
        preambleCount++;

        if(preambleCount == 4)
        {
            // next byte must be START
            ReadFile(hSerial, &byte, 1, &bytesRead, NULL);

            if(byte != START)
            {
                preambleCount = 0;
                continue;
            }

            packet.clear();
            packet.push_back(byte);

            // read LENGTH
            ReadFile(hSerial, &byte, 1, &bytesRead, NULL);

            int length = byte;

            if(length > MAX_PAYLOAD)
            {
                cout << "Invalid length\n";
                preambleCount = 0;
                continue;
            }

            packet.push_back(length);

            // read DATA
            for(int i = 0; i < length; i++)
            {
                ReadFile(hSerial, &byte, 1, &bytesRead, NULL);
                packet.push_back(byte);
            }

            // read CHECKSUM
            ReadFile(hSerial, &byte, 1, &bytesRead, NULL);
            packet.push_back(byte);

            // read END byte
            ReadFile(hSerial, &byte, 1, &bytesRead, NULL);

            if(byte != END)
            {
                cout << "Invalid packet\n";
                preambleCount = 0;
                continue;
            }

            packet.push_back(byte);

            decoder.receivePacket(packet);
            decoder.decode();
            decoder.displayDecodedMessage();

            preambleCount = 0;
        }
    }
    else
    {
        preambleCount = 0;
    }
}

CloseHandle(hSerial);

return 0;


}
