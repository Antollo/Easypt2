# Promise.tick

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Return value </dt><dd markdown="1">
 `this` 
</dd>

</dl>

### Description

Suspends the current context of execution
and pass control to another context.

### Example

```js
let syn = false;
Timeout(10).then(() => {
    console.write(1);
    syn = true;
});
let b = Timeout(10).then(() => {
    while(!syn)
        Promise.tick();
    console.write(2);
});
await b;
```

**Expected output**

```
12
```

