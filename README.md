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

