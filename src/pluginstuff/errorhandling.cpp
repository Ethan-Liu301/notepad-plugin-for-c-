#include <map>
#include <nlohmann/json.hpp>
#include "errorhandling.h"


void handleDiagnostics(const nlohmann::json& diagnostics) {
    for (const auto& diag : diagnostics["diagnostics"]) {
        int line = diag["range"]["start"]["line"];
        int col = diag["range"]["start"]["character"];
        std::string message = diag["message"];

        // Store the error message in a map indexed by line and column
        errorMap[{line, col}] = message;
    }
}

// Function to retrieve error at a given position
std::string getErrorAtPosition(int line, int col) {
    auto it = errorMap.find({line, col});
    if (it != errorMap.end()) {
        return it->second;
    }
    return "";  // No error at this position
}