## Name: `String.classPrototype.append`

#### Instance of: [`Function`](Function.md)

#### Parameters: `str /*String*/, [position = 0 /*Number*/, length = string.length() /*Number*/]`

#### Return value: `this`

### Description:

Appends str to this string (or optionally 
substring starting from given position 
and of given length).

#### Example:

```js
let str = "Roses are ";
let str2 = "red, Violets are blue.";
str.append(str2);
console.write(str);
str = "Roses are ";
str.append(str2, 17, 5);
console.write(" | ", str);
```

##### Expected output:

```
Roses are red, Violets are blue. | Roses are blue.
```

