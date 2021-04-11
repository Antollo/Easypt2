## Name: `Promise.tick`

#### Instance of: [`Function`](Function.md)

#### Return value: `this`

### Description:

Suspends the current context of execution
and pass control to another context.

#### Example:

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

##### Expected output:

```
12
```

