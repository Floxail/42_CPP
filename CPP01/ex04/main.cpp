#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

static std::string replaceAll(const std::string& content, const std::string& s1, const std::string& s2) {
    std::string result;
    size_t      pos = 0;
    size_t      found;

    while ((found = content.find(s1, pos)) != std::string::npos) {
        result += content.substr(pos, found - pos) + s2;
        pos = found + s1.length();
    }
    return result + content.substr(pos);
}

int main(int ac, char **av) {
    if (ac != 4) {
        std::cerr << "Usage: ./replace filename s1 s2" << std::endl;
        return 1;
    }
    std::string filename = av[1];
    std::string s1 = av[2];
    if (s1.empty()) {
        std::cerr << "Error: s1 must not be empty" << std::endl;
        return 1;
    }

    std::ifstream infile(filename.c_str());
    if (!infile) {
        std::cerr << "Error: cannot open " << filename << std::endl;
        return 1;
    }
    std::stringstream buffer;
    buffer << infile.rdbuf();

    std::ofstream outfile((filename + ".replace").c_str());
    if (!outfile) {
        std::cerr << "Error: cannot create " << filename << ".replace" << std::endl;
        return 1;
    }
    outfile << replaceAll(buffer.str(), s1, av[3]);
    return 0;
}
