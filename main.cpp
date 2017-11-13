
#include <iostream>
#include <base64.h>

int main()
{
    std::string str;
    
    std::cout << "Source string: ";
    std::cin >> str;
    std::string encode_str = B64Encode((unsigned char const *)str.c_str(),
                                                        str.length());
    std::string decode_str = B64Decode((unsigned char const *)encode_str.c_str(),
                                                        encode_str.length());
    std::cout << "Encode: " << encode_str << std::endl;
    std::cout << "Decode: " << decode_str << std::endl;    
   
    return 0;
}
