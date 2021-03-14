#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>
#include <iterator>
#include <chrono>
#include <random>

class BitArray
{
private:
    u_int64_t *array = NULL;
    u_int64_t array_length = 0;

public:
    uint64_t length = 0;

    uint64_t get(uint64_t index)
    {
        return (uint64_t)((array[(int)(index / 64)]) >> (index % 64) & 1);
    }

    void set(uint64_t index, uint64_t bit)
    {
        int i = (int)(index / 64);
        array[i] = (uint64_t)(array[i] | (bit << (index % 64)));
    }

    void toSring()
    {

        for (uint64_t i = 0; i < array_length; i++)
        {
            std::bitset<64> x(array[i]);
            std::cout << x << "\n";
        }
    }

    BitArray(uint64_t size)
    {
        array_length = (int)(size / 64) + 1;
        length = size;
        array = new uint64_t[array_length]();
    }
};

int main(int argc, char *argv[])
{
    int size;
    int rcount;
    std::vector<uint64_t> rnumbers;
    std::default_random_engine rd;

    if (argc != 3 || (size = std::atol(argv[1])) == 0 || (rcount = std::atol(argv[2])) == 0)
    {
        fprintf(stderr, "usage: %s array_size random_size\n", argv[0]);

        exit(EXIT_FAILURE);
    }

    BitArray bitArray = BitArray(size);
    std::uniform_int_distribution<int> dist(0, size);

    /*Generate rnd numbers and push them to vecto r*/

    for (uint64_t i = 0; i < rcount; i++)
    {
        rnumbers.push_back(dist(rd));
    }

    //Set benchmark

    std::chrono::time_point<std::chrono::high_resolution_clock> start, stop;

    start = std::chrono::high_resolution_clock::now();
    for (auto &&i : rnumbers)
    {
        bitArray.set(i, 1);
    }
    stop = std::chrono::high_resolution_clock::now();

    //Get benchmark
    auto time1 = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();

    start = std::chrono::high_resolution_clock::now();
    for (auto &&i : rnumbers)
    {
        bitArray.get(i);
    }
    stop = std::chrono::high_resolution_clock::now();
    auto time2 = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();

    //Second get
    rnumbers.clear();
    for (size_t i = 0; i < rcount; i++)
    {
        rnumbers.push_back(dist(rd));
    }

    start = std::chrono::high_resolution_clock::now();
    for (auto &&i : rnumbers)
    {
        bitArray.get(i);
    }

    stop = std::chrono::high_resolution_clock::now();

    auto time3 = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();

    std::cout << "Set: " << time1 << " ns\n";
    std::cout << "Get: " << time2 << " ns\n";
    std::cout << "Second Get: " << time3 << " ns\n";
}