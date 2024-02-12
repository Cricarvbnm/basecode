#pragma once

#include <array>
#include <optional>

namespace basecode::_ {

template <int set_len> class EncodingSet {
  const std::array<char, set_len> set;

public:
  EncodingSet(const std::array<char, set_len> &set,
              std::optional<char> pad = std::nullopt)
      : set(set), pad(pad) {}

  int find(char chr) const noexcept {
    for (int i = 0; i < set_len; ++i)
      if (set[i] == chr)
        return i;

    return -1;
  }

  const std::optional<char> pad;

  char operator[](size_t i) const noexcept { return set[i]; }
  char at(size_t i) const { return set.at(i); }
};

} // namespace basecode::_