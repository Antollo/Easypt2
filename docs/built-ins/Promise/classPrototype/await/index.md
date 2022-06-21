# Promise.classPrototype.await

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*Object*/` 
</dd>
</dl>

### Description

Suspends the current context of execution until 
asynchronous operation associated with promise 
completes. Returns value returned by that 
operation. `await` doesn't block, the control swaps 
to another task.

### Example

```js
let condition = false;
let promise = Promise(() => {
    while (!condition)
        Promise.tick();
    console.write("c is now true");
});
Timeout(10).then(() => condition = true;);

// We can't use promise.await()
// directly because there is await
// operator. However it is possible
// to use promise["await"]()

await promise;
```

**Expected output**

```
c is now true
```

