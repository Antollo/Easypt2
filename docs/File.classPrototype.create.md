## Name: `File.classPrototype.create`

#### Instance of: [`Function`](Function.md)

#### Parameters: `path /*String*/`

#### Return value: `/*File*/`

### Description:

Creates (or open and remove contents) 
file in the specified path.

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


