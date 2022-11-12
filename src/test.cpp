#ifdef WINDOWS 
#include <iostream> 
#include <bitset>

static uint32_t convert_8_32(uint32_t num)
{
    std::cout << std::bitset<32>(num) << '\n';

    num = ((num & 0x0000000F) | ((num & 0x000000F0) << 12));
    std::cout << std::bitset<32>(num) << '\n';
    num = ((num & 0x00030003) | ((num & 0x000C000C) << 6));
    std::cout << std::bitset<32>(num) << '\n';

    num = ((num & 0x01010101) | ((num & 0x02020202) << 3));
    std::cout << std::bitset<32>(num) << '\n';

    return num;
}

void decode_32_16(uint32_t num1) {
    std::cout << std::bitset<32>(num1) << '\n';

    num1 = (num1 & 0x11111111) | ((num1 & 0x44444444) >> 1);
    std::cout << std::bitset<32>(num1) << '\n';

    num1 = (num1 & 0x03030303) | ((num1 & 0x30303030) >> 2);
    std::cout << std::bitset<32>(num1) << '\n';

    num1 = (num1 & 0x000F000F) | ((num1 & 0x0F000F00) >> 4);
    std::cout << std::bitset<32>(num1) << '\n';

    num1 = (num1 & 0x000000FF) | ((num1 & 0x00FF0000) >> 8);
    std::cout << std::bitset<32>(num1) << '\n';

}

int main() {
    decode_32_16(0x55555555);
}
#endif 
