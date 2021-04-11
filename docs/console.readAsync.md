## Name: `console.readAsync`

#### Instance of: [`Function`](Function.md)

#### Return value: `/*Promise of String*/`

### Description:

Reads word terminated by whitespace from standard input stream.

#### Example:

```js
console.writeLine("What is your name?");
let name = await console.readAsync();
console.writeLine("Welcome ", name);
```

