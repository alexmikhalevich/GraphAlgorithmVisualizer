#include <iostream>

class IException {
public:
       virtual void what() = 0;
};

class ExInvalidInput : IException {
public:
       void what() {
              std::cerr << "Input data is invalid" << std::endl;
       }
};
