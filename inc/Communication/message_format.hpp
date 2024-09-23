/***********************************************************************
 * @file	:	message_format.hpp
 * @brief 	:	Message Format Interface
 *              Signature of message frame handlers.
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

    constexpr size_t BUFFER_SIZE = 256; // Maximum message length in bytes

    /**
     * @brief Circular buffer data structure to store received messages through ISR. Not thread safe.
     */
    struct Queue
    {
        uint8_t data_[BUFFER_SIZE];
        uint8_t readNode;
        uint8_t writeNode;
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

        inline bool push(uint8_t data)
        {
            if (isFull()) {
                return false; // Buffer is full
            }
            data_[writeNode] = data;
            writeNode = (writeNode + 1) % BUFFER_SIZE;
            capacity++;
            return true;
        }

        inline bool pop(uint8_t &data)
        {
            if (isEmpty()) {
                return false; // Buffer is empty
            }
            data = data_[readNode];
            readNode = (readNode + 1) % BUFFER_SIZE;
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

        inline uint8_t * front()
        {
            return & data_[readNode];
        }
    };

} // namespace Communication
