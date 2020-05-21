## Name: `console.getOutput`

#### Instance of: [`Function`](Function.md)

#### Parameters: `/*Function*/`

#### Return value: `/*String*/`

### Description:

Calls provided function and intercet output written 
by that function. Useful for testing.

#### Example:

```js
function f {
    console.write("abc");
};
let output = console.getOutput(f);
console.write(output);
```

##### Expected output:

```
abc
```

### Properties:

- `prototype`


