### Specs:
```
$gcc-7 --version
gcc-7 (Homebrew GCC 7.3.0_1) 7.3.0
Copyright (C) 2017 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

$elixir --version
Elixir 1.6.1 (compiled with OTP 20)
```
### Compilation
`gcc-7 -undefined dynamic_lookup -dynamiclib niftest.c -o niftest.so -I /usr/local/Cellar/erlang/20.2.4/lib/erlang/usr/include`

### Usage
Change compilation in `test` file and run `./test`

### Docs
**EMatrix**: _list of lists of doubles_

**EVector**: _list of doubles_

**Matrix**: _custom data type (ERL_NIF_TERM)_

**Vector**: _custom data type (ERL_NIF_TERM)_
- - - - 
make_vector(**EVector**) -> **Vector**

make_matrix(**EMatrix**) -> **Matrix**

make_list(**Vector**) -> **EVector**

make_deep_list(**Matrix**) -> **EMatrix**

vsum(**Vector**, **Vector**) -> **Vector** 

msum(**Matrix**, **Matrix**) -> **Matrix**

scale(**double**, **Vector**) -> **Vector**

scale(**double**, **Matrix**) -> **Matrix**

get_col(**int**, **Matrix**) -> **Matrix**

get_row(**int**, **Matrix**) -> **Matrix**

get_col(**int(1 or -1)**, **Matrix**) -> **Matrix**

mult(**Matrix**, **Vector**) -> **Vector**

mult(**Matrix**, **Matrix**) -> **Vector**
