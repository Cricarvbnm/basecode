#pragma once

#include "encoding_id.hpp"
#include "encoding_info.hpp"
#include "encoding_set.hpp"

#include <array>

namespace basecode::_ {

constexpr std::array<EncodingInfo, 3> info_list{4, 5, 6};

constexpr EncodingInfo getEncodingInfo(EncodingID id) noexcept {
  return info_list[static_cast<size_t>(id)];
}

template <EncodingID id>
EncodingSet<getEncodingInfo(id).set_len> getInitialSet() noexcept;

template <EncodingID id> struct Encoding {
  static constexpr EncodingInfo info = getEncodingInfo(id);
  static const EncodingSet<info.set_len> set = getInitialSet<id>();
};

} // namespace basecode::_