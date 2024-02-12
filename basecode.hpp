#pragma once

#include "_/encoding.hpp"

#include <algorithm>
#include <bitset>
#include <iterator>
#include <string>
#include <type_traits>

namespace basecode::_ {

template <typename Iterator>
using getIteratorCategory =
    typename std::iterator_traits<Iterator>::iterator_category;

template <EncodingID id> class BaseCode {
  static const Encoding<id> encoding;

public:
  template <typename InputIterator, typename OutputIterator,
            typename = std::enable_if_t<
                std::is_base_of_v<std::input_iterator_tag,
                                  getIteratorCategory<InputIterator>> &&
                std::is_base_of_v<std::output_iterator_tag,
                                  getIteratorCategory<OutputIterator>>>>
  static void encode(InputIterator text_it, InputIterator text_end,
                     OutputIterator code_it);
  template <typename InputIterator, typename OutputIterator,
            typename = std::enable_if_t<
                std::is_base_of_v<std::input_iterator_tag,
                                  getIteratorCategory<InputIterator>> &&
                std::is_base_of_v<std::output_iterator_tag,
                                  getIteratorCategory<OutputIterator>>>>
  static void decode(InputIterator code_it, InputIterator code_end,
                     OutputIterator text_it);

private:
  template <typename OutputIterator,
            typename = std::enable_if_t<std::is_base_of_v<
                std::output_iterator_tag, getIteratorCategory<OutputIterator>>>>
  static void groupToCode(const std::string &group, OutputIterator code_it);

  static std::string textToBits(const std::string &bits);

  static void padBits(std::string &group);

  template <typename OutputIterator,
            typename = std::enable_if_t<std::is_base_of_v<
                std::output_iterator_tag, getIteratorCategory<OutputIterator>>>>
  static void groupToText(const std::string &group, OutputIterator text_it);

  static std::string codeToBits(const std::string &group);
};

// ---

template <EncodingID id> const Encoding<id> BaseCode<id>::encoding;

template <EncodingID id>
template <typename InputIterator, typename OutputIterator, typename>
void BaseCode<id>::encode(InputIterator text_it, InputIterator text_end,
                          OutputIterator code_it) {
  std::string group;

  int byte_count = 0;
  for (char text_chr; text_it != text_end; ++text_it) {
    text_chr = *text_it;
    group.push_back(text_chr);
    ++byte_count;

    if (byte_count == encoding.info.byte_number) {
      groupToCode(group, code_it);
      group.clear();
      byte_count = 0;
    }
  }

  if (byte_count != 0) {
    groupToCode(group, code_it);
  }
}

template <EncodingID id>
template <typename OutputIterator, typename>
void BaseCode<id>::groupToCode(const std::string &group,
                               OutputIterator code_it) {
  std::string bits = textToBits(group);
  padBits(bits);

  int i = 0;
  for (; i < bits.size(); i += encoding.info.single_size) {
    auto single_bits = bits.substr(i, encoding.info.single_size);
    unsigned single_code = std::stoul(single_bits, nullptr, 2);
    char base_chr = encoding.set[single_code];
    *code_it++ = base_chr;
  }

  if (encoding.set.pad) {
    auto pad_number =
        (encoding.info.group_size - i) / encoding.info.single_size;
    std::for_each_n(code_it, pad_number,
                    [](auto &c) { c = *encoding.set.pad; });
  }
}

template <EncodingID id> void BaseCode<id>::padBits(std::string &group) {
  auto lack_number =
      (encoding.info.single_size - (group.size() % encoding.info.single_size)) %
      encoding.info.single_size;
  group.append(lack_number, '0');
}

template <EncodingID id>
std::string BaseCode<id>::textToBits(const std::string &bits) {
  std::string result;
  for (auto chr : bits) {
    result += std::bitset<encoding.info.byte_size>(chr).to_string();
  }
  return result;
}

template <EncodingID id>
template <typename InputIterator, typename OutputIterator, typename>
void BaseCode<id>::decode(InputIterator code_it, InputIterator code_end,
                          OutputIterator text_it) {
  std::string group;

  int single_count = 0;
  for (char base_chr; code_it != code_end; ++code_it) {
    base_chr = *code_it;
    if (encoding.set.pad && base_chr == encoding.set.pad.value())
      break;

    group += base_chr;
    single_count++;

    if (single_count == encoding.info.single_number) {
      groupToText(group, text_it);
      group.clear();
      single_count = 0;
    }
  }

  if (single_count != 0) {
    groupToText(group, text_it);
  }
}

template <EncodingID id>
template <typename OutputIterator, typename>
void BaseCode<id>::groupToText(const std::string &group,
                               OutputIterator text_it) {
  auto bits = codeToBits(group);
  for (int i = 0; i + encoding.info.byte_size <= bits.size();
       i += encoding.info.byte_size) {
    auto byte_bits = bits.substr(i, encoding.info.byte_size);
    char text_chr = std::stoul(byte_bits, nullptr, 2);
    *text_it++ = text_chr;
  }
}

template <EncodingID id>
std::string BaseCode<id>::codeToBits(const std::string &group) {
  std::string result;
  for (auto base_chr : group) {
    auto single_code = encoding.set.find(base_chr);
    auto single_bits =
        std::bitset<encoding.info.single_size>(single_code).to_string();
    result += single_bits;
  }
  return result;
}

} // namespace basecode::_

namespace basecode {

using _::EncodingID;
template <EncodingID id> using BaseCode = _::BaseCode<id>;

} // namespace basecode
