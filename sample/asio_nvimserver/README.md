# asio\_nvimserver

start nvim and wait for connection to it
at server side:
```bash
$ asio_nvimserver --port 6666
```

at client side, embeds the nvim remotely:
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
    client.nvim_input("$i123<CR>123<ESC>");
    client.nvim_buf_set_name(1, "1234");
```
