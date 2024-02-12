#pragma once

#include "encoding_id.hpp"
#include "encoding_info.hpp"
#include "encoding_set.hpp"

#include <array>

namespace basecode::_ {

constexpr std::array<EncodingInfo, 5> info_list{4, 5, 6, 6, 6};

constexpr EncodingInfo getEncodingInfo(EncodingID id) noexcept {
  return info_list[static_cast<size_t>(id)];
}

template <EncodingID id> struct Encoding {
  static constexpr EncodingInfo info = getEncodingInfo(id);

  static EncodingSet<info.set_len> getInitialSet() noexcept;

  static const EncodingSet<info.set_len> set;
};
template <EncodingID id>
const EncodingSet<Encoding<id>::info.set_len> Encoding<id>::set =
    getInitialSet();

// ---

template <>
inline EncodingSet<getEncodingInfo(EncodingID::base16).set_len>
Encoding<EncodingID::base16>::getInitialSet() noexcept {
  constexpr auto set_len = getEncodingInfo(EncodingID::base16).set_len;
  std::array<char, set_len> result_set;

  int i = 0;
  for (char chr = '0'; chr <= '9'; ++chr, ++i)
    result_set[i] = chr;

  for (char chr = 'A'; chr <= 'F'; ++chr, ++i)
    result_set[i] = chr;

  return EncodingSet<info.set_len>(result_set);
}

template <>
inline EncodingSet<getEncodingInfo(EncodingID::base32).set_len>
Encoding<EncodingID::base32>::getInitialSet() noexcept {
  constexpr auto set_len = getEncodingInfo(EncodingID::base32).set_len;
  std::array<char, set_len> result_set;

  int i = 0;
  for (char chr = 'A'; chr <= 'Z'; ++chr, ++i)
    result_set[i] = chr;

  for (char chr = '2'; chr <= '7'; ++chr, ++i)
    result_set[i] = chr;

  return EncodingSet<info.set_len>(result_set, '=');
}

template <>
inline EncodingSet<getEncodingInfo(EncodingID::base64).set_len>
Encoding<EncodingID::base64>::getInitialSet() noexcept {
  constexpr auto set_len = getEncodingInfo(EncodingID::base64).set_len;
  std::array<char, set_len> result_set;

  int i = 0;
  for (char chr = 'A'; chr <= 'Z'; ++chr, ++i)
    result_set[i] = chr;

  for (char chr = 'a'; chr <= 'z'; ++chr, ++i)
    result_set[i] = chr;

  for (char chr = '0'; chr <= '9'; ++chr, ++i)
    result_set[i] = chr;

  result_set[i++] = '+';
  result_set[i++] = '/';

  return EncodingSet<info.set_len>(result_set, '=');
}

template <>
inline EncodingSet<getEncodingInfo(EncodingID::base64_url_no_pad).set_len>
Encoding<EncodingID::base64_url_no_pad>::getInitialSet() noexcept {
  constexpr auto set_len =
      getEncodingInfo(EncodingID::base64_url_no_pad).set_len;
  std::array<char, set_len> result_set;

  int i = 0;
  for (char chr = 'A'; chr <= 'Z'; ++chr, ++i)
    result_set[i] = chr;

  for (char chr = 'a'; chr <= 'z'; ++chr, ++i)
    result_set[i] = chr;

  for (char chr = '0'; chr <= '9'; ++chr, ++i)
    result_set[i] = chr;

  result_set[i++] = '-';
  result_set[i++] = '_';

  return EncodingSet<info.set_len>(result_set);
}

template <>
inline EncodingSet<getEncodingInfo(EncodingID::base64_url).set_len>
Encoding<EncodingID::base64_url>::getInitialSet() noexcept {
  constexpr auto set_len = getEncodingInfo(EncodingID::base64_url).set_len;
  std::array<char, set_len> result_set;

  int i = 0;
  for (char chr = 'A'; chr <= 'Z'; ++chr, ++i)
    result_set[i] = chr;

  for (char chr = 'a'; chr <= 'z'; ++chr, ++i)
    result_set[i] = chr;

  for (char chr = '0'; chr <= '9'; ++chr, ++i)
    result_set[i] = chr;

  result_set[i++] = '-';
  result_set[i++] = '_';

  return EncodingSet<info.set_len>(result_set, '.');
}

} // namespace basecode::_