#include "TorchSystem.h"
#include <algorithm>

std::vector<ModelInstance> modelInstances;
std::string currentModelPath = "";
glm::vec3 currentScale = glm::vec3(1.0f, 1.0f, 1.0f);

void SetCurrentModel(const std::string& modelPath) {
    currentModelPath = modelPath;
}

void AddModelInstance(const glm::vec3& pos) {
    modelInstances.push_back({pos, currentScale});
}

void RemoveModelInstance(const glm::vec3& pos, float threshold) {
    auto it = std::remove_if(modelInstances.begin(), modelInstances.end(), [&](const ModelInstance& m) {
        return glm::distance(m.position, pos) < threshold;
    });
    modelInstances.erase(it, modelInstances.end());
}

void RemoveLastModelInstance() {
    if (!modelInstances.empty()) {
        modelInstances.pop_back();
        printf("Modelo eliminado. Modelos restantes: %zu\n", modelInstances.size());
    } else {
        printf("No hay modelos para eliminar.\n");
    }
}

void ScaleCurrentModel(float delta) {
    currentScale += glm::vec3(delta, delta, delta);
    if (currentScale.x < 0.1f) currentScale = glm::vec3(0.1f, 0.1f, 0.1f);
}

void ScaleLastModel(float delta) {
    if (!modelInstances.empty()) {
        // Escalar el último modelo colocado
        modelInstances.back().scale += glm::vec3(delta, delta, delta);
        // Limitar escala mínima
        if (modelInstances.back().scale.x < 0.1f) {
            modelInstances.back().scale = glm::vec3(0.1f, 0.1f, 0.1f);
        }
        printf("Escala del ultimo modelo: %.2f\n", modelInstances.back().scale.x);
    } else {
        // Si no hay modelos, modificar la escala para el próximo
        ScaleCurrentModel(delta);
        printf("No hay modelos colocados. Escala para el proximo: %.2f\n", currentScale.x);
    }
}

void ExportModelPositions(const char* filename) {
    FILE* file = NULL;
    errno_t err = fopen_s(&file, filename, "w");
    if (err == 0 && file != NULL) {
        fprintf(file, "# Model positions and scales (x, y, z, scale)\n");
        for (const auto& instance : modelInstances) {
            fprintf(file, "%.2f,%.2f,%.2f,%.2f\n", 
                instance.position.x, instance.position.y, instance.position.z, instance.scale.x);
        }
        fclose(file);
        printf("Coordenadas exportadas a %s\n", filename);
    } else {
        printf("Error: No se pudo crear el archivo %s\n", filename);
    }
}
