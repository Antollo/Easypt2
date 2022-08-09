# SslServer.classPrototype.constructor

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Function`]({{site.baseurl}}\built-ins\Function) 
</dd>
<dt> Parameters </dt><dd markdown="1">
 `cert /*String*/, key /*String*/` 
</dd>
<dt> Return value </dt><dd markdown="1">
 `this` 
</dd>

</dl>

### Description

Creates a new SSL server socket. Files are searched in directories 
listed in `import.getImportPaths()`.
 - `cert` - path to certificate stored in file in PEM format
            (see `SSL_CTX_use_certificate_file`)
 - `key`  - path to private key in PEM format
            (see `SSL_CTX_use_PrivateKey`)
 

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

