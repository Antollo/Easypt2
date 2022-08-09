# SslServer

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Class`]({{site.baseurl}}\built-ins\Class) 
</dd>
<dt> Inherits from </dt><dd markdown="1">
 [`TcpServer`]({{site.baseurl}}\built-ins\TcpServer) 
</dd>
</dl>

### Description

SSL server listens for secure sockets (such as SSL or TLS) connections.

### Example

```js
// Simple HTTPS server
let tcpServer = SslServer("cert.pem", "cert.key");
await tcpServer.bind(3003);
while (true) {
    try {
        let tcpClientB = await tcpServer.listen();
        let time = Time.sinceEpoch.asFloatingPoint();
        await tcpClientB.receive();
        await tcpClientB.send("HTTP/1.1 200 OK\r\n"+
                            "Server: Easypt ssl example\r\n"+
                            "Content-Length: 48\r\n"+
                            "Content-Type: text/html\r\n"+
                            "Connection: Closed\r\n\r\n"+
                            "<html>"+
                            "<body>"+
                            "<h1>Hello, World!</h1>"+
                            "</body>"+
                            "</html>");
        console.log(Time.sinceEpoch.asFloatingPoint() - time);
    } catch {
        console.error(exception);
    }
}
```

### Members

- [`constructor`]({{site.baseurl}}\built-ins\SslServer\classPrototype\constructor\index)


### Inherited members

- [`bind`]({{site.baseurl}}\built-ins\TcpServer\classPrototype\bind\index)
- [`close`]({{site.baseurl}}\built-ins\TcpServer\classPrototype\close\index)
- `constructor`
- [`listen`]({{site.baseurl}}\built-ins\TcpServer\classPrototype\listen\index)


### Properties

- `classPrototype`


