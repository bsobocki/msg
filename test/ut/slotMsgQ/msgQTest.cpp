#include <iostream>
#include <gtest/gtest.h>
#include <msgQ/msgQ.hpp>

namespace {
   static const size_t msgQSize = 10;
}

class msgQTest : public ::testing::Test {
public:
    msgQTest() :
      shmem( msg_t::MSG_SLOT_SIZE * msgQSize),
      msgQ(shmem, msgQSize) {
        for (int i = 0; i < msgQSize; i++)
            msgs[i] = {i, i+2, i+3, {0}};
    }

    virtual ~msgQTest() {
    }

    virtual void SetUp() {
    }

    virtual void TearDown() {
    }

    void pushMsgs(size_t num) {
        for(int i = 0; i < num; i++){
            msgQ.push(msgs[i]);
        }
    }

    void fullMsgQ() {
       pushMsgs(msgQ.getCapacity());
    }

protected:
    msg_t msgs[10];
    shmem_segment_t shmem;
    msgQ_t msgQ;
};

TEST_F(msgQTest, push) {
   msg_t msg = {1, 2, 3, {0}};
   msgQ.push(msg);
   EXPECT_EQ(msgQ.getSize(), 1);
   EXPECT_EQ(msg, msgQ.peek());
}

TEST_F(msgQTest, pop) {
   msg_t msg = {1, 2, 3, {0}};
   std::cout<<"beeelo"<<std::endl;
   msgQ.push(msg);
   std::cout<<"eeelo"<<std::endl;
   msg_t pop_msg = msgQ.pop();
   std::cout<<"meeelo"<<std::endl;
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

TEST_F(msgQTest, full) {
   EXPECT_FALSE(msgQ.full());
   fullMsgQ();
   EXPECT_TRUE(msgQ.full());
}

TEST_F(msgQTest, peek) {
   msg_t msg = {1, 2, 3, {0}};
   msgQ.push(msg);

   msg_t peek_msg = msgQ.peek();
   EXPECT_EQ(peek_msg.from, msg.from);
   EXPECT_EQ(peek_msg.to, msg.to);
   EXPECT_EQ(peek_msg.type, msg.type);
   EXPECT_EQ(msgQ.getSize(), 1);
}

TEST_F(msgQTest, getSize) {
   EXPECT_EQ(msgQ.getSize(), 0);
   msgQ.push(msg_t::invalid_msg);
   EXPECT_EQ(msgQ.getSize(), 1);
   pushMsgs(4);
   EXPECT_EQ(msgQ.getSize(), 5);
}

TEST_F(msgQTest, circleBuffer){
    fullMsgQ();
    EXPECT_EQ(msgQ.getSize(), 10);

    auto msg = msgQ.pop();
    EXPECT_EQ(msg, msgs[0]);
    EXPECT_EQ(msgQ.getSize(), 9);

    msgQ.push(msgs[0]);
    EXPECT_EQ(msgQ.getSize(), 10);
}