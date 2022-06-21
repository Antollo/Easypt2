# console.readAsync

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*Promise of String*/` 
</dd>
</dl>

### Description

Reads word terminated by whitespace from standard input stream.

### Example

```js
console.writeLine("What is your name?");
let name = await console.readAsync();
console.writeLine("Welcome ", name);
```

