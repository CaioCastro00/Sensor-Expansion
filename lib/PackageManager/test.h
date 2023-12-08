#pragma once

#include <iostream>
#include <vector>
#include "Types.h"

/*

+ Theoretical Description

    In computer science, a circular buffer, circular queue,
    cyclic buffer or ring buffer is a data structure that uses a single,
    fixed-size buffer as if it were connected end-to-end.
    This structure lends itself easily to buffering data streams.

    Circular buffering makes a good implementation strategy for a queue that has fixed maximum size.
    Should a maximum size be adopted for a queue, then a circular buffer is a completely ideal implementation;
    all queue operations are constant time.

    A circular buffer can be implemented using a pointer and three integers:

        -> buffer start in memory
        -> buffer capacity (length)
        -> write to buffer index (end)
        -> read from buffer index (start)

    Image description: https://upload.wikimedia.org/wikipedia/commons/thumb/f/fd/Circular_Buffer_Animation.gif/400px-Circular_Buffer_Animation.gif

+ Pratical Example

    You're tasked with getting data from  thousands of low-powered sensors to a database.
    You realize that maintaining connections for each sensor to the database is not a good architectural decision.
    Additionally, hitting the database witha insert for every sensor read will strain the backend.
    Thus, you want to create a co-located emitter that can receveive sensor data and emit it in baches every second to the database.

    This situation require consideration around:
        1- What should our max batch size be?
        2- What should we do if sensor data enters when the batch size is reached?
        (Can be caused by database conncetion issue, outage or sensor emitting to much data)

    While this conditionis unideal, there's a commonly taken trade-off here.
    Rather than trying to deal with backpressure, it may be bes t to jsut dorp the oldest data stored in the emitter and ensure whatever is queued up for the next emit is the newest.
    This way, while there's data loss, we can ensure we have the most up-to-date signal form the sensors.

*/

enum MockSensorID : uint8_t
{
    INIT_0,
    ADS1256_PT,
};

struct MockPackage
{
    uint8_t  datagram_ID;  // 1 Byte - unsigned char   // Define type of next payload data // 
    uint32_t timestamp;    // 4 Byte - unsigned long
    float_t  value;        // 4 Byte
} __attribute__((packed)); // attribute packed: Set smallest possible alignment, remove every padding.


template <class T>
class RingBuffer{

private:

    T* data;
    size_t size_;
    size_t lastInsert_;
    size_t nextRead_;
    // time.Time emitTime;

public:

    // Constructor
    RingBuffer(size_t size) : size_(size), data(new T[size]), lastInsert_(-1), nextRead_(0) {
        static_assert(std::is_default_constructible<T>::value, "T must be default-constructible");
    }

    // Destructor
    ~RingBuffer(){
        delete[] data;
    }

    // Insert data into buffer
    void Insert(const T& input){

        // len = 5
        // (3 + 1) % 5 = 4
        // (4 + 1) % 5 = 0
        // 5 % 5 = 0

        lastInsert_ = (lastInsert_ +1) % size_;
        data[lastInsert_] = input;

        if (nextRead_ == lastInsert_) {
            nextRead_ = (nextRead_ + 1) % size_;
        }
    }

    // Emit and remove data from buffer
   std::vector<T> Emit(){

        std::vector<T> output;

        while (true) {
            if (data[nextRead_] != T{}){
                output.push_back(data[nextRead_]);
                data[nextRead_] = T{};
            }

            if (nextRead_ == lastInsert_ || lastInsert_ == static_cast<size_t>(-1)){
               break; 
            }

            nextRead_ = (nextRead_ + 1) % size_;

        }

        return output;
    }

};

template <class T>
void testRingBuffer(size_t bufferSize) {
    // Create a RingBuffer of the specified size
    RingBuffer<T> buffer(bufferSize);

    // Insert some elements into the buffer
    for (int i = 1; i <= 5; ++i) {
        MockPackage package;
        package.datagram_ID = static_cast<uint8_t>(i);
        package.timestamp = i * 1000;  // Just a placeholder timestamp
        package.value = i * 1.1;       // Just a placeholder value
        buffer.Insert(package);
    }

    // Emit and print the elements from the buffer
    std::vector<MockPackage> emitted = buffer.Emit();

    // Print the emitted elements
    std::cout << "Emitted Elements:\n";
    for (const auto& element : emitted) {
        std::cout << "ID: " << static_cast<int>(element.datagram_ID)
                  << ", Timestamp: " << element.timestamp
                  << ", Value: " << element.value << "\n";
    }
}

int main() {
    // Test the RingBuffer with MockPackage elements and a buffer size of 10
    testRingBuffer<MockPackage>(10);

    return 0;
}