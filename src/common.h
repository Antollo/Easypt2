
#ifndef COMMON_H_
#define COMMON_H_

#include <functional>
#include "nobject.h"
#include "assert.h"

template <class... Args>
bool all(Args... args) { return (... && args); }

template <class... Args, std::size_t... Is>
void argsGuardHelper(const object::type::Array &arr, std::index_sequence<Is...>)
{
	if (!all((
			std::is_same_v<nullptr_t, Args> ||
			arr[Is]->isOfType<Args>())...))
		throw std::runtime_error("wrong type of argument");
}

template <class... Args>
void argsGuard(const object::type::Array &arr)
{
	if (arr.size() < sizeof...(Args))
		throw std::runtime_error("wrong number of arguments");
	argsGuardHelper<Args...>(arr, std::index_sequence_for<Args...>());
}

template <class... Args, std::size_t... Is>
void argsConvertibleGuardHelper(const object::type::Array &arr, std::index_sequence<Is...>)
{
	if (!all((
			std::is_same_v<nullptr_t, Args> ||
			arr[Is]->isConvertible<Args>())...))
		throw std::runtime_error("wrong type of argument");
}

template <class... Args>
void argsConvertibleGuard(const object::type::Array &arr)
{
	if (arr.size() < sizeof...(Args))
		throw std::runtime_error("wrong number of arguments");
	argsConvertibleGuardHelper<Args...>(arr, std::index_sequence_for<Args...>());
}

#endif /* !COMMON_H_ */
