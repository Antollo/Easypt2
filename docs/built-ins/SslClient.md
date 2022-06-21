# SslClient

<dl>
<dt> Instance of </dt><dd markdown="1">
 [`Class`]({{site.baseurl}}\built-ins\Class) 
</dd>
<dt> Inherits from </dt><dd markdown="1">
 [`TcpClient`]({{site.baseurl}}\built-ins\TcpClient) 
</dd>
</dl>

### Description

SSL client provides secure socket (such as SSL 
or TLS) input and output operations.

### Example

```js
// HTTPS request
let tcpClientA = SslClient();
await tcpClientA.connect("www.example.com", 443);
await tcpClientA.send("GET / HTTP/1.1\r\nHost: www.example.com\r\n\r\n");
console.write((await tcpClientA.receive()).split("\r")[0]);
await tcpClientA.close();
```

**Expected output**

```
HTTP/1.1 200 OK
```

### Members

- `constructor`


### Inherited members

- [`close`]({{site.baseurl}}\built-ins\TcpClient\classPrototype\close\index)
- [`connect`]({{site.baseurl}}\built-ins\TcpClient\classPrototype\connect\index)
- `constructor`
- [`receive`]({{site.baseurl}}\built-ins\TcpClient\classPrototype\receive\index)
- [`send`]({{site.baseurl}}\built-ins\TcpClient\classPrototype\send\index)


### Properties

- `classPrototype`


