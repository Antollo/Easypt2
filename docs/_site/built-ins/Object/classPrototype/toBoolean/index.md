# Object.classPrototype.toBoolean

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*Boolean*/` 
</dd>
</dl>

### Description

Converts object to number:
- from `Number` (returns false if number is equal to 
  0 and true otherwise)
- from `String` (returns false if string is empty and 
  true otherwise)
- from `Array` (returns false if array is empty and 
  true otherwise)
- from other types (if not default `toBoolean` method 
  exists, calls it, otherwise returns false if the number 
  of properties not including prototype property is 
  equal to 0 and true otherwise)

### Example

```js
console.write([].toBoolean(), " ", 10.toBoolean());
```

**Expected output**

```
false true
```

