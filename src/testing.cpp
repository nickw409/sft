#include <string>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Need to specify file or a directory with -r" << std::endl;
        return 1;
    }
    std::string path = argv[1];
    for (const auto &entry : fs::directory_iterator(path))
    {
        std::cout << entry.path() << std::endl;
    }
    return 0;
}
