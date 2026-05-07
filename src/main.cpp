#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
    /*

    std::cout << "csv-analyzer says hello\n";
    std::cout << __cplusplus << std::endl;
    return 0;
    */
    std::ifstream file{"data/TSLA.csv"};
    std::string strInput{};

    if (!file)
    {
        // Print an error and exit
        std::cerr << "Uh oh, TSLA.csv could not be opened for reading!\n";
        return 1;
    }

    int i = 0;
    while (i < 3) {
        getline(file, strInput);
        std::cout << strInput << "\n";
        i++;
    }
    return 0;


}