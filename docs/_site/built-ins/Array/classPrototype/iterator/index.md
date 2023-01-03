# Array.classPrototype.iterator

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*Iterator*/` 
</dd>
</dl>

### Description

Returns an iterator that iterates over the elements the of array.

### Example

```js
let arr = [36.6, "Harry", true];
let it = arr.iterator()
while (!((let x <- it.next()) is Iterator.end))
    console.write(x, " ");
```

**Expected output**

```
36.600000 Harry true 
```

