## Name: `console.controlSequence`

#### Instance of: [`Function`](Function.md)

#### Return value: `/*String*/`

### Description:

Writes ANSI escape code starting with `ESC` and `[` 
(CSI sequence) to the standard output stream.

#### Example:

```js
console.controlSequence("7m").write("special").controlSequence("0m");
```

### Properties:

- `prototype`


