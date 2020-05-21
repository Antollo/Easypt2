## Name: `import`

#### Instance of: [`Function`](Function.md)

#### Parameters: `file /*String or File*/`

#### Return value: `/*Object*/`

### Description:

Imports source code from another *.ez file (source parsed, and called, 
return value is returned as import return value). First and only parameter 
is filename (file is searched in current working directory and then in 
interpreter directory) or opened `File` object.

#### Example:

```js
let file = File();
file.create("module.ez");
file.write("let x = 7; let y = 3; return x*y + y;");
file.close();
let result = import("module.ez");
file.remove();
console.write(result);
```

##### Expected output:

```
24
```

### Properties:

- `prototype`


