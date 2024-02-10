#include <iostream>
#include <vector>

template <typename T>
T findMax(const T array[], int size) {
    if (size <= 0) {
        std::cerr << "Error: Array size must be greater than 0." << std::endl;
        exit(EXIT_FAILURE);
    }

    T maxElement = array[0];

    for (int i = 1; i < size; ++i) {
        if (array[i] > maxElement) {
            maxElement = array[i];
        }
    }

    return maxElement;
}

template <typename T>
std::vector<T> getUserInputArray(int size) {
    std::vector<T> userArray;

    std::cout << "Enter " << size << " elements for the array:" << std::endl;
    for (int i = 0; i < size; ++i) {
        T element;
        std::cout << "Element " << i + 1 << ": ";
        std::cin >> element;
        userArray.push_back(element);
    }

    return userArray;
}

int main() {
    int size;
    int select;
    bool isOngoing = true;


    while (isOngoing)
    {
        std::cout << "What type of list do you want to know the max off?" << std::endl;
        std::cout << "1) int" << std::endl;
        std::cout << "2) double" << std::endl;
        std::cout << "3) float" << std::endl;
        std::cout << "4) char" << std::endl;
        std::cout << "0) quit" << std::endl;

        std::cin >> select;
        while (!std::cin.good() || select < 0 || select > 4)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Please only a number between 0 and 4!" << std::endl;
            std::cin >> select;
        }

        if (select == 0)
        {
            break;
        }

        std::cout << std::endl;

        // Get input for the array size
        std::cout << "Enter the size of the array: ";
        std::cin >> size;
        while (!std::cin.good())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Please only enter numbers!" << std::endl;
            std::cin >> size;
        }

        if (select == 1)
        {
            // Get input for an array of integers
            auto intArray = getUserInputArray<int>(size);
            int intMax = findMax(intArray.data(), size);
            std::cout << "Max integer: " << intMax << std::endl;
        }
        
        if (select == 2)
        {
            // Get input for an array of doubles
            auto doubleArray = getUserInputArray<double>(size);
            double doubleMax = findMax(doubleArray.data(), size);
            std::cout << "Max double: " << doubleMax << std::endl;
        }
        
        if (select == 3)
        {
            // Get input for an array of floats
            auto floatArray = getUserInputArray<float>(size);
            double floatMax = findMax(floatArray.data(), size);
            std::cout << "Max float: " << floatMax << std::endl;
        }
        
        if (select == 4)
        {
            // Get input for an array of characters
            auto charArray = getUserInputArray<char>(size);
            char charMax = findMax(charArray.data(), size);
            std::cout << "Max char: " << charMax << std::endl;
        }
    }

    return 0;
}
