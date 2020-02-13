#ifndef PATTERNS_UTIL_TEXT_HPP
#define PATTERNS_UTIL_TEXT_HPP

#include <cxxabi.h>
#include <typeindex>


const std::string demangle(const char* name)
{
    int status = -4;
    char* res = abi::__cxa_demangle(name, nullptr, nullptr, &status);
    const char* const demangled_name = (status==0) ? res : name;
    std::string ret_val(demangled_name);
    free(res);
    return ret_val;
}

/**
 * Create a string with arguments types separated by ","
 * @tparam Types    Pack of types
 * @param vec       vector where to store arguments type indices
 * @return          A string representing the arguments signature
 */
template <typename... Types>
std::string print_args_types()
{
    std::vector<std::type_index> vec;
    std::ostringstream args_str;
    vec.insert(vec.end(), {typeid(Types)...});
    bool first = true;
    for (auto k : vec){
        if (first) {
            args_str << demangle(k.name());
            first = false;
        }else
            args_str << ", " << demangle(k.name());
    }
    return args_str.str();
}

/**
 * Create a string with arguments values separated by ","
 * @tparam Arg
 * @param out
 * @param arg
 */
template <typename Arg>
std::string print_args_values(Arg&& arg){
    std::ostringstream out;
    (out << arg);
    return out.str();
}
template <typename... Args>
std::string print_args_values(Args&&... args) {
    std::ostringstream out;
    ((out << args << ", "), ...);
    return out.str();
}


#endif //PATTERNS_UTIL_TEXT_HPP
