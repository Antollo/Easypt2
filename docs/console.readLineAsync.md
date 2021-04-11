## Name: `console.readLineAsync`

#### Instance of: [`Function`](Function.md)

#### Return value: `/*Promise of String*/`

### Description:

Reads string terminated by new line character 
from standard input stream.

#### Example:

```js
console.writeLine("Who is your favourite writer?");
let writer = await console.readLineAsync();
console.writeLine("I like books written by ", writer, " too.");
```

