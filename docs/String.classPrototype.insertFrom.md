## Name: `String.classPrototype.insertFrom`

#### Instance of: [`Function`](Function.md)

#### Parameters: `n /*Number*/, string /*String*/, [position = 0 /*Number*/], [length = string.length() /*Number*/]`

#### Return value: `this`

### Description:

Inserts bytes to string before n-th byte from substring of second
argument starting at given position and of given length.

#### Example:

```js
let x = "xyz";
x.insertFrom(1, "abc", 1, 2);
console.write(x);
```

##### Expected output:

```
xbcyz
```

