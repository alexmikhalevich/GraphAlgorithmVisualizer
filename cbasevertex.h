#include <iostream>

class CBaseVertex {
private:
       std::size_t bv_vertexIndex;
public:
       CBaseVertex() {}
       void setIndex(std::size_t index);
       std::size_t getIndex() const;
};
