# Array.classPrototype.forEach

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `callback /*Function*/` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `this` 
</dd>

</dl>

### Description

Executes a callback for each array element. Arguments of callback are:
element, index of element, array.

### Example

```js
let arr = [36.6, "Harry", true];
arr.forEach(function {
    console.write(args[0], " is element with index ", args[1], ", ");
});
```

**Expected output**

```
36.600000 is element with index 0, Harry is element with index 1, true is element with index 2, 
```

