#ifndef COMMON_H_
#define COMMON_H_

#include <functional>
#include "nobject.h"

#ifdef assert
#undef assert
#endif


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
	argsConvertibleGuardHelper<Args...>(arr, std::index_sequence_for<Args...>());
}

template <class T>
void assert(const T& a, const std::string& message = "assertion error: ")
{
	if(a)
		return;
	throw std::runtime_error(message + std::to_string(a));
}

template <template<class> class O, class T, class U>
void assert(const T& a, const U& b, const std::string& message = "assertion error: ")
{
	if(O<T>()(a,b))
		return;
	if constexpr(std::is_same_v<O<T>, std::less<T>>)
		throw std::runtime_error(message + std::to_string(a) + " < " + std::to_string(b));
	else if constexpr(std::is_same_v<O<T>, std::less_equal<T>>)
		throw std::runtime_error(message + std::to_string(a) + " <= " + std::to_string(b));
	else if constexpr(std::is_same_v<O<T>, std::greater<T>>)
		throw std::runtime_error(message + std::to_string(a) + " > " + std::to_string(b));
	else if constexpr(std::is_same_v<O<T>, std::greater_equal<T>>)
		throw std::runtime_error(message + std::to_string(a) + " >= " + std::to_string(b));
	else if constexpr(std::is_same_v<O<T>, std::equal_to<T>>)
		throw std::runtime_error(message + std::to_string(a) + " == " + std::to_string(b));
	else if constexpr(std::is_same_v<O<T>, std::not_equal_to<T>>)
		throw std::runtime_error(message + std::to_string(a) + " != " + std::to_string(b));
	else
		throw std::runtime_error(message + std::to_string(a) + " operator " + std::to_string(b));
}

#endif /* !COMMON_H_ */
