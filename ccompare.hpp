#ifndef CCOMPARE_HPP
#define CCOMPARE_HPP

template<typename Value, typename CompareFunction>
class CCompare
{
public:
       CCompare(CompareFunction l) : lessFunction(l) {}
       CompareFunction& getLessFunction() {
              return lessFunction;
       }

       bool greater(Value x, Value y) {
              return lessFunction(y, x);
       }

       bool greaterEqual(Value x, Value y) {
              return !lessFunction(x, y);
       }

       bool less(Value x, Value y) {
              return lessFunction(x, y);
       }

       bool lessEqual(Value x, Value y) {
              return !lessFunction(y, x);
       }

private:
       CompareFunction lessFunction;
};

#endif // CCOMPARE_HPP
