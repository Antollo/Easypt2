## Name: `Object.classPrototype.getId`

#### Instance of: [`Function`](Function.md)

#### Return value: `/*String*/`

### Description:

Returns array of bytes unique for each Object instance.
It can be equal to the id of the destructed object.

#### Example:

```js
console.writeLine(1.getId());
console.writeLine("ABC".getId());
console.writeLine(Object().getId());
```

