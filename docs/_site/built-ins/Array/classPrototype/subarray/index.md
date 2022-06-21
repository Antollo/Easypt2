# Array.classPrototype.subarray

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `position /*Number*/, [length = array.length() /*Number*/]` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*Array*/` 
</dd>
</dl>

### Description

Creates subarray of given length starting at given position.

### Example

```js
let arr = [1, 2, 3];
let subarr = arr.subarray(1, 1);
console.write(subarr[0]);
```

**Expected output**

```
2
```

