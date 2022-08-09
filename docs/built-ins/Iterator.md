# Iterator

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Class`]({{site.baseurl}}\built-ins\Class) 
</dd>
<dt> Inherits from </dt><dd markdown="1">
 [`Object`]({{site.baseurl}}\built-ins\Object) 
</dd>
</dl>

### Description

Iterator is an object that can iterate over a sequence. 
To satisfy iterator requirements object should implement 
`next` method that either returns next element in the sequence 
or `Iterator.end`. Object does not need to extend `Iterator` 
to be an iterator. To satisfy iterable requirements object 
should implement `iterator` method that returns an iterator.

### Example

```js
for (let i in [1, 2, 3, 4])
    console.write(i)
console.newLine()

for (let i in "abcd")
    console.write(i)
console.newLine()

let it <- [1, 2, 3, 4].iterator()
while (!(let i <- it.next()) is Iterator.end)
    console.write(i)
console.newLine()

class MyRangeIterator extends Iterator {
    constructor: function(i, end) {
        this.i = i - 1
        this.end = end
    },
    next: function() {
        return this.i++ < this.end ? this.i : Iterator.end
    }
}

class MyRange {
    constructor: function(start, end) {
        this.start = start
        this.end = end
    },
    iterator: function() {
        return MyRangeIterator(this.start, this.end)
    }
}

for (let i in MyRange(11, 15))
    console.write(i, " ")
```

**Expected output**

```
1234
abcd
1234
11 12 13 14 
```

### Members

- [`constructor`]({{site.baseurl}}\built-ins\Iterator\classPrototype\constructor\index)
- [`next`]({{site.baseurl}}\built-ins\Iterator\classPrototype\next\index)


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
- [`end`]({{site.baseurl}}\built-ins\Iterator\end\index)


