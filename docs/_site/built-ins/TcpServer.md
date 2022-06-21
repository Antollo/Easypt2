# TcpServer

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Class`]({{site.baseurl}}\built-ins\Class) 
</dd>
<dt> Inherits from </dt><dd markdown="1">
 [`Object`]({{site.baseurl}}\built-ins\Object) 
</dd>
</dl>

### Description

TCP server listens for TCP connections.

### Example

```js
let tcpServer = TcpServer();
let port = 3000;
let portNotSet = true;

while (portNotSet) {
    try {
        await tcpServer.bind(port++);
        portNotSet = false;
    } catch {
        // https://wiki.c2.com/?PokemonExceptionHandling
    }
}

tcpServer.listen().then((tcpClientB) => {
    console.write(await tcpClientB.receive());
    await tcpClientB.close();
});

let tcpClientA = TcpClient();
await tcpClientA.connect("127.0.0.1", port);
await tcpClientA.send("abcd");
await tcpClientA.close();
```

**Expected output**

```
abcd
```

### Members

- [`bind`]({{site.baseurl}}\built-ins\TcpServer\classPrototype\bind\index)
- [`close`]({{site.baseurl}}\built-ins\TcpServer\classPrototype\close\index)
- `constructor`
- [`listen`]({{site.baseurl}}\built-ins\TcpServer\classPrototype\listen\index)


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


