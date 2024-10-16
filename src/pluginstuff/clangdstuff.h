#pragma once
#include <boost/process.hpp>
#include <nlohmann/json.hpp>


// Function to start Clangd process
void startClangd();

void handleDiagnostics(const nlohmann::json& diagnostics);

void showTooltip(int line, int col);

// Hook into Scintilla to capture hover events
void onHover(int position);

void openProject();

