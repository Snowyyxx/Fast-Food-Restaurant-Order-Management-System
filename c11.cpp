#include <iostream>
#include <vector>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};

    // C++11 feature: range-based for loop
    for (auto i : vec) {
        std::cout << i << " ";
    }

    std::cout << std::endl;

    // C++11 feature: lambda expression
    auto add = [](int a, int b) -> int { return a + b; };

    std::cout << "Sum of 3 and 4 is " << add(3, 4) << std::endl;

    return 0;
}
