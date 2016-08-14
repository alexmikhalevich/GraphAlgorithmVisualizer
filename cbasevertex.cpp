#include "cbasevertex.h"

void CBaseVertex::setIndex(std::size_t index) {
       bv_vertexIndex = index;
}

std::size_t CBaseVertex::getIndex() const {
       return bv_vertexIndex;
}
