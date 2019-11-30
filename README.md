# libnvc

another cpp nvim msgpack-rpc client.  
nvim's rpc interface is convenient but not something you can finish in 10 minutes.

![image](https://github.com/etorth/libnvc/raw/master/readme/cap2.png)
![image](https://github.com/etorth/libnvc/raw/master/readme/cap1.png)

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

### build  
libnvc use asio and mpack internally but hiden by pimpl.  
so there is zero dependenct for user's building environment.

```bash
# build the libnvc library
$ cd $HOME
$ git clone https://github.com/etorth/libnvc.git
$ mkdir b_libnvc && cd b_libnvc
$ cmake ../libnvc -DCMAKE_INSTALL_PREFIX=install
$ make && make install

# build the sample project, a simple nvim gui
$ cd $HOME
$ mkdir b_nvim_sdl2 && cd b_nvim_sdl2
$ cmake ../libnvc/sample/nvim_sdl2 -DCMAKE_INSTALL_PREFIX=install -DLIBNVC_INCLUDE=$HOME/b_libnvc/install/include -DLIBNVC_LIB=$HOME/b_libnvc/install/lib
$ make && make install

# run the sample gui
$ $HOME/b_nvim_sdl2/install/nvim_sdl2/nvim_sdl2
```
