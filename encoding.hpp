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
  static const EncodingSet<info.set_len> set;
};
template <EncodingID id>
const EncodingSet<Encoding<id>::info.set_len> Encoding<id>::set =
    getInitialSet<id>();

// ---

template <>
inline EncodingSet<getEncodingInfo(EncodingID::base16).set_len>
getInitialSet<EncodingID::base16>() noexcept {
  constexpr auto set_len = getEncodingInfo(EncodingID::base16).set_len;
  std::array<char, set_len> result;

  int i = 0;
  for (char chr = '0'; chr <= '9'; ++chr, ++i)
    result[i] = chr;

  for (char chr = 'A'; chr <= 'F'; ++chr, ++i)
    result[i] = chr;

  return result;
}

template <>
inline EncodingSet<getEncodingInfo(EncodingID::base32).set_len>
getInitialSet<EncodingID::base32>() noexcept {
  constexpr auto set_len = getEncodingInfo(EncodingID::base32).set_len;
  std::array<char, set_len> result;

  int i = 0;
  for (char chr = 'A'; chr <= 'Z'; ++chr, ++i)
    result[i] = chr;

  for (char chr = '2'; chr <= '7'; ++chr, ++i)
    result[i] = chr;

  return result;
}

} // namespace basecode::_