#include "TorchSystem.h"
#include <algorithm>

std::vector<TorchInstance> torches;

void AddTorch(const glm::vec3& pos) {
    torches.push_back({pos});
}

void RemoveTorch(const glm::vec3& pos, float threshold) {
    auto it = std::remove_if(torches.begin(), torches.end(), [&](const TorchInstance& t) {
        return glm::distance(t.position, pos) < threshold;
    });
    torches.erase(it, torches.end());
}

void RemoveLastTorch() {
    if (!torches.empty()) torches.pop_back();
}
