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

   void pushMsgTimes(msg_t msg, size_t num){
      for (int i = 0; i < num; i++)
         msgQ.push(msg);
   }

   void checkMsgsInMemoryByIndexesInMsgsArr(std::vector<size_t> indexes) {
      auto msgsInMemory = reinterpret_cast<msg_t*>(shmem.getMemory());
      for (auto index : indexes)
         EXPECT_EQ(*(msgsInMemory++), msgs[index]);
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

TEST_F(msgQTest, circleBuffer) {
   fullMsgQ();
   EXPECT_EQ(msgQ.getSize(), 10);

   auto msg = msgQ.pop();
   EXPECT_EQ(msg, msgs[0]);
   EXPECT_EQ(msgQ.getSize(), 9);

   msgQ.push(msgs[0]);
   EXPECT_EQ(msgQ.getSize(), 10);
}

TEST_F(msgQTest, circleBuffer_checkMemory) {
   fullMsgQ();
   EXPECT_EQ(msgQ.getSize(), 10);
   checkMsgsInMemoryByIndexesInMsgsArr({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

   auto msg = msgQ.pop();
   EXPECT_EQ(msg, msgs[0]);
   EXPECT_EQ(msgQ.getSize(), 9);
   checkMsgsInMemoryByIndexesInMsgsArr({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

   msgQ.push(msgs[9]);
   EXPECT_EQ(msgQ.getSize(), 10);
   checkMsgsInMemoryByIndexesInMsgsArr({ 9, 1, 2, 3, 4, 5, 6, 7, 8, 9});

   msg = msgQ.pop();
   EXPECT_EQ(msg, msgs[1]);
   EXPECT_EQ(msgQ.getSize(), 9);
   checkMsgsInMemoryByIndexesInMsgsArr({ 9, 1, 2, 3, 4, 5, 6, 7, 8, 9});

   msgQ.push(msgs[8]);
   EXPECT_EQ(msgQ.getSize(), 10);
   checkMsgsInMemoryByIndexesInMsgsArr({ 9, 8, 2, 3, 4, 5, 6, 7, 8, 9});

   EXPECT_EQ(msgQ.pop(), msgs[2]);
   EXPECT_EQ(msgQ.pop(), msgs[3]);
   EXPECT_EQ(msgQ.pop(), msgs[4]);
   EXPECT_EQ(msgQ.pop(), msgs[5]);
   checkMsgsInMemoryByIndexesInMsgsArr({ 9, 8, 2, 3, 4, 5, 6, 7, 8, 9});
   EXPECT_EQ(msgQ.getSize(), 6);

   pushMsgTimes(msgs[9], 4);
   checkMsgsInMemoryByIndexesInMsgsArr({ 9, 8, 9, 9, 9, 9, 6, 7, 8, 9});
   EXPECT_EQ(msgQ.getSize(), 10);

   EXPECT_EQ(msgQ.pop(), msgs[6]);
   EXPECT_EQ(msgQ.pop(), msgs[7]);
   EXPECT_EQ(msgQ.pop(), msgs[8]);
   EXPECT_EQ(msgQ.pop(), msgs[9]);

   pushMsgTimes(msgs[8], 4);
   EXPECT_EQ(msgQ.getSize(), 10);
   checkMsgsInMemoryByIndexesInMsgsArr({ 9, 8, 9, 9, 9, 9, 8, 8, 8, 8});

   EXPECT_EQ(msgQ.pop(), msgs[9]);
   EXPECT_EQ(msgQ.pop(), msgs[8]);
   EXPECT_EQ(msgQ.getSize(), 8);
   checkMsgsInMemoryByIndexesInMsgsArr({ 9, 8, 9, 9, 9, 9, 8, 8, 8, 8});

   msgQ.push(msgs[0]);
   msgQ.push(msgs[4]);
   EXPECT_EQ(msgQ.getSize(), 10);
   checkMsgsInMemoryByIndexesInMsgsArr({ 0, 4, 9, 9, 9, 9, 8, 8, 8, 8});
}