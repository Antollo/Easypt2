# Iterator.end

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Boolean`]({{site.baseurl}}\built-ins\Boolean) 
</dd>
</dl>

### Description

Represents the end of iteration.

### Example

```js
let it <- [1, 2, 3, 4].iterator()
while (!(let i <- it.next()) is Iterator.end)
    console.write(i)
```

**Expected output**

```
1234
```

