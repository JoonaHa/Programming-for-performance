#include <iostream>
#include <fstream>
#include <cstring>

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

    filename.append(".vb");
    writeF.open(filename, std::ios::out | std::ios::binary);

    if (writeF.fail())
        std::cerr << "Error: " << strerror(errno);

    uint64_t buffer;
    uint8_t out;
    uint8_t sum;
    int i=0;
    while (readF.read(reinterpret_cast<char *>(&buffer), sizeof(buffer)))
    {

        
        while (true)
        {
            out = buffer % 128;
            if (buffer < 128)
            {
                sum = 128 + out;
                writeF.write(reinterpret_cast<char *>(&sum), sizeof(sum));
                break;
            }
            else
            {
                buffer = buffer / 128;
                writeF.write(reinterpret_cast<char *>(&out), sizeof(out));
            }
        }
    i++;
    }

    readF.close();
    writeF.close();
}
