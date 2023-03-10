#pragma once

#include <iostream>
#include <stdint.h>

typedef pid_t addr_t;
typedef uint32_t msg_type_t;

static const uint32_t SLOT_DATA_SIZE = 64;

struct msg_t {
   addr_t from;
   addr_t to;
   msg_type_t type;
   uint8_t data[SLOT_DATA_SIZE];

   bool isInvalid();

   static const msg_t invalid_msg;
   static const uint32_t MSG_SLOT_SIZE;

   bool operator==(const msg_t& rhs) const;
};

std::ostream& operator<<(std::ostream& os, const msg_t& msg);
const std::string msgToStr(const msg_t& msg);