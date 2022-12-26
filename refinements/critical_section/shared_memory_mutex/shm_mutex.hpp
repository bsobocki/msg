#pragma once

#ifndef CRITICAL_SECTION_HPP
#define CRITICAL_SECTION_HPP

#include <iostream>
#include "shmem_segment.hpp"
#include <mutex>

// key value for the shared memory
static const char* mutexFilePath = "critical_section.lock";
static const auto mutexSize = sizeof(std::mutex);
static const auto boolSize = sizeof(bool);

class shm_mutex_t {
public:
   shm_mutex_t(const char* shmemFilePath = mutexFilePath);
   ~shm_mutex_t() = default;
   void initialize();

   void lock();
   void unlock();

private:
   bool hasBeenInitialized; 
   shmem_segment_t shmem;
   std::mutex* mutexMem;
};

#endif