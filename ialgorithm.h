#ifndef IALGORITHM
#define IALGORITHM

#include <vector>
#include <istream>

template<class CompareFunction>
class IAlgorithm {
public:
       virtual void apply_algorithm(const std::vector<std::vector<int> >& edges,
                                   const std::size_t edges_size, CompareFunction func, std::istream& stream) = 0;
};

#endif // IALGORITHM

