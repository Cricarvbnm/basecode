#pragma once

#include <array>

namespace basecode::_ {

template <int set_len> class EncodingSet {
  const std::array<char, set_len> set;

public:
  EncodingSet(const std::array<char, set_len> &set) : set(set) {}

  int find(char chr) const noexcept {
    for (int i = 0; i < set_len; ++i)
      if (set[i] == chr)
        return i;

    return -1;
  }
};

} // namespace basecode::_::_