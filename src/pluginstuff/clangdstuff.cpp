#include <boost/process.hpp>
#include <nlohmann/json.hpp>
#include "clangdstuff.h"

boost::process::child clangdProc;
boost::process::ipstream clangdOutput;

// Function to start Clangd process
void startClangd() {
    clangdProc = boost::process::child("clangd", boost::process::std_out > clangdOutput);
    // Continuously read output in a separate thread
    std::thread clangdThread([&]() {
        std::string line;
        while (std::getline(clangdOutput, line)) {
            nlohmann::json jsonOutput = nlohmann::json::parse(line);
            // Handle syntax errors and diagnostics
            handleDiagnostics(jsonOutput);
        }
    });
    clangdThread.detach();
}

void handleDiagnostics(const nlohmann::json& diagnostics) {
    // Parse diagnostics JSON
    for (const auto& diag : diagnostics["diagnostics"]) {
        int line = diag["range"]["start"]["line"];
        int col = diag["range"]["start"]["character"];
        std::string errorMessage = diag["message"];
        
        // Store diagnostics for hover tooltips
        storeErrorAtPosition(line, col, errorMessage);
    }
}

void showTooltip(int line, int col) {
    std::string errorMessage = getErrorAtPosition(line, col);
    if (!errorMessage.empty()) {
        SendMessage(nppHandle, SCI_CALLTIPSHOW, (WPARAM)(line + 1), (LPARAM)errorMessage.c_str());
    }
}

// Hook into Scintilla to capture hover events
void onHover(int position) {
    int line = SendMessage(nppHandle, SCI_LINEFROMPOSITION, position, 0);
    int col = SendMessage(nppHandle, SCI_GETCOLUMN, position, 0);
    showTooltip(line, col);
}

void openProject() {
    TCHAR folderPath[MAX_PATH];
    BROWSEINFO bi = { 0 };
    bi.lpszTitle = TEXT("Pick a Project Folder");
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

    if (pidl != 0) {
        SHGetPathFromIDList(pidl, folderPath);
        // Now folderPath contains the project folder. Use this for Clangd's `compile_commands.json`.
        setupClangdForProject(folderPath);
    }
}

void setupClangdForProject(LPCTSTR projectPath) {
    clangdProc = boost::process::child("clangd", "-p", projectPath, boost::process::std_out > clangdOutput);
}



