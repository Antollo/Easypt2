# Object.classPrototype.readOperator

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

Read operator (string[key]) returns property.

### Example

```js
let obj;
obj.number = 21;
obj["secondNumber"] = 22;
console.write(obj["number"], " ", obj.secondNumber);
```

**Expected output**

```
21 22
```

