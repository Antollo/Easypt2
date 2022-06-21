# Array.classPrototype.erase

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `position /*Number*/, [length = array.length() /*Number*/]` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `this` 
</dd>

</dl>

### Description

Erases elements from range [position, position + length).

### Example

```js
let arr = [1, 2, 3];
arr.erase(1, 1);
console.write(arr[0], arr[1]);
```

**Expected output**

```
13
```

