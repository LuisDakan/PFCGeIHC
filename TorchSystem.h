#include <vector>
#include <glm.hpp>
#include <string>

// Estructura para almacenar instancias de modelos
struct ModelInstance {
    glm::vec3 position;
    glm::vec3 scale;
};

extern std::vector<ModelInstance> modelInstances;
extern std::string currentModelPath;
extern glm::vec3 currentScale;

void SetCurrentModel(const std::string& modelPath);
void AddModelInstance(const glm::vec3& pos);
void RemoveModelInstance(const glm::vec3& pos, float threshold = 1.0f);
void RemoveLastModelInstance();
void ScaleCurrentModel(float delta);
void ScaleLastModel(float delta);
void ExportModelPositions(const char* filename);
