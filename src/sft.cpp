#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>
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

bool is_dir(char *path)
{
    const auto dir_path = path;
    fs::directory_entry dir { path };
    return dir.exists() && dir.is_directory();
}

int write_file(const char *filename, char *buff, size_t len)
{
    const auto path = fs::current_path() / filename;
    fs::directory_entry file { path };
    std::fstream wfile (filename, std::fstream::out | std::fstream::trunc);
    wfile.write(buff, len);

    return 0;
}

int read_file(const char *filename, char **buff)
{
    std::fstream src_fp (filename, std::fstream::in);
    if (src_fp.fail())
    { 
        std::cerr << "Error opening source file" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    int len = file_len(src_fp);
    *buff = new char[len];
    src_fp.read(*buff, len);
    return len;
}

int copy_file(char *src, char *dest)
{ 
    if (is_dir(src))
    {
        std::cerr << "Source file cannot be a directory" << src << std::endl;
        std::exit(EXIT_FAILURE);
    }
    std::string str_src { src };
    std::size_t found = str_src.find_last_of("/");

    int buff_len = 0;
    char *buff;
    if (found == std::string::npos)
    {
        buff_len = read_file(src, &buff);
    }
    else
    {
        std::string str_dir = str_src.substr(0, found);
        str_src = str_src.substr(found+1);
        const auto current_path = fs::current_path();
        fs::current_path(str_dir.c_str());
        buff_len = read_file(str_src.c_str(), &buff);
        fs::current_path(current_path);
    }

    if (is_dir(dest))
    {
        const auto current_path = fs::current_path();
        fs::current_path(dest);
        write_file(src, buff, buff_len);
        fs::current_path(current_path);
    }
    else
    {
        std::string str_dest { dest };
        found = str_dest.find_last_of("/");
        if (found == std::string::npos)
        {
            write_file(dest, buff, buff_len);
        }
        else
        {
            std::string str_dir = str_dest.substr(0, found);
            str_dest = str_dest.substr(found+1);
            const auto current_path = fs::current_path();
            fs::current_path(str_dir.c_str());
            write_file(str_dest.c_str(), buff, buff_len);
            fs::current_path(current_path);
        }
    }
    return 0;
}

int get_input(int argc, char *argv[])
{
    int err = 0;
    if (argc <= 2)
    {
        std::cerr << "Need to specify files or a directory with -r" << std::endl;
        return 1;
    }
    else if (argc == 3)
    {
        return copy_file(argv[1], argv[2]);
    }
    else 
    {
        for (int i = 1; i < argc-1; ++i)
        {
            if (!is_dir(argv[argc-1]))
            {
                std::cerr << "Destination must be a directory when using multiple source files" << std::endl;
                std::exit(EXIT_FAILURE);
            }
            copy_file(argv[i], argv[argc-1]);
        }
        return 0;
    }
    

}

int main(int argc, char *argv[])
{
    get_input(argc, argv);
    return 0;
}