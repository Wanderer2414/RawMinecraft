#ifndef PATH_CREATOR_H
#define PATH_CREATOR_H
#include "Path/Path.h"
#include "WorldRender.h"
namespace MyCraft {
    class PathCreator {
    public:
        PathCreator(WorldRender& world);
        ~PathCreator();

        Path FollowPath(const glm::vec3& size, const glm::vec3& source, const glm::vec3& destination)  const;
        Path PatrolPath(const glm::vec3& size, const glm::vec3& source)                                const;
        Path RunAwayPath(const glm::vec3& size, const glm::vec3& source)                               const;
        void Pair(Path& path)                                                   const;
    protected:
    private:
        WorldRender& __worldRender;
    };
}
#endif