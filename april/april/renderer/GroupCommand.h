//
//  GroupCommand.hpp
//  april3d
//
//  Created by 黄世平 on 2022/4/4.
//

#ifndef GroupCommand_hpp
#define GroupCommand_hpp

#include <stdio.h>
#include <vector>
#include <unordered_map>

#include "base/Ref.h"
#include "renderer/RenderCommand.h"

APRIL_BEGIN

class GroupCommandManager : public Ref
{
public:
    int getGroupID();
    void releaseGroupID(int groupID);

protected:
    friend class Renderer;
    GroupCommandManager();
    ~GroupCommandManager();
    bool init();
    std::unordered_map<int, bool> _groupMapping;
    std::vector<int> _unusedIDs;
};

/**
 GroupCommand is used to group several command together, and more, it can be nested.
 So it is used to generate the hierarchy for the rendcommands. Every group command will be assigned by a group ID.
 */
class  GroupCommand : public RenderCommand {
public:
    GroupCommand();
    ~GroupCommand();
    
    void init(float globalOrder);

    int getRenderQueueID() const { return render_queue_id; }
    
protected:
    int render_queue_id;
};

APRIL_END

#endif /* GroupCommand_hpp */
