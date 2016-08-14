#include "cbaseedge.h"

CBaseEdge::CBaseEdge(const std::size_t sourceId, const std::size_t destId) {
       be_source = sourceId;
       be_destination = destId;
}

void CBaseEdge::setSourceVertice(const std::size_t sourceId) {
       be_source = sourceId;
}

void CBaseEdge::setDestinationVertice(const std::size_t destId) {
       be_destination = destId;
}

std::size_t CBaseEdge::getDestinationVertice() const {
       return be_destination;
}

std::size_t CBaseEdge::getSourceVertice() const {
       return be_source;
}
