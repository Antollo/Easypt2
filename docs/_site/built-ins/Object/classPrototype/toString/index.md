# Object.classPrototype.toString

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*String*/` 
</dd>
</dl>

### Description

Converts object to number:
- from `Number` (serializes the number to string)
- from `Array` (serializes the array to string)
- from `Boolean` (serializes the boolean to string)
- from other types (if not default `toString` method 
  exists, calls it, otherwise serializes the object 
  to JSON)

### Example

```js
console.write(12.1e1.toString(), "
", { x: 1 }.toString());
```

**Expected output**

```
121
{
    "x": 1
}
```

