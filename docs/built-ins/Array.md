# Array

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Class`]({{site.baseurl}}\built-ins\Class) 
</dd>
<dt> Inherits from </dt><dd markdown="1">
 [`Object`]({{site.baseurl}}\built-ins\Object) 
</dd>
</dl>

### Description

Array class provides methods for manipulating arrays (list-like
objects). Array elements are indexed with non-negative integers or
negative integers (counts elemensts from the end of the array).

### Example

```js
let arr = [36.6, "Harry", true];
arr.forEach(function {
    console.write(args[0], " is element with index ", args[1], ", ");
});
```

**Expected output**

```
36.600000 is element with index 0, Harry is element with index 1, true is element with index 2, 
```

### Members

- [`append`]({{site.baseurl}}\built-ins\Array\classPrototype\append\index)
- `constructor`
- [`erase`]({{site.baseurl}}\built-ins\Array\classPrototype\erase\index)
- [`forEach`]({{site.baseurl}}\built-ins\Array\classPrototype\forEach\index)
- [`insert`]({{site.baseurl}}\built-ins\Array\classPrototype\insert\index)
- [`insertFrom`]({{site.baseurl}}\built-ins\Array\classPrototype\insertFrom\index)
- [`join`]({{site.baseurl}}\built-ins\Array\classPrototype\join\index)
- [`length`]({{site.baseurl}}\built-ins\Array\classPrototype\length\index)
- [`readOperator`]({{site.baseurl}}\built-ins\Array\classPrototype\readOperator\index)
- [`subarray`]({{site.baseurl}}\built-ins\Array\classPrototype\subarray\index)


### Inherited members

- `==`
- [`addProperty`]({{site.baseurl}}\built-ins\Object\classPrototype\addProperty\index)
- `classPrototype`
- [`getId`]({{site.baseurl}}\built-ins\Object\classPrototype\getId\index)
- [`getOwnPropertyNames`]({{site.baseurl}}\built-ins\Object\classPrototype\getOwnPropertyNames\index)
- [`hasOwnProperty`]({{site.baseurl}}\built-ins\Object\classPrototype\hasOwnProperty\index)
- [`readOperator`]({{site.baseurl}}\built-ins\Object\classPrototype\readOperator\index)
- [`removeProperty`]({{site.baseurl}}\built-ins\Object\classPrototype\removeProperty\index)
- [`toArray`]({{site.baseurl}}\built-ins\Object\classPrototype\toArray\index)
- [`toBoolean`]({{site.baseurl}}\built-ins\Object\classPrototype\toBoolean\index)
- [`toJson`]({{site.baseurl}}\built-ins\Object\classPrototype\toJson\index)
- [`toNumber`]({{site.baseurl}}\built-ins\Object\classPrototype\toNumber\index)
- [`toString`]({{site.baseurl}}\built-ins\Object\classPrototype\toString\index)


### Properties

- `classPrototype`
- [`readOperator`]({{site.baseurl}}\built-ins\Array\readOperator\index)


