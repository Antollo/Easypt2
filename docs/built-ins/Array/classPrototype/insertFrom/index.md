# Array.classPrototype.insertFrom

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `n /*Number*/, array /*Array*/, [position = 0 /*Number*/], [length = array.length() /*Number*/]` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `this` 
</dd>

</dl>

### Description

Inserts to array new elements before n-th element from subarray of second
argument starting at given position and of given length.

### Example

```js
let arr = [36.6, "Harry", true];
arr.insertFrom(3, [1, 2, 3, 4, 5, 6], 2, 4);
console.write(arr[3], arr[4], arr[5], arr[6]);
```

**Expected output**

```
3456
```

