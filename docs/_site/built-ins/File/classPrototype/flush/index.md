# File.classPrototype.flush

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*File*/` 
</dd>
</dl>

### Description

Makes sure all output operations are
saved to the underlaying storage.

### Example

```js
let file = File();
file.create("abc.txt");
file.write("abbacda");
console.write(file.read());
file.flush();
// File contents are now saved o disk
file.close();
```

**Expected output**

```
abbacda
```

