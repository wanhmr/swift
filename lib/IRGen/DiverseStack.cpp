//===--- DiverseStack.cpp - Out-of-line code for the heterogenous stack ---===//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2015 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See http://swift.org/LICENSE.txt for license information
// See http://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//
//
//  This file implements the small amount of code for the heterogenous
//  stack and list classes.
//
//===----------------------------------------------------------------------===//

#include "DiverseList.h"
#include "DiverseStack.h"
using namespace swift;
using namespace irgen;

void DiverseStackBase::pushNewStorageSlow(std::size_t needed) {
  bool wasInline = isAllocatedInline();

  std::size_t capacity = End - Allocated;
  std::size_t requiredCapacity = capacity + needed;
  do {
    capacity = 2 * capacity + 16;
  } while (capacity < requiredCapacity);

  assert(capacity % 16 == 0 && "not allocating multiple of alignment");

  char *oldAllocation = Allocated;
  char *oldBegin = Begin;
  std::size_t oldSize = (std::size_t) (End - oldBegin);

  Allocated = new char[capacity];
  End = Allocated + capacity;
  Begin = End - oldSize;
  std::memcpy(Begin, oldBegin, oldSize);

  if (!wasInline) delete[] oldAllocation;
}  

char *DiverseListBase::addNewStorageSlow(std::size_t needed) {
  bool wasInline = isAllocatedInline();

  std::size_t capacity = EndOfAllocation - Begin;
  std::size_t requiredCapacity = capacity + needed;
  do {
    capacity = 2 * capacity + 16;
  } while (capacity < requiredCapacity);

  assert(capacity % 16 == 0 && "not allocating multiple of alignment");

  char *oldBegin = Begin;
  char *oldEnd = End;
  std::size_t oldSize = (std::size_t) (oldEnd - oldBegin);

  Begin = new char[capacity];
  EndOfAllocation = Begin + capacity;
  End = Begin + oldSize;
  std::memcpy(Begin, oldBegin, oldSize);

  if (!wasInline) delete[] oldBegin;

  return Begin + oldSize;
}  
