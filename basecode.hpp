#pragma once

#include <bitset>
#include <string>

namespace basecode {

template <int single_size, int id = 0> class BaseCode {
private:
  static constexpr int __leastCommonMultiple(int a, int b);
  static constexpr int __pow2(int x);
  static std::string __initialBaseSet() noexcept;

  static constexpr int __byte_size = 8;
  static constexpr int __group_size =
      __leastCommonMultiple(single_size, __byte_size);
  static constexpr int __single_count = __group_size / single_size;
  static constexpr int __byte_count = __group_size / __byte_size;
  static constexpr int __set_len = __pow2(single_size);

private:
  using __Bits = std::string;

public:
  using CodeSet = std::string;
  using Code = std::string;

  static const CodeSet base_set;

public:
  static Code encode(const std::string &text);
  static std::string decode(const Code &);

private:
  static constexpr bool __needPadding();
  static Code __groupToCode(__Bits &group);
  static std::string __groupToText(__Bits &group);

  // ---

public:
  ~BaseCode() = delete;
};

using Base64 = BaseCode<6>;
template <> inline auto Base64::__initialBaseSet() noexcept -> Code {
  CodeSet result;

  for (char c = 'A'; c <= 'Z'; ++c)
    result += c;

  for (char c = 'a'; c <= 'z'; ++c)
    result += c;

  for (char c = '0'; c <= '9'; ++c)
    result += c;

  result += '+';
  result += '/';
  result += '=';

  return result;
}

using Base64URL = BaseCode<6, 1>;
template <> inline auto Base64URL::__initialBaseSet() noexcept -> Code {
  CodeSet result;

  for (char c = 'A'; c <= 'Z'; ++c)
    result += c;

  for (char c = 'a'; c <= 'z'; ++c)
    result += c;

  for (char c = '0'; c <= '9'; ++c)
    result += c;

  result += '-';
  result += '_';

  return result;
}

using Base32 = BaseCode<5>;
template <> inline auto Base32::__initialBaseSet() noexcept -> Code {
  CodeSet result;

  for (char c = 'A'; c <= 'Z'; ++c)
    result += c;

  for (char c = '2'; c <= '7'; ++c)
    result += c;

  result += '=';

  return result;
}

using Base16 = BaseCode<4>;
template <> inline auto Base16::__initialBaseSet() noexcept -> Code {
  CodeSet result;

  for (char c = '0'; c <= '9'; ++c)
    result += c;

  for (char c = 'A'; c <= 'F'; ++c)
    result += c;

  return result;
}

template <int single_size, int id>
inline const typename BaseCode<single_size, id>::CodeSet
    BaseCode<single_size, id>::base_set = __initialBaseSet();

// ---

template <int single_size, int id>
inline constexpr int BaseCode<single_size, id>::__pow2(int x) {
  int result = 1;
  for (; x > 0; --x) {
    result *= 2;
  }
  return result;
}

template <int single_size, int id>
inline constexpr int BaseCode<single_size, id>::__leastCommonMultiple(int a,
                                                                      int b) {
  int max = a > b ? a : b;

  int result = 0;
  for (result = max; result % a != 0 || result % b != 0; result += max)
    ;

  return result;
}

template <int single_size, int id>
inline auto BaseCode<single_size, id>::encode(const std::string &text) -> Code {

  using std::string;
  auto toBits = [](char c) { return std::bitset<__byte_size>(c).to_string(); };

  string code;
  __Bits group;

  int count = 0;
  for (auto c : text) {
    group += toBits(c);
    ++count;

    if (count == __byte_count) {
      code += __groupToCode(group);
      group.clear();
      count = 0;
    }
  }

  if (count != 0) {
    code += __groupToCode(group);
  }

  return code;
}

template <int single_size, int id>
inline std::string BaseCode<single_size, id>::decode(const Code &code) {
  using std::string;
  string text;
  BaseCode::__Bits group;
  auto toBits = [](int single_code) {
    return std::bitset<single_size>(single_code).to_string();
  };

  int count = 0;
  for (auto c : code) {
    auto single_code = base_set.find(c);

    if (single_code != __set_len) {
      group += toBits(single_code);
      ++count;
    } else {
      break;
    }

    if (count == __single_count) {
      text += __groupToText(group);
      count = 0;
      group.clear();
    }
  }

  if (count != 0) {
    text += __groupToText(group);
  }

  return text;
}

template <int single_size, int id>
inline auto BaseCode<single_size, id>::__groupToCode(__Bits &group) -> Code {
  using std::string;

  string code;

  auto group_lack = single_size - (group.size() % single_size);
  if (group_lack != single_size && group_lack != single_size)
    for (; group_lack > 0; --group_lack) {
      group += '0';
    }

  for (int i = 0; i < group.size(); i += single_size) {
    string single = group.substr(i, single_size);
    auto single_code = std::stoul(single, nullptr, 2);
    code += base_set.operator[](single_code);
  }

  if (__needPadding())
    for (auto padding_count = __single_count - code.size(); padding_count > 0;
         --padding_count) {
      code += base_set.operator[](__set_len);
    }

  return code;
}

template <int single_size, int id>
inline std::string BaseCode<single_size, id>::__groupToText(__Bits &group) {
  std::string text;

  for (int i = 0; i + 8 <= group.size(); i += 8) {
    auto byte = group.substr(i, 8);
    char chr = std::stoul(byte, nullptr, 2);
    text += chr;
  }

  return text;
}

template <int single_size, int id>
constexpr bool BaseCode<single_size, id>::__needPadding() {
  if constexpr (id > 0) {
    return true;
  } else {
    return false;
  }
}

} // namespace basecode