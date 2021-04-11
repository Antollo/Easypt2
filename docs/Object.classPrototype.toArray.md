## Name: `Object.classPrototype.toArray`

#### Instance of: [`Function`](Function.md)

#### Return value: `/*Array*/`

### Description:

Converts object to number:
- from `Number` (creates one element array containing 
  this object)
- from `String` (creates one element array containing 
  this object)
- from `Boolean` (creates one element array containing 
  this object)
- from other types (if not default `toArray` method 
  exists, calls it, otherwise creates one element array 
  containing this object)

#### Example:

```js
console.write("abc".toArray(), " ", true.toArray());
```

##### Expected output:

```
["abc"] [true]
```

