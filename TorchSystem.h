#include <vector>
#include <glm.hpp>

// Estructura para almacenar la posición de una antorcha
struct TorchInstance {
    glm::vec3 position;
};

extern std::vector<TorchInstance> torches;

void AddTorch(const glm::vec3& pos);
void RemoveTorch(const glm::vec3& pos, float threshold = 1.0f);
void RemoveLastTorch();
