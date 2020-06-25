## Name: `Object.classPrototype.removeProperty`

#### Instance of: [`Function`](Function.md)

#### Parameters: `key /*String*/, value`

### Description:

Adds new property to object.

#### Example:

```js
let obj;
obj.addProperty("first", 1);
obj.addProperty("second", "a");
obj.removeProperty("second");
if (obj.hasOwnProperty("second"))
    console.write(obj.second);
else
    console.write(obj.first);
```

##### Expected output:

```
1
```

### Properties:

- `prototype`


