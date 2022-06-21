# File.classPrototype.remove

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*File*/` 
</dd>
</dl>

### Description

Deletes the recently associated file. 
If the file object was associated with 
file close it first.

### Example

```js
let file = File();
file.create("protocols.txt");
file.remove();
try {
    file.open("protocols.txt");
} catch {
    console.write("error");
}
```

**Expected output**

```
error
```

