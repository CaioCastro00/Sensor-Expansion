#ifndef PACKAGE_MANAGER_H
#define PACKAGE_MANAGER_H

#include <Arduino.h>
#include <stdlib.h>
#include <cstddef>
#include <memory_resource>
#include <iostream>
#include "math.h"
#include "Wire.h"
#include "SerialTransfer.h"
#include "PackageTypes.h"
#include "CommunicationManager.h"

/*
static unsigned long deletes_left_to_do = 0;

void *operator new(std::size_t n){
    void *p = std::malloc(n);
    if (p == nullptr){
        // throw std::bad_alloc();
    }
    ++deletes_left_to_do;
    return p;
}

void operator delete(void *p) noexcept{
    --deletes_left_to_do;
    std::free(p);
}

*/


/*
struct AllocationMetrics{

    uint32_t TotalAllocated = 0;
    uint32_t TotalFreed = 0;

    uint32_t CurrentUsage(){ return  TotalAllocated - TotalFreed;}
    
};

static AllocationMetrics _AlloctionMetrics;

void* operator new(size_t size){
    std::cout << "Allocating" << size << "bytes\n";

    return malloc(size);
}

void operator delete(void* memory, size_t size){
    std::cout << "Freeing" << size << "bytes\n";

    free(memory);
}
*/

/*
void showmemory(unsigned char* buffer, std::size_t buffer_size,const char* headline = ""){
    if (headline != "")
        std::cout << headline << std::endl;

    std::cout << "&buffer=0x" << std::hex << (unsigned int)(buffer)<< " " << std::dec << buffer_size << " bytes" << std::endl;

    int i = 0;

    while (i < buffer_size){
        int first = i;
        int last = i + std::min(10, int(buffer_size - first));
        std::cout << "&=" << std::setw(2) << std::hex << std::size_t(first);
        std::cout << " asc: ";

        for (int k = first; k < last; k++){
            if ((buffer[k] >= 32) and (buffer[k] <= 127))
            std::cout << buffer[k];
            else
            std::cout << ".";
        }

        i = i + 10;
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
*/

/*

// Default Global Allocator
class GlobalAllocator{
    // no data memebers
    public:
    // CREATORS
    GlobalAllocator(){}
    GlobalAllocator(const GlobalAllocator&){}
    ~GlobalAllocator(){}

    // MANIPULATORS
    GlobalAllocator& operator=(const GlobalAllocator&) = delete;
    void *allocate(std::size_t nBytes){
        return ::operator new(nBytes);
    }

    void deallocate(void *address){
        ::operator delete(address);
    }
};

// FREE OPERATORS
bool operator==(const GlobalAllocator&, const GlobalAllocator&){
    return true;
}

class LocalAllocator{
    // internal data structure
    public:
    // CREATORS
    LocalAllocator(void *begin, void *end);
    LocalAllocator(const LocalAllocator&) = delete;
    LocalAllocator& operator=(const LocalAllocator&) = delete;
    
    // MANIPULATORS
    void *allocate(std::size_t nBytes);
    void deallocate(void *address);
    void release(); // local allocators only
};

*/

struct MockPackage1
{
  uint8_t datagram_ID;     // 1 Byte - unsigned char   // Define type of next payload data
  uint32_t timestamp;      // 4 Byte - unsigned long
  float_t value;           // 4 Byte
} __attribute__((packed)); // attribute packed: Set smallest possible alignment, remove every padding.

struct MockPackage2
{
  uint8_t datagram_ID;     // 1 Byte - unsigned char   // Define type of next payload data
  uint32_t timestamp;      // 4 Byte - unsigned long
  uint8_t value;           // 1 Byte
  char padding[3];         // 3 Bytes
} __attribute__((packed)); // attribute packed: Set smallest possible alignment, remove every padding.

// namespace pmr {
//     template <class T>
//     using vector = std::vector<T, polymorphic_allocator<T>>;
// }

