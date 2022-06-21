# TcpServer.classPrototype.close

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Return value </dt><dd markdown="1">
 `this` 
</dd>

</dl>

### Description

Closes the socket.

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

