# Object.classPrototype.hasOwnProperty

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `key /*String*/` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*Boolean*/` 
</dd>
</dl>

### Description

Checks whether the object has own property.

### Example

```js
let obj;
obj.addProperty("first", 1);
console.write(obj.hasOwnProperty("first"), " ", obj.hasOwnProperty("second"));
```

**Expected output**

```
true false
```

