# Timeout.classPrototype.constructor

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `milliseconds /*Number*/` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*Timeout*/` 
</dd>
</dl>

### Description

Creates new `Timeout` object.

### Example

```js
// Wait for 1 ms
Timeout(1).then(() => console.write(1););
console.write(0);
await Timeout(23);
console.write(23);
```

**Expected output**

```
0123
```

