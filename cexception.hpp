#include <iostream>
#include <string>

class IException {
public:
       virtual std::string what() = 0;
};

class ExInvalidInput : IException {
public:
       std::string what() { return "Input data is invalid."; }
};

class ExLastStep : IException {
public:
       std::string what() { return "This is already the last step."; }
};

class ExFirstStep : IException {
public:
       std::string what() { return "This is already the first step."; }
};

class ExExistingVertex : IException {
public:
       std::string what() { return "Vertex with this id is already exists."; }
};

class ExExistingEdge : IException {
public:
       std::string what() { return "Edge with this id is already exists."; }
};

class ExInvalidId : IException {
public:
       std::string what() { return "Invalid id."; }
};

class ExInvalidVertex : IException {
public:
       std::string what() { return "No such vertex."; }
};
