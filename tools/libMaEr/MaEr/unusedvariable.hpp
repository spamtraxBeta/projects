#ifndef UNUSEDVARIABLE_HPP
#define UNUSEDVARIABLE_HPP

namespace MaEr
{
    template<class T>
    /**
     * @brief unused_variable suppresses compiler warnings about unused variables
     * @param first parameter: unused variable
     * @param second parameter: reason why variable is not used
     */
    void unused_variable(const T &, const char *)
    {}
}

#endif // UNUSEDVARIABLE_HPP

