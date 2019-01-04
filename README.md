# libnvc

another cpp nvim msgpack-rpc client.  
nvim's rpc interface is convenient but not something you can finish in 10 minutes.

```cpp
#include "libnvc.hpp"

int main()
{
    // start nvim quickly:
    // $ nvim --listen "127.0.0.1:6666"
    libnvc::asio_socket socket;
    if(!socket.connect("localhost", 6666)){
        throw std::runtime_error("failed to connect to localhost:6666");
    }

    libnvc::api_client client(&socket);
    client.nvim_input({"$i123<CR>123<ESC>"});
    client.nvim_buf_set_name(1, "1234");
```

### build  
libnvc use asio and mpack internally but hiden by pimpl.  
so there is zero dependenct for user's building environment.

```bash
$ git clone https://github.com/etorth/libnvc.git
$ mkdir b && cd b && cmake ../libnvc
```
