#include <example/example.hpp>

namespace example {

uint64_t gcd(uint64_t a, uint64_t b) {
  while (b != 0) {
    uint64_t t = b;
    b = a % b;
    a = t;
  }
  return a;
}

} // end of namespace gcd

