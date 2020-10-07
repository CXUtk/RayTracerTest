#include "AccelStructure.h"
#include <string>
#include "OCTree.h"
#include "BVHSAH.h"
#include "BruteForce.h"

std::unique_ptr<AccelStructure> AccelStructure::makeAccelStructure(const std::string& name, int maxnodes) {
    if (name == "Oct") return std::make_unique<Octree>(maxnodes);
    else if (name == "BVH") return std::make_unique<BVHSAH>();
    return std::make_unique<BruteForce>();
}
