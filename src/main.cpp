
#include <cstdlib>
#include <fstream>
#include <iostream>

#include <vole/exception.hpp>
#include <vole/datamodel.hpp>
#include <vole/datamodel_parsers.hpp>
#include <vole/node_printer.hpp>

/**
 * Read a file from the filesystem and return it as a string
 * @param path The path to the file
 * @return A std::string containing the contents of the file
 */
std::string load_file(std::filesystem::path path) {
    std::ifstream file(path, std::ios::in);
    if (file.is_open() == false) {
        throw std::runtime_error("Failed to open file");
    }
    std::istreambuf_iterator<char> fitr{file}, end;
    return std::string(fitr, end);
}

void load_and_display_model(std::string_view filename) {
    vole::datamodel::json_parser parser;
    auto model = parser.parse(load_file(filename));
    vole::node_printer renderer;
    std::cout << renderer.render(*model) << std::endl;
}

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file>" << std::endl;
        return EXIT_FAILURE;
    }

    try {
        load_and_display_model(argv[0]);
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}