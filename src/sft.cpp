#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <filesystem>
namespace fs = std::filesystem;

#define MAX_BUFF_LEN 16000

int file_len(std::fstream &file)
{
    int len = 0;
    file.seekg(0, file.end);
    len = file.tellg();
    file.seekg(0, file.beg);
    return len;
}

int copy_file(char *src, char *dest)
{
    std::fstream src_file (src, std::fstream::in);
    if (src_file.fail())
    {
        std::cerr << "Error opening source file" << std::endl;
        return 1;
    }

    std::fstream dest_file (dest, std::fstream::out | std::fstream::trunc);
    if (dest_file.fail())
    {
        std::cerr << "Error opening destination file" << std::endl;
        return 1;
    }

    int buff_len = file_len(src_file);
    buff_len = (buff_len < MAX_BUFF_LEN) ? buff_len : MAX_BUFF_LEN;
    char buff[buff_len];
   
    src_file.read(buff, buff_len);
    dest_file.write(buff, src_file.gcount());

    return 0;
}

int get_input(int argc, char *argv[])
{
    int err = 0;
    if (argc < 2)
    {
        std::cerr << "Need to specify file or a directory with -r" << std::endl;
        return 1;
    }
    if (argc == 3)
    {
        err = copy_file(argv[1], argv[2]);
        if (err != 0)
        {
            return 1;
        }
        return 0;
    }
    return 1;

}

int main(int argc, char *argv[])
{
    get_input(argc, argv);
    return 0;
}