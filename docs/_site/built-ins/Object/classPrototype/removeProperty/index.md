# Object.classPrototype.removeProperty

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `key /*String*/` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `this` 
</dd>

</dl>

### Description

Removes the property from the object.

### Example

```js
let obj;
obj.addProperty("first", 1);
obj.addProperty("second", "a");
obj.removeProperty("second");
if (obj.hasOwnProperty("second"))
    console.write(obj.second);
else
    console.write(obj.first);
```

**Expected output**

```
1
```