class PackageManager
{
public:
    PackageManager(size_t capacity, size_t datasize, SerialTransfer *serialTransferPtr) : _serialTransfer(serialTransferPtr), _capacity(capacity), _size(0), _datasize(datasize)
    {
        buffer.reserve(capacity);
    }

    // Buffer(const Buffer& buffer) : capacity_(buffer.capacity_), _size(buffer._size), _datasize(buffer._datasize) {
    //     std::cout << "copied" << std::endl;
    // }

    template <typename T>
    void addItem(const T &item)
    {
        // Serialize the item and append it to the buffer
        const char *itemBytes = reinterpret_cast<const char *>(&item);
        buffer.insert(buffer.end(), itemBytes, itemBytes + sizeof(T));
        _size++;
    }

    template <typename T>
    T getItem(size_t index) const
    {
        // Deserialize the item from the buffer
        T item;
        std::memcpy(&item, &buffer[index], sizeof(T));
        return item;
    }

    bool isFull() const
    {
        return (_size * _datasize == _capacity);
    }

    bool isEmpty() const
    {
        return buffer.empty();
    }

    void clearBuffer()
    {
        buffer.clear();
        _size = 0;
    }

    size_t getSize()
    {
        return _size;
    }

    void sendViaSerial()
    {
        // Send PackageHeader

        // size_t lastPayloadBufferSize = (getSize() % PAYLOAD_BUFFER_MAX_PACKAGES) * PACKAGE_SIZE;

        size_t flushQtd = getSize() <= PAYLOAD_BUFFER_MAX_PACKAGES ? 1 : 1 + (getSize() / PAYLOAD_BUFFER_MAX_PACKAGES);
        size_t lastFlushSize = (getSize() % PAYLOAD_BUFFER_MAX_PACKAGES) == 0 ? PAYLOAD_BUFFER_MAX_SIZE : (getSize() % PAYLOAD_BUFFER_MAX_PACKAGES) * PACKAGE_SIZE;

        Serial.print("lastFlushSize: ");
        Serial.println(lastFlushSize);

        Serial.print("flushQtd: ");
        Serial.println(flushQtd);

        // Send buffer divided into payloads packages
        for (size_t i = 1; i <= flushQtd; i++)
        {
              Serial.print("for  aaa: ");
              Serial.println(i);
            size_t bufferOfSet = (i - 1) * PAYLOAD_BUFFER_MAX_SIZE;

            // Last flush
            if (i == flushQtd)
            {
                _serialTransfer->sendDatum(buffer[bufferOfSet], lastFlushSize);
                break;
            }
            _serialTransfer->sendDatum(buffer[bufferOfSet], PAYLOAD_BUFFER_MAX_SIZE);
        }
        clearBuffer();
    }

    // std::pmr::vector<std::byte> buffer;
    std::vector<char> buffer; // Should be private for security measure

private:
    SerialTransfer *_serialTransfer;
    size_t _capacity;
    size_t _size;
    size_t _datasize;
};


// int main() {
//     Buffer buffer(243, 9);

//     for (uint8_t i = 1; i < 10; i++)
//     {
//         MockPackage1 p;
//         p = {25, 200, 1000.0F + i};
//         buffer.addItem(p);
//     }


//     for (uint8_t i = 10; i < 28; i++)
//     {
//         MockPackage2 p;
//         p  = {i, 300, static_cast<uint8_t>(i%2) };
//         buffer.addItem(p);
//     }

//     MockPackage1 retrievedPackage1 = buffer.getItem<MockPackage1>(0);
//     std::cout << "Retrieved Package: " << retrievedPackage1.datagram_ID << ", Value: " << retrievedPackage1.value << std::endl;
//     printf("%f\n", retrievedPackage1.value);

//     MockPackage2 retrievedPackage2 = buffer.getItem<MockPackage2>(312);
//     std::cout << "Retrieved Package: " << retrievedPackage2.datagram_ID << ", Value: " << retrievedPackage2.value << std::endl;
//     printf("%i\n", retrievedPackage2.value);
   

//     // Send the buffer via serial
//     // buffer.sendViaSerial();

//     return 0;
// }

// int main();

#endif