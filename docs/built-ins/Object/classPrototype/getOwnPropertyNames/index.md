# Object.classPrototype.getOwnPropertyNames

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*Array*/` 
</dd>
</dl>

### Description

Returns array of all own properties. Order is not specified.
Possible output for following example is:
"secondNumber prototype number thirdNumber"

### Example

```js
let obj;
obj.number = 21;
obj["secondNumber"] = 22;
obj.addProperty("thirdNumber", 1);
obj.getOwnPropertyNames().forEach(function {
    console.write(args[0], " ");
});
```

