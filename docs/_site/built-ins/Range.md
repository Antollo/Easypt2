# Range

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Class`]({{site.baseurl}}\built-ins\Class) 
</dd>
<dt> Inherits from </dt><dd markdown="1">
 [`Object`]({{site.baseurl}}\built-ins\Object) 
</dd>
</dl>

### Description

Represents an immutable range that has start end, and step. 
The default value of step is 1, it can be floating-point 
number. Range operator `..` (e.g. `0..10`, `-0.1..-0.2..-0.01`) 
is the recommended way of creating the range.

### Example

```js
for (let i in 0..10)
    console.write(i, ", ")
console.newLine()

for (let i in 0..10..2)
    console.write(i, ", ")
console.newLine()

for (let i in 0.5..-0.51..-0.25)
    console.write(i, ", ")
```

**Expected output**

```
0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
0, 2, 4, 6, 8, 
0.500000, 0.250000, 0, -0.250000, -0.500000, 
```

### Members

- [`constructor`]({{site.baseurl}}\built-ins\Range\classPrototype\constructor\index)
- [`iterator`]({{site.baseurl}}\built-ins\Range\classPrototype\iterator\index)
- [`length`]({{site.baseurl}}\built-ins\Range\classPrototype\length\index)
- [`readOperator`]({{site.baseurl}}\built-ins\Range\classPrototype\readOperator\index)
- [`start`]({{site.baseurl}}\built-ins\Range\classPrototype\start\index)
- [`step`]({{site.baseurl}}\built-ins\Range\classPrototype\step\index)
- [`stop`]({{site.baseurl}}\built-ins\Range\classPrototype\stop\index)


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


