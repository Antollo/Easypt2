---
sort: 4
---

# Type conversion

### Implicit conversion

- To `Number`
    - from `String` (parses the string as number)
    - from `Array` (returns length of the array)
    - from `Boolean` (returns _1_ if true and _0_ otherwise)
    - from other types (if not default `toNumber` method exists, calls it, otherwise returns the number of properties not including prototype property)

- To `String`
    - from `Number` (serializes the number to string)
    - from `Array` (serializes the array to string)
    - from `Boolean` (serializes the boolean to string)
    - from other types (if not default `toString` method exists, calls it, otherwise serializes the object to JSON)

- To `Array`
    - from `Number` (creates one element array containing this object)
    - from `String` (creates one element array containing this object)
    - from `Boolean` (creates one element array containing this object)
    - from other types (if not default `toArray` method exists, calls it, otherwise creates one element array containing this object)

- To `Boolean`
    - from `Number` (returns _false_ if number is equal to 0 and _true_ otherwise)
    - from `String` (returns _false_ if string is empty and _true_ otherwise)
    - from `Array` (returns _false_ if array is empty and _true_ otherwise)
    - from other types (if not default `toBoolean` method exists, calls it, otherwise returns false if the number of properties not including prototype property is equal to 0 and _true_ otherwise)

### Explicit conversion

To make make type _`Abc`_ convertible to _`Xyz`_ _`Abc.toXyz`_ method should be defined.

There are `Object.toNumber`, `Object.toString`, `Object.toArray` and `Object.toBoolean` default 
conversion methods that works in the same way as implicit conversion.



