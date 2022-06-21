# Promise.classPrototype.then

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `/*Function*/` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*Promise*/` 
</dd>
</dl>

### Description

When asynchronous operation associated with 
this promise completes its resulting value is
passed to callback supplied with then method.
Represents value returned by that callback.

### Example

```js
let promise = Timeout(10)
    .then(() => 7;)
    .then((x) => x + 3;)
    .then((x) => x / 2;)
    .then((x) => console.write(x););
await promise;
```

**Expected output**

```
5
```

