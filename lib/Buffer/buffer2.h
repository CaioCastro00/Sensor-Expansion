#include <vector>
#include <iostream>
#include <cstring>
#include "math.h"
// #include "SerialTransfer.h"
// #include "Arduino.h"

struct MockPackage1
{
    int  datagram_ID;   // 4 Byte - unsigned char   // Define type of next payload data 
    float timestamp;    // 4 Byte - unsigned long
    float  value;       // 4 Byte
} __attribute__((packed)); // attribute packed: Set smallest possible alignment, remove every padding.

struct MockPackage2
{
    int  datagram_ID;   // 4 Byte - unsigned char   // Define type of next payload data 
    float timestamp;    // 4 Byte - unsigned long
    int  value;       // 4 Byte
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
        return (size_*12 == capacity_); // Multiply size_ to the struct size
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
    Buffer buffer(324);

    for (uint8_t i = 1; i < 10; i++)
    {
        MockPackage1 p;
        p  = {i, 300, 1000.0F + i };
        buffer.addItem(p);
    }

    for (uint8_t i = 10; i < 28; i++)
    {
        MockPackage2 p;
        p  = {i, 300, 1000 + i };
        buffer.addItem(p);
    }

    MockPackage1 retrievedPackage1 = buffer.getItem<MockPackage1>(0);
    std::cout << "Retrieved Package: " << retrievedPackage1.datagram_ID << ", Value: " << retrievedPackage1.value << std::endl;
    printf("%f\n", retrievedPackage1.value);

    MockPackage2 retrievedPackage2 = buffer.getItem<MockPackage2>(312);
    std::cout << "Retrieved Package: " << retrievedPackage2.datagram_ID << ", Value: " << retrievedPackage2.value << std::endl;
    printf("%i\n", retrievedPackage2.value);



    // Example usage with different types of structs
    // Person person{"John Doe", 1.0F};
    // buffer.addItem(person);

    // Point point{"John Doe", 2};
    // buffer.addItem(point);

    // Person person1{"John Doe", 3.0F};
    // buffer.addItem(person1);
   

    // Send the buffer via serial
    // buffer.sendViaSerial();

    // // Retrieve items from the buffer
    // Person retrievedPerson = buffer.getItem<Person>(0);
    // std::cout << "Retrieved Person: " << retrievedPerson.name << ", Age: " << retrievedPerson.age << std::endl;
    // printf("%f\n", retrievedPerson.age);

    // Point retrievedPoint = buffer.getItem<Point>(24);
    // std::cout << "Retrieved Point: " << retrievedPoint.name << ", Age: " << retrievedPoint.age << std::endl;
    // printf("%i\n", retrievedPoint.age);

    // Person retrievedPerson1 = buffer.getItem<Person>(48);
    // std::cout << "Retrieved Person: " << retrievedPerson1.name << ", Age: " << retrievedPerson1.age << std::endl;
    // printf("%f\n", retrievedPerson1.age);
    

    return 0;
}

int main();