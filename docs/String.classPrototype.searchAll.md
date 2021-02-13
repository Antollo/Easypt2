## Name: `String.classPrototype.searchAll`

#### Instance of: [`Function`](Function.md)

#### Parameters: `regex /*String*/`

#### Return value: `/*Array*/`

### Description:

Returns array of object in form (position, length)
representing all substrings that matches regex.

#### Example:

```js
let arr = "Roses are red.".searchAll("[a-z]+");
for (let i = 0; i < arr.length(); i++)
    console.write(arr[i].position, " ", arr[i].length, ", ");
```

##### Expected output:

```
1 4, 6 3, 10 3, 
```

### Properties:



