#include <vector>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <math.h>
#include <inttypes.h>
// #include "SerialTransfer.h"
// #include "Arduino.h"

struct MockPackage1
{
    uint8_t  datagram_ID;  // 1 Byte - unsigned char   // Define type of next payload data 
    uint32_t timestamp;    // 4 Byte - unsigned long
    float_t  value;        // 4 Byte
} __attribute__((packed)); // attribute packed: Set smallest possible alignment, remove every padding.

struct MockPackage2
{
    uint8_t  datagram_ID;  // 1 Byte - unsigned char   // Define type of next payload data 
    uint32_t timestamp;    // 4 Byte - unsigned long
    uint8_t  value;        // 1 Byte
    char padding[2];       // 2 Bytes
} __attribute__((packed)); // attribute packed: Set smallest possible alignment, remove every padding.

class Buffer {
public:

    Buffer(size_t capacity) : capacity_(capacity), size_(0) {}

    template <typename T>
    void addItem(const T& item) {
        // Serialize the item and append it to the buffer
        const char* itemBytes = reinterpret_cast<const char*>(&item);
        buffer.insert(buffer.end(), itemBytes, itemBytes + sizeof(T));
        size_++;
    }

    template <typename T>
    T getItem(size_t index) const {
        // Deserialize the item from the buffer
        T item;
        std::memcpy(&item, &buffer[index], sizeof(T));
        return item;
    }

    bool isFull() const {
        // Implement based on your specific requirements
        return (size_*9 == capacity_); // Multiply size_ to the struct size
    }

    bool isEmpty() const {
        return buffer.empty();
    }

    void clearBuffer(){
        buffer.clear();
    }

    // void sendViaSerial() const {
    //     if (isFull()) {
    //         SerialTransfer myTransfer;  // Instantiate a SerialTransfer object

    //         // Start the serial communication with a specific baud rate
    //         Serial.begin(115200);

    //         // Wait for the serial port to be ready
    //         while (!Serial);

    //         // Configure the SerialTransfer object with the Serial port
    //         myTransfer.begin(Serial);

    //         // Send the data in the buffer via SerialTransfer
    //         if (!buffer.empty()) {
    //             myTransfer.sendDatum(buffer.data());
    //         }

    //         // Optionally, wait for the transmission to complete
    //         while (!myTransfer.available());

    //         // Close the serial port
    //         Serial.end();
    //     }
    // }

private:
    std::vector<char> buffer;
    size_t capacity_;
    size_t size_;
};

int main() {
    Buffer buffer(243);

    for (uint8_t i = 1; i < 10; i++)
    {
        MockPackage1 p;
        p  = {i, 300, 1000.0F + i };
        buffer.addItem(p);
    }

    for (uint8_t i = 10; i < 28; i++)
    {
        MockPackage2 p;
        p  = {i, 300, i};
        buffer.addItem(p);
    }

    MockPackage1 retrievedPackage1 = buffer.getItem<MockPackage1>(0);
    std::cout << "Retrieved Package: " << retrievedPackage1.datagram_ID << ", Value: " << retrievedPackage1.value << std::endl;
    printf("%p \n", retrievedPackage1.datagram_ID);
    // printf("%" PRIu8 "\n", retrievedPackage1.datagram_ID);
    // printf("%f\n", retrievedPackage1.value);

    MockPackage1 retrievedPackage2 = buffer.getItem<MockPackage1>(9);
    std::cout << "Retrieved Package: " << retrievedPackage2.datagram_ID << ", Value: " << retrievedPackage2.value << std::endl;
    printf("%p \n", retrievedPackage2.datagram_ID);
    // printf("%i\n", retrievedPackage2.value);
    

    return 0;
}

int main();