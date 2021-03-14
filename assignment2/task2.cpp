#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>
#include <iterator>
#include <chrono>
#include <random>
#include <cmath>
class BitArray
{
private:
    uint64_t *array = NULL;
    uint64_t array_length = 0;
    uint64_t *sum_array = NULL;
    uint64_t block_size = 0;

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

    void init_sum_array()
    {
        sum_array = new uint64_t[array_length]();
        uint64_t last_sum = 0;
        for (uint64_t i = block_size; i < length; i += block_size)
        {
            uint64_t index = (int)(i / 64);
            uint64_t temp = array[index];
            uint64_t value = __builtin_popcountl(temp >> (i % 64));
            value += last_sum;
            last_sum = value;
            sum_array[index] = sum_array[index] | (value << (i % 64));
        }
    }

    uint64_t sum(const uint64_t index)
    {

        if (sum_array == NULL)
            this->init_sum_array();

        uint64_t i = (int)(index / 64);
        uint64_t sum_position = std::round(index / block_size);
        uint64_t value = ((1 << (block_size)) - 1) & (sum_array[(int)(sum_position / 64)] >> (sum_position % 64));

        if (index == sum_position)
            return value;

        uint64_t rest = 0;

        if (sum_position > index)
            rest = value - __builtin_popcountl(((1 << (sum_position - index)) - 1) & (array[i] >> (index % 64)));

        if (sum_position < index)
            rest = value + __builtin_popcountl(((1 << (index - sum_position)) - 1) & (array[i] >> (sum_position % 64)));
        return rest;
    }

    void toSring()
    {

        for (uint64_t i = 0; i < array_length; i++)
        {
            std::bitset<64> x(array[i]);
            std::cout << x << "\n";
        }
    }

    BitArray(uint64_t size, uint64_t block_size)
    {
        array_length = (int)(size / 64) + 1;
        length = size;
        array = new uint64_t[array_length]();
        this->block_size = block_size;
    }
};

int main(int argc, char *argv[])
{
    int size;
    int rcount;
    int block_size;
    std::vector<uint64_t> rnumbers;
    std::default_random_engine rd;

    if (argc != 4 || (size = std::atol(argv[1])) == 0 || (rcount = std::atol(argv[2])) == 0 || (block_size = std::atol(argv[3])) == 0)
    {
        fprintf(stderr, "usage: %s array_size random_size block_size\n", argv[0]);

        exit(EXIT_FAILURE);
    }

    BitArray bitArray = BitArray(size, block_size);
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
        //std::cout << i << "\n";
        //bitArray.toSring();
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


    bitArray.init_sum_array();
    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < bitArray.length; i++)
    {

        uint64_t sum = bitArray.sum(i);
        //std::cout << "Index: " << i << "\n";
        //std::cout << "Sum: "<< sum << "\n";
    }

    stop = std::chrono::high_resolution_clock::now();

    auto time4 = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();

    std::cout << "Set: " << time1 << " ns\n";
    std::cout << "Get: " << time2 << " ns\n";
    std::cout << "Second Get: " << time3 << " ns\n";
    std::cout << "Sum: " << time4 << " ns\n";
}