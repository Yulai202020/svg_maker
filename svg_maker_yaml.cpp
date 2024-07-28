#include <yaml-cpp/yaml.h>
#include <fmt/core.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <format>
#include <map>

template <typename... Args>
void format_tags(std::vector<std::string>& tags, const std::string fmto, Args... args) {
    std::string tag = fmt::format(fmto, args...);
    tags.push_back(tag);
}


int main(int argc, char** argv) {
    if (argc != 3) {
        return;
    }

    for (int i = 0; i < argc; i++) {
        if (argv[i] == "-o" && i != argc-1) {
            std::string output_file = argv[i+1];
            std::string input_file = (i-1 >= 0) ? argv[i-1] : argv[i+2];
        }
    }

    YAML::Node config = YAML::LoadFile("config.yaml");

    std::map<std::string, std::map<std::string, std::string>> svg = config["svg"].as<std::map<std::string, std::map<std::string, std::string>>>();
    std::vector<std::string> tags;

    for (std::map<std::string, std::map<std::string, std::string>>::iterator it = svg.begin(); it != svg.end(); ++it) {
        std::string key = it->first;
        std::map<std::string, std::string> value = it->second;

        std::string tag;
        std::string style = value["style"];

        if (key == "rect") {
            int x = std::stoi(value["x"]);
            int y = std::stoi(value["y"]);
            int height = std::stoi(value["height"]);
            int width = std::stoi(value["width"]);

            std::string format = "<rect x=\"{}\" y=\"{}\" height=\"{}\" width=\"{}\" style=\"{}\"/>";
            format_tags(tags, format, x, y, height, width, style);
        } else if (key == "circle") {

        }
    }

    std::string result;
    for (const auto& tag : tags) {
        if (!result.empty()) {
            result += "\n";
        }
        result += "\t" + tag;
    }

    std::string format = "<svg>\n{}\n</svg>";
    std::string final_result = fmt::format(format, result);

    std::ofstream outfile("output.svg");
    if (outfile.is_open()) {
        outfile << final_result;
        outfile.close();
    } else {
        std::cerr << "Unable to open file for writing\n";
    }

    return 0;
}