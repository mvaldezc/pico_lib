#include "message_format.hpp"
#include "communication_handler.hpp"
#include <gtest/gtest.h>

using namespace Communication;

TEST(QueueTest, Initialization) {
    Queue queue;
    EXPECT_TRUE(queue.isEmpty());
    EXPECT_FALSE(queue.isFull());
    EXPECT_EQ(queue.size(), 0);
}

TEST(QueueTest, PushAndPop) {
    Queue queue;
    uint8_t value;

    // Push elements into the queue
    for (uint8_t i = 0; i < 10; ++i) {
        EXPECT_TRUE(queue.push(i));
    }

    EXPECT_EQ(queue.size(), 10);
    EXPECT_FALSE(queue.isEmpty());
    EXPECT_FALSE(queue.isFull());

    // Pop elements from the queue
    for (uint8_t i = 0; i < 10; ++i) {
        EXPECT_TRUE(queue.pop(value));
        EXPECT_EQ(value, i);
    }

    EXPECT_TRUE(queue.isEmpty());
    EXPECT_FALSE(queue.isFull());
    EXPECT_EQ(queue.size(), 0);
}

TEST(QueueTest, Overflow) {
    Queue queue;

    // Fill the queue
    for (size_t i = 0; i < BUFFER_SIZE; ++i) {
        EXPECT_TRUE(queue.push(static_cast<uint8_t>(i)));
    }

    EXPECT_TRUE(queue.isFull());
    EXPECT_FALSE(queue.push(0)); // Should fail as the queue is full
}

TEST(QueueTest, Underflow) {
    Queue queue;
    uint8_t value;

    // Try to pop from an empty queue
    EXPECT_FALSE(queue.pop(value));
}

TEST(QueueTest, Clear) {
    Queue queue;

    // Fill the queue
    for (size_t i = 0; i < 10; ++i) {
        EXPECT_TRUE(queue.push(static_cast<uint8_t>(i)));
    }

    EXPECT_FALSE(queue.isEmpty());
    EXPECT_EQ(queue.size(), 10);

    // Clear the queue
    queue.clear();

    EXPECT_TRUE(queue.isEmpty());
    EXPECT_EQ(queue.size(), 0);
}

TEST(QueueTest, Front) {
    Queue queue;

    // Push elements into the queue
    for (uint8_t i = 0; i < 10; ++i) {
        EXPECT_TRUE(queue.push(i));
    }

    EXPECT_EQ(*queue.front(), 0); // The front element should be 0
}
