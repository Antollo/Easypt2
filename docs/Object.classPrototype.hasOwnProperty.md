## Name: `Object.classPrototype.hasOwnProperty`

#### Instance of: [`Function`](Function.md)

#### Parameters: `key /*String*/`

#### Return value: `/*Boolean*/`

### Description:

Checks whether the object has own property.

#### Example:

```js
let obj;
obj.addProperty("first", 1);
console.write(obj.hasOwnProperty("first"), " ", obj.hasOwnProperty("second"));
```

##### Expected output:

```
true false
```

