## Name: `Timeout`

#### Instance of: [`Class`](Class.md)

#### Inherits from: [`Promise`](Promise.md)

#### Parameters: `milliseconds /*Number*/`

### Description:

Asynchronously waits for the given amount 
of time (in milliseconds). Completes after 
finishing waiting.

#### Example:

```js
// Wait for 10 ms
Timeout(1).then(() => console.write(1););
console.write(0);
await Timeout(23);
console.write(23);
```

##### Expected output:

```
0123
```

#### Members:

- `constructor`


#### Inherited members:

- [`await`](Promise.classPrototype.await.md)
- `constructor`
- [`then`](Promise.classPrototype.then.md)


### Properties:

- `classPrototype`


