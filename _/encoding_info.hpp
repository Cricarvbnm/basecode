#pragma once

#include "math.hpp"

namespace basecode::_ {

struct EncodingInfo {
  const int single_size;
  const int byte_size;
  const int group_size;

  const int single_number;
  const int byte_number;

  const int set_len;

  constexpr EncodingInfo(int single_size)
      : single_size(single_size), byte_size(8),
        group_size(math::leastCommonMultiple(single_size, byte_size)),
        single_number(group_size / single_size),
        byte_number(group_size / byte_size), set_len(math::pow2(single_size)) {}
};

} // namespace basecode::_::_