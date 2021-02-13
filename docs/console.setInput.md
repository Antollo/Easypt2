## Name: `console.setInput`

#### Instance of: [`Function`](Function.md)

#### Parameters: `/*String*/, /*Function*/, [...]`

#### Return value: `console`

### Description:

Calls provided function (forwards arguments) and pass the 
string as input to this function. Useful for testing.

#### Example:

```js
console.setInput("abc", () => {
    let input = console.read();
    console.write(input);
});
```

##### Expected output:

```
abc
```

### Properties:



