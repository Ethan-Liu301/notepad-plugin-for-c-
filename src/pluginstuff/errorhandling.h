#include <map>
#include <nlohmann/json.hpp>

std::map<std::pair<int, int>, std::string> errorMap;  // Store errors by line and column

void handleDiagnostics(const nlohmann::json& diagnostics);

// Function to retrieve error at a given position
std::string getErrorAtPosition(int line, int col);