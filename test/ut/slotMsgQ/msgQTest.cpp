#include <iostream>
#include <gtest/gtest.h>
#include <msgQ/msgQ.hpp>

class msgQTest : public ::testing::Test {
public:
    msgQTest() : msgQ("/tmp/msgQTest", 10) {
    }

    virtual ~msgQTest() {
    }

    virtual void SetUp() {
    }

    virtual void TearDown() {
    }

protected:
    msgQ_t msgQ;
};

TEST_F(msgQTest, push) {
   msg_t msg = {1, 2, 3, {0}};
   msgQ.push(msg);
   EXPECT_EQ(msgQ.getSize(), 1);
}

TEST_F(msgQTest, pop) {
   msg_t msg = {1, 2, 3, {0}};
   msgQ.push(msg);

   msg_t pop_msg = msgQ.pop();
   EXPECT_EQ(pop_msg.from, msg.from);
   EXPECT_EQ(pop_msg.to, msg.to);
   EXPECT_EQ(pop_msg.type, msg.type);
   EXPECT_EQ(msgQ.getSize(), 0);
}

TEST_F(msgQTest, empty) {
   EXPECT_TRUE(msgQ.empty());
   msgQ.push(msg_t::invalid_msg);
   EXPECT_FALSE(msgQ.empty());
}