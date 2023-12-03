#include <iostream>
#include <vector>
#include "Types.h"

// Circular Buffer class
class PacketBuffer {
public:

    // Constructor
    PacketBuffer(size_t capacity) : capacity_(capacity), size_(0), head_(0), tail_(0) {
        buffer_.resize(capacity, nullptr);
    }

    // Destructor
    ~PacketBuffer() {
        clear();
    }

    bool Is_BufferFull(void){
        return(IsFull_Flag);
    }

    bool Is_BufferEmpty(){
        int temp = 0;
        if((head_ == tail_) && (IsFull_Flag != 1)){
            temp = 1;
        }
        return(temp);
    }

    void addPacket(Package* packet, size_t size) {
        if(Is_BufferFull()){

        }
        else{
            if (size < sizeof(FloatPayload)) { // Change size so each one have 12 B
                // If the packet size is smaller than biggest structure, add padding
                FloatPayload* paddedPacket = new FloatPayload(*reinterpret_cast<FloatPayload*>(packet));
                buffer_[head_] = paddedPacket;
            } else {
                // If the packet size is larger, directly add the packet without padding
                buffer_[head_] = packet;
            }

            head_ = (head_ + 1) % capacity_;
            if(head_ == tail_){
                IsFull_Flag = 1;
            }
            size_++;
        }
    }

    // Remove and get the oldest packet from the buffer
    Package* getOldestPacket() {
        if(Is_BufferEmpty()){

        }
        else{
            // if (size_ > 0) {
                Package* packet = buffer_[tail_];
                buffer_[tail_] = nullptr;
                tail_ = (tail_ + 1) % capacity_;
                IsFull_Flag = 0;
                size_--;
                return packet;
            // }
        }
    }

    // Get the current size of the buffer
    size_t getSize() const {
        return size_;
    }

    void printBuffer() {
        std::cout << "Packets in the buffer:" << std::endl;
        size_t index = head_;
        for (size_t i = 0; i < size_; ++i) {
            Package* packet = buffer_[index];
            std::cout << "Packet ID: " << static_cast<int>(packet->datagram_ID)
                      << ", Timestamp: " << packet->timestamp << std::endl;
            index = (index + 1) % capacity_;
        }
    }

    // Clear all packets from the buffer
    void clear() {
        for (auto packet : buffer_) {
            delete packet;
        }
        buffer_.clear();
        size_ = 0;
        head_ = 0;
        tail_ = 0;
    }


private:
    size_t capacity_;
    size_t size_;
    size_t head_;
    size_t tail_;
    bool IsFull_Flag;
    std::vector<Package*> buffer_;
};