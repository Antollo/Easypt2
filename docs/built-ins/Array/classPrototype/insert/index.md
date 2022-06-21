# Array.classPrototype.insert

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `n /*Number*/, element` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `this` 
</dd>

</dl>

### Description

Inserts new element before n-th element.

### Example

```js
let arr = [36.6, "Harry", true];
arr.insert(1, "Dirty ");
console.write(arr[1], arr[2]);
```

**Expected output**

```
Dirty Harry
```

