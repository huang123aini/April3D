//
//  ProgramCache.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/2.
//

#ifndef ProgramCache_hpp
#define ProgramCache_hpp

#include <stdio.h>
#include <string>
#include <unordered_map>

#include "Macros.h"
#include "base/Ref.h"
#include "platform/AprilPlatformMacros.h"
#include "Program.h"



BACKEND_BEGIN

class ProgramCache : public april3d::Ref {
public:
    static ProgramCache* getInstance();

    static void destroyInstance();
    
    backend::Program* getBuiltinProgram(ProgramType type) const;
    
    void removeProgram(backend::Program* program);

    void removeUnusedProgram();

    void removeAllPrograms();
    
protected:
    ProgramCache() = default;
    virtual ~ProgramCache();
    
    bool init();

    void addProgram(ProgramType type);
    
    static std::unordered_map<backend::ProgramType, backend::Program*> cached_programs;
    
    static ProgramCache *s_shared_program_cache;
};

BACKEND_END


#endif /* ProgramCache_hpp */
