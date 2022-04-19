//
//  FileUtils.hpp
//  ImageKit
//
//  Created by huangshiping on 2020/8/4.
//  Copyright © 2020 huangshiping. All rights reserved.
//

#ifndef FileUtils_hpp
#define FileUtils_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <type_traits>
#include <mutex>

#include "platform/AprilPlatformMacros.h"
#include "platform/AdjustBuffer.h"
#include "base/aprilTypes.h"
#include "base/Value.h"
#include "base/Data.h"


APRIL_BEGIN

class FileUtils {
public:
  static FileUtils* GetInstance();
  static void DestroyInstance();
public:
  virtual Data GetDataFromFile(const std::string& filename) const;
  enum class Status
  {
    OK = 0,
    NotExists = 1,
    OpenFailed = 2,
    ReadFailed = 3,
    NotInitialized = 4,
    TooLarge = 5,
    ObtainSizeFailed = 6,
    NotRegularFileType = 7
  };
  template <
  typename T,
  typename Enable = typename std::enable_if<
  std::is_base_of< AdjustBuffer, AdjustBufferAdapter<T> >::value
  >::type
  >
  Status GetContents(const std::string& filename, const std::string& mode, T* buffer) const {
    AdjustBufferAdapter<T> buf(buffer);
    return GetContents(filename, mode, &buf);
  }
  virtual Status GetContents(const std::string& filename, const std::string& mode, AdjustBuffer* buffer) const;
  virtual unsigned char* GetFileData(const std::string& filename, const std::string& mode, ssize_t *size) const;
protected:
  std::recursive_mutex mutex_;
private:
  FileUtils() = default;
  virtual ~FileUtils() = default;
  FileUtils(const FileUtils&) = delete;
  FileUtils& operator=(const FileUtils&) = delete;
  static FileUtils* _instance;
  static std::mutex initMutex_;
};

APRIL_END

#endif /* FileUtils_hpp */
