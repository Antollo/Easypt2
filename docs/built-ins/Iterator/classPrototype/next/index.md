# Iterator.classPrototype.next

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Return value </dt><dd markdown="1">
 `/*any*/` 
</dd>
</dl>

### Description

Returns the next element in the sequence or `Iterator.end`.

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

