//
//  FileUtils.cpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/7.
//

#include "platform/FileUtils.h"

#include <stack>

#include "base/Data.h"
#include "base/aprilMacros.h"

#include <stack>
#include <sys/stat.h>
#include <assert.h>

#define DECLARE_GUARD std::lock_guard<std::recursive_mutex> mutexGuard(mutex_)

APRIL_BEGIN

FileUtils* FileUtils::_instance = nullptr;
std::mutex FileUtils::initMutex_;

void FileUtils::DestroyInstance() {
  if (_instance != nullptr) {
    delete _instance;
    _instance = nullptr;
  }
}

FileUtils* FileUtils::GetInstance() {
  if(_instance == nullptr) {
    std::lock_guard<std::mutex> lock(initMutex_);
    if (_instance == nullptr) {
      _instance = new FileUtils;
    }
  }
  return _instance;
}


Data FileUtils::GetDataFromFile(const std::string& filename) const {
  Data d = Data();
  std::string mode = "rb";
  GetContents(filename, mode, &d);
  return d;
}

FileUtils::Status FileUtils::GetContents(const std::string& filename, const std::string& mode, AdjustBuffer* buffer) const {
  if (filename.empty()) {
    return Status::NotExists;
  }
  if (mode.empty()) {
    return Status::OpenFailed;
  }
  struct stat statBuf;
  if (stat(filename.c_str(), &statBuf) == -1) {
    return Status::ReadFailed;
  }
  if (!(statBuf.st_mode & S_IFREG)) {
    return Status::NotRegularFileType;
  }
  FILE *fp = fopen(filename.c_str(), mode.c_str());
  if (!fp) {
    return Status::OpenFailed;
  }
  size_t size = statBuf.st_size;
  buffer->Resize(size);
  size_t readsize = fread(buffer->Buffer(), 1, size, fp);
  fclose(fp);
  if (readsize < size) {
    buffer->Resize(readsize);
    return Status::ReadFailed;
  }
  return Status::OK;
}

unsigned char* FileUtils::GetFileData(const std::string& filename, const std::string& mode, ssize_t *size) const {
  /**ASSERT(!filename.empty() && size != nullptr && !mode.empty(), "Invalid parameters.");*/
  Data d;
  if (GetContents(filename, mode ,&d) != Status::OK) {
    *size = 0;
    return nullptr;
  }
  return d.takeBuffer(size);
}

APRIL_END
