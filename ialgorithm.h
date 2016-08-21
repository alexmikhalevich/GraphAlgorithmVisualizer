#ifndef IALGORITHM
#define IALGORITHM

#include <vector>
#include <istream>
#include "cgraph.hpp"

template<class CompareFunction>
class IAlgorithm {
public:
       virtual void apply_algorithm(CGraph* graph) = 0;
       virtual void step() = 0;
       virtual ~IAlgorithm() {}
};

Q_DECLARE_INTERFACE(IAlgorithm, "com.maked0n.GVis.IAlgorithm")

#endif // IALGORITHM

