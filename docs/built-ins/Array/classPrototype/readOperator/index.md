# Array.classPrototype.readOperator

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `position /*Number*/` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*Object*/` 
</dd>
</dl>

### Description

Read operator (`arrayInstance[position]`) returns element at given position. 
Negative indicies can be used, and count from the end of the array.
Not to be confused with static method `Array.readOperator`.

### Example

```js
let arr = [36.6, "Harry", true];
console.write(arr[1]);
```

**Expected output**

```
Harry
```

