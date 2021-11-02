## Name: `TcpClient`

#### Instance of: [`Class`](Class.md)

#### Inherits from: [`Object`](Object.md)

### Description:

TCP client provides TCP socket 
input and output operations.

#### Example:

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

##### Expected output:

```
abcd
```

#### Members:

- [`close`](TcpClient.classPrototype.close.md)
- `connect`
- `constructor`
- [`receive`](TcpClient.classPrototype.receive.md)
- [`send`](TcpClient.classPrototype.send.md)


#### Inherited members:

- `==`
- [`addProperty`](Object.classPrototype.addProperty.md)
- [`getId`](Object.classPrototype.getId.md)
- [`getOwnPropertyNames`](Object.classPrototype.getOwnPropertyNames.md)
- [`hasOwnProperty`](Object.classPrototype.hasOwnProperty.md)
- [`readOperator`](Object.classPrototype.readOperator.md)
- [`removeProperty`](Object.classPrototype.removeProperty.md)
- [`toArray`](Object.classPrototype.toArray.md)
- [`toBoolean`](Object.classPrototype.toBoolean.md)
- [`toNumber`](Object.classPrototype.toNumber.md)
- [`toString`](Object.classPrototype.toString.md)


### Properties:

- `classPrototype`


