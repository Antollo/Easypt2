# Object.classPrototype.getId

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*String*/` 
</dd>
</dl>

### Description

Returns array of bytes unique for each Object instance.
It can be equal to the id of the destructed object.

### Example

```js
console.writeLine(1.getId());
console.writeLine("ABC".getId());
console.writeLine(Object().getId());
```

