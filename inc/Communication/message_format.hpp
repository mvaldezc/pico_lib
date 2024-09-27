/***********************************************************************
 * @file	:	message_format.hpp
 * @brief 	:	Message Format Interface
 *              Signature of message frame handlers and data structures.
 * @author	:	Marco Valdez @mvaldezc
 *
 ***********************************************************************/

#pragma once
#include <cstdint>
#include <cstring>

namespace Communication {

    /**
     * @brief Message id data type 
     */
    typedef uint8_t RxMessageId;

    /**
     * @brief Function pointer signature that is called by the system when a received message
     *        interruption occurs.
     */
    typedef void (*RxHandler)(RxMessageId msgId, size_t dataLength, uint8_t * data);

    /**
     * @brief Function pointer signature that is called by the system when a message request
     *        interruption occurs.
     */
    typedef void (*TxHandler)(uint8_t * data);

    constexpr size_t BUFFER_SIZE = 32; // Maximum message length in bytes

    /**
     * @brief Circular buffer data structure to store received messages through ISR. Not thread safe.
     * @tparam T Data type to store in the buffer
     */
    template <typename T>
    struct Queue
    {
        T data_[BUFFER_SIZE];
        size_t readNode;
        size_t writeNode;
        size_t capacity;

        Queue() : readNode(0), writeNode(0), capacity(0) {}

        inline bool isFull() const
        {
            return capacity == BUFFER_SIZE;
        }

        inline bool isEmpty() const
        {
            return capacity == 0;
        }

        inline bool push(T data)
        {
            if (isFull()) {
                return false; // Buffer is full
            }
            data_[writeNode] = data;
            writeNode = (writeNode + sizeof(T)) % (BUFFER_SIZE / sizeof(T));
            capacity++;
            return true;
        }

        inline bool pop(T & data)
        {
            if (isEmpty()) {
                return false; // Buffer is empty
            }
            data = data_[readNode];
            readNode = (readNode + sizeof(T)) % (BUFFER_SIZE / sizeof(T));
            capacity--;
            return true;
        }

        inline void clear()
        {
            memset(data_, 0, BUFFER_SIZE);
            readNode = 0;
            writeNode = 0;
            capacity = 0;
        }

        inline size_t size() const
        {
            return capacity;
        }

        inline T * front()
        {
            return & data_[readNode];
        }
    };

} // namespace Communication
