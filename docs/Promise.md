## Name: `Promise`

#### Instance of: [`Class`](Class.md)

#### Inherits from: [`Object`](Object.md)

### Description:

Represents an operation that that executes 
asynchronously and its resulting value or error that 
raised during its execution.

#### Example:

```js
Timeout(20).then(() => console.write(20, " "););
await Timeout(30);
console.write(30, " ");
```

##### Expected output:

```
20 30 
```

#### Members:

- [`await`](Promise.classPrototype.await.md)
- `constructor`
- [`then`](Promise.classPrototype.then.md)


#### Inherited members:

- `==`
- [`addProperty`](Object.classPrototype.addProperty.md)
- [`getId`](Object.classPrototype.getId.md)
- [`getOwnPropertyNames`](Object.classPrototype.getOwnPropertyNames.md)
- [`hasOwnProperty`](Object.classPrototype.hasOwnProperty.md)
- [`readOperator`](Object.classPrototype.readOperator.md)
- [`removeProperty`](Object.classPrototype.removeProperty.md)
- [`toArray`](Object.classPrototype.toArray.md)
- [`toBoolean`](Object.classPrototype.toBoolean.md)
- [`toNumber`](Object.classPrototype.toNumber.md)
- [`toString`](Object.classPrototype.toString.md)


### Properties:

- `classPrototype`
- [`tick`](Promise.tick.md)


