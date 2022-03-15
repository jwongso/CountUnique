// Please submit the complete C++ source code for a program that counts the number of distinct unique words in a file whose name is passed as an argument to a program.
// For example, given the file content "a horse and a dog" the program must output "4" (the word 'a' appears twice but only accounts for one distinct unique occurrence).
// The input text is guaranteed to contain only 'a'..'z' and space characters in ASCII encoding.
// The program should be able to handle large inputs(e.g. 32 GiB)
// You can assume that all unique words fit into memory when using the data structure of your choice.
// The solution must utilize all available CPU resources.

// Enable run time measurement as default
// Comment this out to disable it.
#define ENABLE_TIME_MEASUREMENT

#ifdef ENABLE_TIME_MEASUREMENT
#include <chrono>
#endif
#include <algorithm>
#include <iterator>
#include <fstream>
#include <future>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <sys/stat.h> // Use posix/c stat since it's the fastest
#include <vector>

// Read block 64 kB
// Todo: Put this into an option as parameter?
const std::vector<char>::size_type MAX_BLOCK_SIZE = 65536;


/// Probably fastest file exist function (Linux gcc >= 5.x)
bool FileExists(const std::string& file_path)
{
    
    struct stat buf;
    return (stat(file_path.c_str(), &buf) == 0);
}


/// calculate_unique_words
/// Return the number of unique words
/// Todo: Find out the better way or an ultimate algorithm to find unique words in combination with 
/// reading data from a file in block size.
size_t calculate_unique_words(const std::vector<char>& text)
{
    const std::string s(text.begin(), text.end());
    std::istringstream ss(s);
    // Todo: Perhaps find the fastest way to have distinct unique words?
    const std::set<std::string> words(std::istream_iterator<std::string>(ss), {});
    return text.back() != ' ' && text.size() == MAX_BLOCK_SIZE ? words.size() - 1 : words.size();
}


/// read_async
/// Return a bunch of tasks or futures
/// We could use shared_future in case the future is needed or promised somewhere else
/// shared_future and future are analog to shared_ptr and unique_ptr
std::vector<std::future<size_t>> read_async(std::ifstream& in)
{
    std::vector<std::future<size_t>> tasks;

    while (!in.eof() && !in.fail())
    {
        // Reserve the sizte of vector to speed up the vector operations
        std::vector<char> c(MAX_BLOCK_SIZE);
        // Use reading in block size to avoid blocking and unexpected memory fragmentation
        in.read(c.data(), MAX_BLOCK_SIZE);
        c.resize(static_cast<const unsigned int>(in.gcount()));
        // Schedule async tasks -> The CPU utilization will be managed via std::async internally
        tasks.push_back(std::async([](const std::vector<char>& data)
        {
            // Run the real task
            return calculate_unique_words(data);
        },
        std::move(c)));
    }

    return tasks;
}


/// Main function
int main(int argc, char** argv)
{
    std::string file_path;
    // We have only one parameter, otherwise or with different options, better use getopt()
    if (argc > 1)
    { 
        file_path = argv[1]; 
    }

    // Check whether the file exists
    if (!FileExists(file_path))
    {
        std::cout << "File: " << file_path << " does't exist!" << std::endl;
        return -1;
    }

    unsigned long counter = 0L;
#ifdef ENABLE_TIME_MEASUREMENT
    auto start = std::chrono::high_resolution_clock::now();
#endif
    std::ifstream is(file_path, std::ios::binary);
    auto res = read_async(is);
    // Collect the task results
    std::for_each(res.begin(), res.end(), [&counter](std::future<size_t>& x) 
    { 
        counter += x.get(); 
    });
#ifdef ENABLE_TIME_MEASUREMENT
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    std::cout << "Counter = " << counter << " and Duration: " << duration.count() << " sec." << std::endl;
#endif

    return counter;
}

// End of file