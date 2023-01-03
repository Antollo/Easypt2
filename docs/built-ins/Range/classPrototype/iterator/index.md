# Range.classPrototype.iterator

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*Iterator*/` 
</dd>
</dl>

### Description

Returns an iterator that iterates over the elements the of the range.

### Example

```js
let it = (0..10).iterator()
while (!((let x <- it.next()) is Iterator.end))
    console.write(x, " ")
```

**Expected output**

```
0 1 2 3 4 5 6 7 8 9 
```

