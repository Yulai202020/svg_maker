#include <yaml-cpp/yaml.h>
#include <fmt/core.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <format>
#include <map>
#include <iterator>

std::string join(const std::vector<std::string>& vec, const std::string& separator) {
    std::ostringstream result;
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<std::string>(result, separator.c_str()));
    
    // Convert the stream to a string and remove the last separator
    std::string joinedString = result.str();
    if (!joinedString.empty()) {
        joinedString.erase(joinedString.length() - separator.length());
    }

    return joinedString;
}


template <typename... Args>
void format_tags(std::vector<std::string>& tags, const std::string fmto, Args... args) {
    std::string tag = fmt::format(fmto, args...);
    tags.push_back(tag);
}


int main(int argc, char** argv) {
    if (argc != 3) {
        return 1;
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
            int cx = std::stoi(value["cx"]);
            int cy = std::stoi(value["cy"]);
            int radius = std::stoi(value["r"]);

            std::string format = "<circle cx=\"{}\" cy=\"{}\" r=\"{}\" style=\"{}\"/>";
            format_tags(tags, format, cx, cy, radius, style);
        } else if (key == "ellipse") {
            int cx = std::stoi(value["cx"]);
            int cy = std::stoi(value["cy"]);
            int radius_x = std::stoi(value["rx"]);
            int radius_y = std::stoi(value["ry"]);

            std::string format = "<ellipse cx=\"{}\" cy=\"{}\" rx=\"{}\" ry=\"{}\" style=\"{}\"/>";
            format_tags(tags, format, cx, cy, radius_x, radius_y, style);
        } else if (key == "line") {
            int x1 = std::stoi(value["x1"]);
            int y1 = std::stoi(value["y1"]);

            int x2 = std::stoi(value["x2"]);
            int y2 = std::stoi(value["y2"]);

            std::string format = "<ellipse x1=\"{}\" y1=\"{}\" x2=\"{}\" y2=\"{}\" style=\"{}\"/>";
            format_tags(tags, format, x1, y1, x2, y2, style);
        } else if (key == "polygon") {
            std::vector<std::string> points = config["key"]["points"].as<std::vector<std::string>>();
            std::string joined = join(points, " ");

            std::string format = "<polygon points=\"{}\" style=\"{}\"/>";
            format_tags(tags, format, joined, style);
        } else if (key == "polyline") {
            std::vector<std::string> points = config["key"]["points"].as<std::vector<std::string>>();
            std::string joined = join(points, " ");

            std::string format = "<polyline points=\"{}\" style=\"{}\"/>";
            format_tags(tags, format, joined, style);
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