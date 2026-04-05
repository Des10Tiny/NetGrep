#include "shared_stats.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

std::vector<std::string> load_patterns(const std::string& config_path) {
    std::vector<std::string> patterns;
    std::ifstream file(config_path);

    if (!file.is_open()) {
        throw std::runtime_error("Statistics Error: Cannot open config file");
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            patterns.push_back(line);
        }
    }

    return patterns;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config_file>\n";
        return EXIT_FAILURE;
    }

    std::string config_path = argv[1];

    try {
        std::vector<std::string> patterns = load_patterns(config_path);
        SharedStats stats(patterns.size(), false);

        std::cout << "\n---------- CURRENT SERVER STATISTICS ----------\n";
        std::cout << "Total files checked: " << stats.get_files_checked() << "\n";

        for (size_t i = 0; i < patterns.size(); ++i) {
            size_t count = stats.get_threat_count(i);
            if (count > 0) {
                std::cout << "Threat '" << patterns[i] << "' found " << count << " times\n";
            }
        }

        std::cout << "-----------------------------------------------\n\n";

    } catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}