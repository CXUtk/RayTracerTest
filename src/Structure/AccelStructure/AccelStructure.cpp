#include "AccelStructure.h"
#include <string>
#include "Quick.h"
#include "BVHSAH.h"
#include "BruteForce.h"

std::unique_ptr<AccelStructure> AccelStructure::makeAccelStructure(const std::string& name, int maxnodes) {
    if (name == "Quick") return std::make_unique<Quick>(1048576);
    else if (name == "BVH") return std::make_unique<BVHSAH>();
    return std::make_unique<BruteForce>();
}
