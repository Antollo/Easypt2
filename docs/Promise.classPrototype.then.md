## Name: `Promise.classPrototype.then`

#### Instance of: [`Function`](Function.md)

#### Parameters: `/*Function*/`

#### Return value: `/*Promise*/`

### Description:

When asynchronous operation associated with 
this promise completes its resulting value is
passed to callback supplied with then method.
Represents value returned by that callback.

#### Example:

```js
let promise = Timeout(10)
    .then(() => 7;)
    .then((x) => x + 3;)
    .then((x) => x / 2;)
    .then((x) => console.write(x););
await promise;
```

##### Expected output:

```
5
```

### Properties:

- `prototype`


