/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "MemoryManagerAdapter.h"

#include <stdexcept>
#include <utility>

#include "flashlight/common/Utils.h"

namespace fl {

MemoryManagerAdapter::MemoryManagerAdapter(
    std::shared_ptr<MemoryManagerDeviceInterface> interface,
    std::ostream* logStream)
    : deviceInterface(interface), logStream_(logStream) {
  if (!interface) {
    throw std::invalid_argument(
        "MemoryManagerAdapter::MemoryManagerAdapter - "
        "memory manager device interface is null");
  }
  if (logStream_) {
    loggingEnabled_ = true;
  }

  // Create handle and set payload to point to this instance
  AF_CHECK(af_create_memory_manager(&interface_));
  af_memory_manager_set_payload(interface_, (void*)this);
}

MemoryManagerAdapter::~MemoryManagerAdapter() {
  // Flush the log buffer and log stream
  *logStream_ << logStreamBuffer_.str();
  logStream_->flush();

  af_release_memory_manager(interface_);
}

void MemoryManagerAdapter::setLogStream(std::ostream* logStream) {
  logStream_ = logStream;
}

void MemoryManagerAdapter::setLoggingEnabled(bool log) {
  loggingEnabled_ = log;
}

void MemoryManagerAdapter::setLogFlushInterval(size_t interval) {
  if (interval < 1) {
    throw std::invalid_argument(
        "MemoryManagerAdapter::setLogFlushInterval - "
        "flush interval must be great than zero.");
  }
  logFlushInterval_ = interval;
}

const af_memory_manager MemoryManagerAdapter::getHandle() const {
  return interface_;
}

} // namespace fl
