#pragma once

#include <bitset>
#include <string>

namespace basecode {
using CodeSet = std::string;
} // namespace basecode

namespace basecode::base64 {
using Code = std::string;
extern const CodeSet base_set;

Code encode(const std::string &text);
std::string decode(const Code &code);
} // namespace basecode::base64

namespace basecode::detail {
using Bits = std::string;
}

namespace basecode::base64::detail {
constexpr int single_size = 6;
constexpr int single_count = 4;
constexpr int byte_count = 3;
constexpr int group_size = 24;
constexpr int set_len = 64;

CodeSet initSet() noexcept;

Code groupToCode(basecode::detail::Bits &group);
std::string groupToText(basecode::detail::Bits &group);
} // namespace basecode::base64::detail

// ---

namespace basecode::base64 {
inline const CodeSet base_set = detail::initSet();

inline Code encode(const std::string &text) {

  using std::string;
  auto toBits = [](char c) { return std::bitset<8>(c).to_string(); };

  string code;
  basecode::detail::Bits group;

  int count = 0;
  for (auto c : text) {
    group += toBits(c);
    ++count;

    if (count == detail::byte_count) {
      code += detail::groupToCode(group);
      group.clear();
      count = 0;
    }
  }

  if (count != 0) {
    code += detail::groupToCode(group);
  }

  return code;
}

inline std::string decode(const Code &code) {
  using std::string;
  string text;
  basecode::detail::Bits group;
  auto toBits = [](int single_code) {
    return std::bitset<6>(single_code).to_string();
  };

  int count = 0;
  for (auto c : code) {
    auto single_code = base_set.find(c);

    if (single_code != detail::set_len) {
      group += toBits(single_code);
      ++count;
    } else {
      break;
    }

    if (count == detail::single_count) {
      text += detail::groupToText(group);
      count = 0;
      group.clear();
    }
  }

  if (count != 0) {
    text += detail::groupToText(group);
  }

  return text;
}
} // namespace basecode::base64

namespace basecode::base64::detail {
inline CodeSet initSet() noexcept {
  CodeSet rtn;

  for (char c = 'A'; c <= 'Z'; ++c)
    rtn += c;

  for (char c = 'a'; c <= 'z'; ++c)
    rtn += c;

  for (char c = '0'; c <= '9'; ++c)
    rtn += c;

  rtn += '+';
  rtn += '/';
  rtn += '=';

  return rtn;
}

inline Code groupToCode(basecode::detail::Bits &group) {
  using std::string;

  string code;

  auto group_lack = single_size - (group.size() % single_size);
  if (group_lack != single_size)
    for (; group_lack > 0; --group_lack) {
      group += '0';
    }

  for (int i = 0; i < group.size(); i += single_size) {
    string single = group.substr(i, single_size);
    auto single_code = std::stoul(single, nullptr, 2);
    code += base_set.at(single_code);
  }

  for (auto padding_count = single_count - code.size(); padding_count > 0;
       --padding_count) {
    code += base_set.at(set_len);
  }

  return code;
}

inline std::string groupToText(basecode::detail::Bits &group) {
  std::string text;

  for (int i = 0; i + 8 <= group.size(); i += 8) {
    auto byte = group.substr(i, 8);
    char chr = std::stoul(byte, nullptr, 2);
    text += chr;
  }

  return text;
}
} // namespace basecode::base64::detail
