## Name: `Object.classPrototype.getOwnPropertyNames`

#### Instance of: [`Function`](Function.md)

#### Return value: `/*Array*/`

### Description:

Returns array of all own properties. Order is not specified.
Possible output for following example is:
"secondNumber prototype number thirdNumber"

#### Example:

```js
let obj;
obj.number = 21;
obj["secondNumber"] = 22;
obj.addProperty("thirdNumber", 1);
obj.getOwnPropertyNames().forEach(function {
    console.write(args[0], " ");
});
```

