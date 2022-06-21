# Object.classPrototype.toArray

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*Array*/` 
</dd>
</dl>

### Description

Converts object to number:
- from `Number` (creates one element array containing 
  this object)
- from `String` (creates one element array containing 
  this object)
- from `Boolean` (creates one element array containing 
  this object)
- from other types (if not default `toArray` method 
  exists, calls it, otherwise creates one element array 
  containing this object)

### Example

```js
console.write("abc".toArray(), " ", true.toArray());
```

**Expected output**

```
["abc"] [true]
```

