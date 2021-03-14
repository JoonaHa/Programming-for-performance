#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>

int main(int argc, char *argv[])
{

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

    filename.append(".sorted.vb");
    writeF.open(filename, std::ios::out | std::ios::binary);

    if (writeF.fail())
        std::cerr << "Error: " << strerror(errno);

    uint64_t buffer;
    std::vector<uint64_t> integers;

    while (readF.read(reinterpret_cast<char *>(&buffer), sizeof(buffer)))
    {
        integers.push_back(buffer);
    }

    //close file
    readF.close();

    //Use default sort to sort the integers
    std::sort(integers.begin(), integers.end());

    //Write as vbytes to file
    uint8_t out;
    uint8_t sum;
    uint64_t value;
    int index = 0;
    for (size_t i = 0; (i + 1) < integers.size(); i++)
    {
        value = abs((integers[i] - integers[i + 1]));

        while (true)
        {
            out = value % 128;
            if (value < 128)
            {
                sum = 128 + out;
                writeF.write(reinterpret_cast<char *>(&sum), sizeof(sum));

                break;
            }
            else
            {
                value = value / 128;
                writeF.write(reinterpret_cast<char *>(&out), sizeof(out));
            }
        }
        index++;
    }

    writeF.close();
}
