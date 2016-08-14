#ifndef CBASEEDGE_H
#define CBASEEDGE_H

#include <iostream>

class CBaseEdge {
private:
       std::size_t be_source;
       std::size_t be_destination;
public:
       CBaseEdge(const std::size_t sourceId, const std::size_t destId);
       void setSourceVertice(const std::size_t sourceId);
       std::size_t getSourceVertice() const;
       void setDestinationVertice(const std::size_t destId);
       std::size_t getDestinationVertice() const;
};

#endif // CBASEEDGE_H
