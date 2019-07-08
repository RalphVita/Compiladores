#!/usr/bin/expect -f

spawn ./trab5 < ./test/in/c05.cm
expect "%"
send "5\r"

