cmake_minimum_required(VERSION 3.10)

#添加基础模块文件

set(APRIL_BASE_HEADER
    base/Ref.h
    base/aprilConfig.h
    base/aprilMacros.h
    base/aprilTypes.h
    base/aprilUtils.h
    base/AutoreleasePool.h
    base/Director.h   
    #base/AsyncTaskPool.h 
    #base/Configuration.h
    #base/Value.h
    #base/Vector.h
    #base/Map.h
    #base/Data.h
    #base/md5.h
    ##base/Scheduler.h
   )
   

set(APRIL_BASE_SRC
    base/Ref.cpp
    base/aprilTypes.cpp
    base/aprilUtils.cpp
    base/AutoreleasePool.cpp
    base/Director.cpp
    #base/AsyncTaskPool.cpp
    #base/Configuration.cpp
    #base/Value.cpp
    #base/Data.cpp
    #base/md5.cpp
    #base/Scheduler.cpp
    )
