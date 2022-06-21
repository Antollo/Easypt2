# Array.classPrototype.append

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `arr /*Array*/, [position = 0 /*Number*/, length = arr.length() /*Number*/]` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `this` 
</dd>

</dl>

### Description

Appends `arr` to this array (or optionally 
subarray starting from given position 
and of given length).

### Example

```js
let arr = [1, 2, 3];
let arr2 = [4, 5, 6];
arr.append(arr2);
console.write(arr);
```

**Expected output**

```
[1, 2, 3, 4, 5, 6]
```

