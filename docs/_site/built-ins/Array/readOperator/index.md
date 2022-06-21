# Array.readOperator

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `...` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*Array*/` 
</dd>
</dl>

### Description

Static read operator (`[1, "A"]`) constructs new Array.
Not to be confused with method `Array.classPrototype.readOperator`.

### Example

```js
let arr = [36.6, "Harry", true];
console.write(arr[1]);
```

**Expected output**

```
Harry
```

