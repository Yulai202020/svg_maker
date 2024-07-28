#include <yaml-cpp/yaml.h>
#include <fmt/core.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <iterator>

std::string join(const std::vector<std::string>& vec, const std::string& separator) {
    std::ostringstream result;
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<std::string>(result, separator.c_str()));

    std::string joinedString = result.str();
    if (!joinedString.empty()) {
        joinedString.erase(joinedString.length() - separator.length());
    }

    return joinedString;
}

template <typename... Args>
void format_tags(std::vector<std::string>& tags, const std::string& fmto, Args... args) {
    std::string tag = fmt::format(fmto, args...);
    tags.push_back(tag);
}

void getAllKeys(const YAML::Node& node, std::vector<std::string>& keys, const std::string& prefix = "") {
    if (node.IsMap()) {
        for (const auto& kv : node) {
            std::string new_prefix = prefix.empty() ? kv.first.as<std::string>() : prefix + "." + kv.first.as<std::string>();
            keys.push_back(new_prefix);
            getAllKeys(kv.second, keys, new_prefix);
        }
    }
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Usage " << argv[0] << " <config_yaml> <output_html>\n";
        return 1;
    }

    std::string config_file = argv[1];
    std::string output_file = argv[2];

    YAML::Node config = YAML::LoadFile(config_file);

    std::vector<std::string> tags;
    std::vector<std::string> keys;
    getAllKeys(config, keys);

    for (const std::string& key : keys) {
        std::string style;
        if (config[key]["style"]) {
            style = config[key]["style"].as<std::string>();
        }

        if (key == "rect") {
            int x = config[key]["x"].as<int>();
            int y = config[key]["y"].as<int>();
            int height = config[key]["height"].as<int>();
            int width = config[key]["width"].as<int>();

            std::string format = "<rect x=\"{}\" y=\"{}\" height=\"{}\" width=\"{}\" style=\"{}\"/>";
            format_tags(tags, format, x, y, height, width, style);
        } else if (key == "circle") {
            int cx = config[key]["cx"].as<int>();
            int cy = config[key]["cy"].as<int>();
            int radius = config[key]["r"].as<int>();

            std::string format = "<circle cx=\"{}\" cy=\"{}\" r=\"{}\" style=\"{}\"/>";
            format_tags(tags, format, cx, cy, radius, style);
        } else if (key == "ellipse") {
            int cx = config[key]["cx"].as<int>();
            int cy = config[key]["cy"].as<int>();
            int radius_x = config[key]["rx"].as<int>();
            int radius_y = config[key]["ry"].as<int>();

            std::string format = "<ellipse cx=\"{}\" cy=\"{}\" rx=\"{}\" ry=\"{}\" style=\"{}\"/>";
            format_tags(tags, format, cx, cy, radius_x, radius_y, style);
        } else if (key == "line") {
            int x1 = config[key]["x1"].as<int>();
            int y1 = config[key]["y1"].as<int>();
            int x2 = config[key]["x2"].as<int>();
            int y2 = config[key]["y2"].as<int>();

            std::string format = "<line x1=\"{}\" y1=\"{}\" x2=\"{}\" y2=\"{}\" style=\"{}\"/>";
            format_tags(tags, format, x1, y1, x2, y2, style);
        } else if (key == "polygon") {
            std::vector<std::string> points = config[key]["points"].as<std::vector<std::string>>();
            std::string joined = join(points, " ");

            std::string format = "<polygon points=\"{}\" style=\"{}\"/>";
            format_tags(tags, format, joined, style);
        } else if (key == "polyline") {
            std::cout << "a";
            std::vector<std::string> points = config[key]["points"].as<std::vector<std::string>>();
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
        result += "\t\t" + tag;
    }

    std::string format = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n\t<meta charset=\"UTF-8\">\n\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\t<title>Svg image</title>\n</head>\n<body>\n\t<svg>\n{}\n\t</svg>\n</body>\n</html>";
    std::string final_result = fmt::format(format, result);

    std::ofstream outfile(output_file);
    if (outfile.is_open()) {
        outfile << final_result;
        outfile.close();
    } else {
        std::cerr << "Unable to open file for writing\n";
    }

    return 0;
}

