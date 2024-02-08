#include "basecode.hpp"

#include <iostream>

int main(int argc, char **argv) {
  if (argc < 2) {
    return 1;
  }

  std::cout << basecode::base64::encode(argv[1]);
  return 0;
}