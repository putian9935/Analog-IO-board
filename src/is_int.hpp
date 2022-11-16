#ifndef IS_INT_HPP
#define IS_INT_HPP 

template<typename T> 
struct is_int {
    enum {value = false};
};

template<> 
struct is_int<int> {
    enum {value = true};
};
#endif 