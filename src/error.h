#ifndef ERROR_H_
#define ERROR_H_

#include <stdexcept>

class os_error : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

#endif /* !ERROR_H_ */
