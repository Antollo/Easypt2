## Name: `console.getOutput`

#### Instance of: [`Function`](Function.md)

#### Parameters: `/*Function*/, [...]`

#### Return value: `/*String*/`

### Description:

Calls provided function (forwards arguments) and intercept 
output written by that function. Useful for testing.

#### Example:

```js
function f (a) {
    console.write(a);
};
let output = console.getOutput(f, "abc");
console.write(output);
```

##### Expected output:

```
abc
```

### Properties:



