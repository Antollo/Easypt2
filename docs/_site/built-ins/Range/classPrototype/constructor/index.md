# Range.classPrototype.constructor

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `start /*Number*/, stop /*Number*/, [step /*Number*/]` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*Range*/` 
</dd>
</dl>

### Description

Creates new range object.

### Example

```js
function printRange(r) {
    console.write(r.start(), " ", r.stop(), " ", r.step()).newLine()
}

printRange(0..5)
printRange(5..0)
printRange(5..0..-2)

printRange(Range(0, 5))
printRange(Range(5, 0))
printRange(Range(5, 0,-2))
```

**Expected output**

```
0 5 1
5 0 -1
5 0 -2
0 5 1
5 0 -1
5 0 -2

```

