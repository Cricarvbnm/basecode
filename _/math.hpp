#pragma once

namespace basecode::_::math {

constexpr int leastCommonMultiple(int a, int b) noexcept {
    int max = 0, min = 0;
    if (a > b) {
        max = a;
        min = b;
    } else {
        max = b;
        min = a;
    }

    int result = max;
    for (; result % min != 0; result += max)
        ;
    return result;
}

constexpr int pow2(int x) noexcept {
    int result = 1;
    for (; x > 0; --x) { result *= 2; }
    return result;
}

} // namespace basecode::_::math