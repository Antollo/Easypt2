# Range.classPrototype.readOperator

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `i /*Number*/` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*Number*/` 
</dd>
</dl>

### Description

Returns i-th element of the range `start + step * i`.

### Example

```js
let r = 0..10
console.write(r[2]).newLine()
r = 0..10..2
console.write(r[2]).newLine()
r = 0.5..-0.51..-0.25
console.write(r[2])
```

**Expected output**

```
2
4
0
```

