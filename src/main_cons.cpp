#include "vectornav/Message.h"
#include "driver.h"
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>
#include <cstring>

int main() {
    using namespace boost::interprocess;

    // Remove shared memory on construction and destruction
    struct shm_remove {
        shm_remove() { shared_memory_object::remove("MySharedMemory"); }
        ~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
    } remover;

    // Create a shared memory object
    shared_memory_object shm(create_only, "MySharedMemory", read_write);

    // Set size
    shm.truncate(1024);

    // Map the whole shared memory in this process
    mapped_region region(shm, read_write);

    // Write a message to the shared memory
    const char *message = "Hello, world!";
    std::memcpy(region.get_address(), message, std::strlen(message) + 1);

    std::cout << "Message written to shared memory: " << message << std::endl;

    return 0;
} 