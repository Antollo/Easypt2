## Name: `String.classPrototype.indexOf`

#### Instance of: [`Function`](Function.md)

#### Parameters: `key /*String*/, [position /*Number*/]`

#### Return value: `/*Number*/`

### Description:

Returns the index of the first occurrence of key 
(optionaly starting from given position) 
or -1 if the key is not found.

#### Example:

```js
let str = "Roses are red, Violets are blue, If you're reading this, I'm sorry for you.";
let a = str.indexOf("are");
let b = str.indexOf("are", a + 1);
let c = str.indexOf("yellow");
console.write(a, " ", b, " ", c);
```

##### Expected output:

```
6 23 -1
```

