# basecode

## Usage

Include basecode.hpp. Use the static methods in
`basecode::BaseCode<EncodingID::base64>`,
or `basecode::encode<EncodingID::base64>` and
`basecode::decode<EncodingID::base64>`.

The parameter of `encode` and `decode` are all input iterators.

## Example

``` cpp
#include <basecode/basecode.hpp>

#include <iostream>
#include <iterator>
#include <sstream>

int main() {
    using std::cin;
    using std::cout;
    using std::stringstream;

    auto cin_it  = std::istreambuf_iterator<char>(cin);
    auto cin_end = std::istreambuf_iterator<char>();
    auto cout_it = std::ostreambuf_iterator<char>(cout);

    auto input_str     = stringstream("Hello, wol");
    auto input_str_it  = std::istreambuf_iterator<char>(input_str);
    auto input_str_end = std::istreambuf_iterator<char>();

    auto output_str    = stringstream();
    auto output_str_it = std::ostreambuf_iterator<char>(output_str);

    // Encoding
    cout << "Encode: ";
    basecode::encode<basecode::EncodingID::base64>(cin_it, cin_end, cout_it);
    basecode::BaseCode<basecode::EncodingID::base64>::encode(
        input_str_it, input_str_end, output_str_it);
    cout << output_str.str() << '\n';

    // Decoding
    cout << "Decode: ";
    basecode::BaseCode<basecode::EncodingID::base64>::decode(cin_it, cin_end,
                                                             cout_it);
    basecode::decode<basecode::EncodingID::base64>(
        std::istreambuf_iterator<char>(output_str),
        std::istreambuf_iterator<char>(), cout_it);
    cout << '\n';
}
```

Input:

- `Hello^Z` # ^Z: Ctrl+Z, like Ctrl+D in linux
- `SGVsbG8K^Z`

Output:

- `SGVsbG8K`
- `SGVsbG8sIHdvbA==`
- `Hello`
- `Hello, wol`

True interaction:

Encode: `Hello`

SGVsbG8K `^Z`

SGVsbG8sIHdvbA==

Decode: `SGVsbG8K`

Hello

`^Z`

Hello, wol

## Files

`basecode.hpp`: The head you need to include.

`_`: The details of the code.
