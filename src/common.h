#ifndef COMMON_H_
#define COMMON_H_

#include "nobject.h"

template<class... Args>
bool all(Args... args) { return (... && args); }

template <class... Args, std::size_t... Is>
void argsGuardHelper(const object::arrayType& arr, std::index_sequence<Is...>)
{
	if(!all((std::is_same_v<nullptr_t, Args> || arr[Is]->isOfType<Args>())... ))
    throw std::runtime_error("wrong type of argument");
	
}

template <class... Args>
void argsGuard(const object::arrayType& arr)
{
	if (arr.size() < sizeof...(Args))
        throw std::runtime_error("wrong number of arguments");
	argsGuardHelper<Args...>(arr, std::index_sequence_for<Args...>());
}

template <class... Args, std::size_t... Is>
void argsConvertibleGuardHelper(const object::arrayType& arr, std::index_sequence<Is...>)
{
	if(!all((std::is_same_v<nullptr_t, Args> || arr[Is]->isConvertible<Args>())... ))
    throw std::runtime_error("wrong type of argument");
	
}

template <class... Args>
void argsConvertibleGuard(const object::arrayType& arr)
{
	if (arr.size() < sizeof...(Args))
        throw std::runtime_error("wrong number of arguments");
	argsGuardHelper<Args...>(arr, std::index_sequence_for<Args...>());
}

#endif /* !COMMON_H_ */
