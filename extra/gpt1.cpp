#include <cstdint>
#include <cstring>
#include <iostream>
#include "Types.h"


class Buffer
{
public:
    Buffer(std::size_t numPackages, std::size_t packageSize)
        : numPackages_(numPackages), packageSize_(packageSize)
    {
        buffer_ = new uint8_t[numPackages * packageSize];
    }

    ~Buffer()
    {
        delete[] buffer_;
    }

    void storePackage(const Package& package, std::size_t index)
    {
        if (index < numPackages_)
        {
            std::size_t offset = index * packageSize_;
            std::memcpy(buffer_ + offset, &package, packageSize_);
        }
        else
        {
            std::cerr << "Index out of bounds." << std::endl;
        }
    }

    const Package* getPackage(std::size_t index) const
    {
        if (index < numPackages_)
        {
            std::size_t offset = index * packageSize_;
            return reinterpret_cast<const Package*>(buffer_ + offset);
        }
        else
        {
            std::cerr << "Index out of bounds." << std::endl;
            return nullptr;
        }
    }

private:
    std::size_t numPackages_;
    std::size_t packageSize_;
    uint8_t* buffer_;
};

int main()
{
    const std::size_t numPackages = 10;
    const std::size_t packageSize = sizeof(FloatPayload); // Change this based on your package type

    Buffer buffer(numPackages, packageSize);

    FloatPayload floatPayload;
    floatPayload.datagram_ID = ADS1256_PT_01;
    floatPayload.timestamp = 123456789;
    floatPayload.value = 3.14;

    buffer.storePackage(floatPayload, 0);

    const Package* retrievedPackage = buffer.getPackage(0);

    if (retrievedPackage != nullptr)
    {
        std::cout << "Retrieved Package - ID: " << retrievedPackage->datagram_ID
                  << ", Timestamp: " << retrievedPackage->timestamp << std::endl;
    }

    return 0;
}
