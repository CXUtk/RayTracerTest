#include "AccelStructure.h"
#include <string>
#include "OCTree.h"
#include "BVHSAH.h"
#include "BruteForce.h"
#include "HybridTree.h"
#include "KDTree.h"

std::unique_ptr<AccelStructure> AccelStructure::makeAccelStructure(const std::string& name, int maxnodes) {
    if (name == "Oct") return std::make_unique<Octree>(maxnodes);
    else if (name == "BVH") return std::make_unique<BVHSAH>();
    else if (name == "KD") return std::make_unique<KDTree>();
    else if (name == "Hybrid")return std::make_unique<HybridTree>();
    return std::make_unique<BruteForce>();
}
