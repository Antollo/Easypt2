## Name: `File.classPrototype.open`

#### Instance of: [`Function`](Function.md)

#### Parameters: `/*String*/`

#### Return value: `/*File*/`

### Description:

Opens file specified by the path.

#### Example:

```js
let file = File();
file.create("protocols.txt");
file.remove();
try {
    file.open("protocols.txt");
} catch {
    console.write("error");
}
```

##### Expected output:

```
error
```

### Properties:

- `prototype`


