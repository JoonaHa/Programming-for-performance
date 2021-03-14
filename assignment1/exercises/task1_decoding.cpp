#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>

int main(int argc, char *argv[])
{

    //Check argument
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s file\n", argv[0]);

        exit(EXIT_FAILURE);
    }
    //Open files with ifstream and ofstream
    std::ifstream readF;
    readF.open(argv[1], std::ios::in | std::ios::binary);

    if (readF.fail())
        std::cerr << "Error: " << strerror(errno);

    std::ofstream writeF;
    std::string filename = argv[1];

    filename.append(".dec");
    writeF.open(filename, std::ios::out | std::ios::binary);

    if (writeF.fail())
        std::cerr << "Error: " << strerror(errno);

    uint8_t buffer;
    uint64_t out = 0;
    int n = 0;
    int index = 0;
    while (readF.read(reinterpret_cast<char *>(&buffer), sizeof(buffer)))
    {

        if (buffer > 128)
        {
            out += (buffer - 128) * std::pow(128, n);
            writeF.write(reinterpret_cast<char *>(&out), sizeof(out));
            n = 0;
            out = 0;
            index++;
        }
        else
        {
            out += buffer * std::pow(128, n);
            n++;
        }
    }

    readF.close();
    writeF.close();
}
