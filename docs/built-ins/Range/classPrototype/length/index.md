# Range.classPrototype.length

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*Number*/` 
</dd>
</dl>

### Description

Returns number of elements in range `ceil((stop - start) / step)`.

### Example

```js
let r = 0..10
console.write(r.length()).newLine()
r = 0..10..2
console.write(r.length()).newLine()
r = 0.5..-0.51..-0.25
console.write(r.length())
```

**Expected output**

```
10
5
5
```

