
#include <iostream>
#include <cstdlib>

#include <vole/exception.hpp>
#include <vole/datamodel.hpp>
#include <vole/datamodel_parsers.hpp>
#include <vole/node_printer.hpp>

void load_and_display_model(std::string_view filename) {
    vole::datamodel::json_parser parser;
    auto model = parser.parse_file(filename);
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