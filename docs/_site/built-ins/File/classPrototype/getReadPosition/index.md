# File.classPrototype.getReadPosition

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*Number*/` 
</dd>
</dl>

### Description

Input operation. 
Returns read position of file
(counted from begining in bytes). 
All input operations set the write 
position to 0, and consistently all 
output operations set the read 
position to 0.

### Example

```js
let file = File();
file.create("ab.txt");
file.write("aaaaa");
file.setWritePosition(2);
file.write("bb");
file.setReadPosition(1);
console.write(file.readBytes(4), " ");
console.write(file.getReadPosition(), " ");
console.write(file.getWritePosition());
```

**Expected output**

```
abba 5 0
```

