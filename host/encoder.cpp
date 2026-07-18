#include <iostream>
#include <vector>
#include <windows.h>

using namespace std;

#define START 170
#define END   85
#define PREAMBLE 204

class ProtocolEncoder {
private:
    vector<uint8_t> data;
    uint8_t checksum, checklength;
    vector<uint8_t> encodedMessage;

    void calculateChecksum()
    {
        int sum = 0;
        for(auto c : data)
            sum += c;

        checksum = sum % 256;
    }

    void calculateLength()
    {
        checklength = data.size();
    }

    void buildpacket()
{
    

    encodedMessage.clear();

    // preamble packet
    for(int i=0;i<4;i++)
        encodedMessage.push_back(PREAMBLE);

    encodedMessage.push_back(START);
    encodedMessage.push_back(checklength);

    encodedMessage.insert(encodedMessage.end(), data.begin(), data.end());

    encodedMessage.push_back(checksum);
    encodedMessage.push_back(END);
}

public:

    void encode(const string &message)
    {
        data.clear();

        for(char c : message)
            data.push_back((uint8_t)c);

        calculateChecksum();
        calculateLength();
        buildpacket();
    }

    vector<uint8_t> getEncodedMessage()
    {
        return encodedMessage;
    }
};


int main()
{
    HANDLE hSerial;

    hSerial = CreateFile("\\\\.\\COM3",
                         GENERIC_WRITE,
                         0,
                         NULL,
                         OPEN_EXISTING,
                         FILE_ATTRIBUTE_NORMAL,
                         NULL);

    if (hSerial == INVALID_HANDLE_VALUE)
    {
        cout << "Error opening COM port\n";
        return 1;
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    GetCommState(hSerial, &dcbSerialParams);

    // Nano baud rate
    dcbSerialParams.BaudRate = CBR_2400;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity   = NOPARITY;

    SetCommState(hSerial, &dcbSerialParams);

    cout << "Type message: ";
    string msg;
    getline(cin, msg);

    ProtocolEncoder encoder;
    encoder.encode(msg);

    vector<uint8_t> packet = encoder.getEncodedMessage();

    DWORD bytesWritten;

    WriteFile(hSerial,
              packet.data(),
              packet.size(),
              &bytesWritten,
              NULL);

    cout << "Packet sent: ";

    for(auto b : packet)
        cout << (int)b << " ";

    cout << endl;

    CloseHandle(hSerial);

    return 0;
}