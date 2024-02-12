#pragma once

#include "encoding.hpp"
#include <bitset>
#include <string>

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

private:
  template <typename OutputIterator>
  static void groupToCode(const std::string &group, OutputIterator code_it);

  static std::string toBits(const std::string &bits);

  static void padBits(std::string &group);
};

// ---

template <EncodingID id> const Encoding<id> BaseCode<id>::encoding;

template <EncodingID id>
template <typename InputIterator, typename OutputIterator>
void BaseCode<id>::encode(InputIterator text_it, InputIterator text_end,
                          OutputIterator code_it) {
  std::string group;

  int single_count = 0;
  for (char chr; text_it != text_end; ++text_it) {
    chr = *text_it;
    group.push_back(chr);
    ++single_count;

    if (single_count == encoding.info.byte_number) {
      groupToCode(group, code_it);
      group.clear();
      single_count = 0;
    }
  }

  if (single_count != 0) {
    groupToCode(group, code_it);
  }
}

template <EncodingID id>
template <typename OutputIterator>
void BaseCode<id>::groupToCode(const std::string &group,
                               OutputIterator code_it) {
  std::string bits = toBits(group);
  padBits(bits);

  for (int i = 0; i < bits.size(); i += encoding.info.single_size) {
    auto single_bits = bits.substr(i, encoding.info.single_size);
    unsigned single_code = std::stoul(single_bits, nullptr, 2);
    char base_chr = encoding.set[single_code];
    *code_it++ = base_chr;
  }
}

template <EncodingID id> void BaseCode<id>::padBits(std::string &group) {
  auto lack_number =
      (encoding.info.single_size - (group.size() % encoding.info.single_size)) %
      encoding.info.single_size;
  group.append(lack_number, '0');
}

template <EncodingID id>
std::string BaseCode<id>::toBits(const std::string &bits) {
  std::string result;
  for (auto chr : bits) {
    result += std::bitset<encoding.info.byte_size>(chr).to_string();
  }
  return result;
}

} // namespace basecode::_