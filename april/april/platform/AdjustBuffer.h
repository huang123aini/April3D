//
//  AdjustBuffer.hpp
//  ImageKit
//
//  Created by huangshiping on 2020/8/4.
//  Copyright © 2020 huangshiping. All rights reserved.
//

#ifndef ADJUSTBUFFER_HPP
#define ADJUSTBUFFER_HPP

#include <stdio.h>
#include <string>

#include "platform/AprilPlatformMacros.h"
#include "base/Data.h"

APRIL_BEGIN

class AdjustBuffer {
public:
  virtual ~AdjustBuffer() {}
  virtual void Resize(size_t size) = 0;
  virtual void* Buffer() const = 0;
};

template<typename T> class AdjustBufferAdapter{};

template<typename CharT, typename Traits, typename Allocator>
class AdjustBufferAdapter< std::basic_string<CharT, Traits, Allocator> > : public AdjustBuffer {
  typedef std::basic_string<CharT, Traits, Allocator> BufferType;
  BufferType* buffer_;
public:
  explicit AdjustBufferAdapter(BufferType* buffer) : buffer_(buffer) {}
  virtual void Resize(size_t size) override {
    buffer_->resize((size + sizeof(CharT) - 1) / sizeof(CharT));
  }
  virtual void* Buffer() const override {
    if (buffer_->empty()) {
      return nullptr;
    } else {
      return &buffer_->front();
    }
  }
};

template<typename T, typename Allocator>
class AdjustBufferAdapter< std::vector<T, Allocator> > : public AdjustBuffer {
  typedef std::vector<T, Allocator> BufferType;
  BufferType* buffer_;
public:
  explicit AdjustBufferAdapter(BufferType* buffer) : buffer_(buffer) {}
  virtual void Resize(size_t size) override {
    buffer_->Resize((size + sizeof(T) - 1) / sizeof(T));
  }
  virtual void* Buffer() const override {
    if (buffer_->empty())
    {
      return nullptr;
    } else {
      return &buffer_->front();
    }
  }
};

template<>
class AdjustBufferAdapter<Data> : public AdjustBuffer {
    typedef Data BufferType;
    BufferType* buffer_;
public:
    explicit AdjustBufferAdapter(BufferType* buffer) : buffer_(buffer) {}
    virtual void Resize(size_t size) override {
        size_t oldSize = static_cast<size_t>(buffer_->getSize());
        if (oldSize != size) {
            auto old = buffer_->getBytes();
            void* buffer = realloc(old, size);
            if (buffer)
                buffer_->fastSet((unsigned char*)buffer, size);
        }
    }
    virtual void* Buffer() const override {
        return buffer_->getBytes();
    }
};

APRIL_END

#endif /* AdjustBuffer_hpp */
