#pragma once

#include "encoding.hpp"

namespace basecode::_ {

template <EncodingID id> class BaseCode {
  static const Encoding<id> encoding;

public:
  template <typename InputIterator, typename OutputIterator>
  static void encode(InputIterator text_it, InputIterator text_end,
                     OutputIterator code_it);
  template <typename InputIterator, typename OutputIterator>
  static void decode(InputIterator code_it, InputIterator code_end,
                     OutputIterator text_it);
};

} // namespace basecode::_