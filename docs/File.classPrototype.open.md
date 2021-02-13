## Name: `File.classPrototype.open`

#### Instance of: [`Function`](Function.md)

#### Parameters: `path /*String*/`

#### Return value: `/*File*/`

### Description:

Opens file specified by the path. If 
the file object was already associated 
with existing file close it first.

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



