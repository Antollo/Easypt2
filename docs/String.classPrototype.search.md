## Name: `String.classPrototype.search`

#### Instance of: [`Function`](Function.md)

#### Parameters: `regex /*String*/`

#### Return value: `/*Array*/`

### Description:

Returns array of object in form (position, length)
representing first substring that matches regex
and substrings that match subexpression of this regex.

#### Example:

```js
let arr = "Roses are red.".search("([A-Z]+)([a-z]+)");
for (let i = 0; i < arr.length(); i++)
    console.write(arr[i].position, " ", arr[i].length, ", ");
```

##### Expected output:

```
0 5, 0 1, 1 4, 
```

### Properties:

- `prototype`


