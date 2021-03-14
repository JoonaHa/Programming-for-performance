#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

int main(int argc, char *argv[])
{
//Check argument
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s number\n", argv[0]);

        exit(EXIT_FAILURE);
    }
//Open files with ifstream and ofstream
    std::ofstream writeF;

    writeF.open("test.bin", std::ios::out | std::ios::binary);

    if (writeF.fail())
        std::cerr << "Error: " << strerror(errno);

    uint64_t a = 0;
    a = std::strtoull(argv[1], nullptr, 0);
    std::cout << a << "\n";
    std::cout << sizeof(a) << "\n";
    std::cout << sizeof(reinterpret_cast<char *>(&a)) << "\n";
    std::cout << strlen(reinterpret_cast<char *>(&a)) << "\n";
    //writeF << a;
    writeF.write((char*)&a, sizeof(a));

    writeF.close();
}