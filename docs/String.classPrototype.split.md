## Name: `String.classPrototype.split`

#### Instance of: [`Function`](Function.md)

#### Parameters: `delimeter /*String*/`

#### Return value: `/*Array*/`

### Description:

Returns array of substrings delimited by 
the delimeter in the original string.

#### Example:

```js
let arr = "abcd abcde abcdef bcdefg".split("cd");
for (let i = 0; i<arr.length(); i++)
    console.write(arr[i], ", ");
```

##### Expected output:

```
ab,  ab, e ab, ef b, efg, 
```

