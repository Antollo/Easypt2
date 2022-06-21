# File

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Class`]({{site.baseurl}}\built-ins\Class) 
</dd>
<dt> Inherits from </dt><dd markdown="1">
 [`Object`]({{site.baseurl}}\built-ins\Object) 
</dd>
</dl>

### Description

File class provides file input and output operations.

### Example

```js
let file = File();
file.create("protocols.txt");
file.writeLine("http").write(80);
file.setReadPosition(0);

let name = file.readLine();
let port = file.read();
file.close();

console.write(name, " ", port);
```

**Expected output**

```
http 80
```

### Members

- [`close`]({{site.baseurl}}\built-ins\File\classPrototype\close\index)
- [`constructor`]({{site.baseurl}}\built-ins\File\classPrototype\constructor\index)
- [`create`]({{site.baseurl}}\built-ins\File\classPrototype\create\index)
- [`flush`]({{site.baseurl}}\built-ins\File\classPrototype\flush\index)
- [`getReadPosition`]({{site.baseurl}}\built-ins\File\classPrototype\getReadPosition\index)
- [`getWritePosition`]({{site.baseurl}}\built-ins\File\classPrototype\getWritePosition\index)
- [`open`]({{site.baseurl}}\built-ins\File\classPrototype\open\index)
- [`read`]({{site.baseurl}}\built-ins\File\classPrototype\read\index)
- [`readAsync`]({{site.baseurl}}\built-ins\File\classPrototype\readAsync\index)
- [`readBytes`]({{site.baseurl}}\built-ins\File\classPrototype\readBytes\index)
- [`readBytesAsync`]({{site.baseurl}}\built-ins\File\classPrototype\readBytesAsync\index)
- [`readLine`]({{site.baseurl}}\built-ins\File\classPrototype\readLine\index)
- [`readLineAsync`]({{site.baseurl}}\built-ins\File\classPrototype\readLineAsync\index)
- [`readTo`]({{site.baseurl}}\built-ins\File\classPrototype\readTo\index)
- [`readToAsync`]({{site.baseurl}}\built-ins\File\classPrototype\readToAsync\index)
- [`remove`]({{site.baseurl}}\built-ins\File\classPrototype\remove\index)
- [`setReadPosition`]({{site.baseurl}}\built-ins\File\classPrototype\setReadPosition\index)
- [`setWritePosition`]({{site.baseurl}}\built-ins\File\classPrototype\setWritePosition\index)
- [`size`]({{site.baseurl}}\built-ins\File\classPrototype\size\index)
- [`write`]({{site.baseurl}}\built-ins\File\classPrototype\write\index)
- [`writeAsync`]({{site.baseurl}}\built-ins\File\classPrototype\writeAsync\index)
- [`writeLine`]({{site.baseurl}}\built-ins\File\classPrototype\writeLine\index)
- [`writeLineAsync`]({{site.baseurl}}\built-ins\File\classPrototype\writeLineAsync\index)


### Inherited members

- `==`
- [`addProperty`]({{site.baseurl}}\built-ins\Object\classPrototype\addProperty\index)
- `classPrototype`
- [`getId`]({{site.baseurl}}\built-ins\Object\classPrototype\getId\index)
- [`getOwnPropertyNames`]({{site.baseurl}}\built-ins\Object\classPrototype\getOwnPropertyNames\index)
- [`hasOwnProperty`]({{site.baseurl}}\built-ins\Object\classPrototype\hasOwnProperty\index)
- [`readOperator`]({{site.baseurl}}\built-ins\Object\classPrototype\readOperator\index)
- [`removeProperty`]({{site.baseurl}}\built-ins\Object\classPrototype\removeProperty\index)
- [`toArray`]({{site.baseurl}}\built-ins\Object\classPrototype\toArray\index)
- [`toBoolean`]({{site.baseurl}}\built-ins\Object\classPrototype\toBoolean\index)
- [`toJson`]({{site.baseurl}}\built-ins\Object\classPrototype\toJson\index)
- [`toNumber`]({{site.baseurl}}\built-ins\Object\classPrototype\toNumber\index)
- [`toString`]({{site.baseurl}}\built-ins\Object\classPrototype\toString\index)


### Properties

- `classPrototype`


