# Object.classPrototype.addProperty

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `key /*String*/, value /*Object*/` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `key /*String*/` 
</dd>
</dl>

### Description

Adds new property to the object.

### Example

```js
let obj;
obj.addProperty("first", 1);
obj.addProperty("second", "a");
console.write(obj.first, " ", obj.second);
```

**Expected output**

```
1 a
```

