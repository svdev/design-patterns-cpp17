#ifndef PATTERNS_EXCEPTION_HPP
#define PATTERNS_EXCEPTION_HPP

class design_pattern_exception : public std::exception {
protected:
    std::string msg;
public:
    explicit design_pattern_exception(const std::string& msg) :msg(msg) { };
    virtual const char* what() const throw() {
        return msg.c_str();
    }
};

#endif //PATTERNS_EXCEPTION_HPP
