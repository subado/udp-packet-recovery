### Example

Command to run server:

```sh
./udp-packet-recovery -s -p 50002 -f out -4 -v
```

Command to run client:

```sh
./udp-packet-recovery -c -p 50000 -r 50002 -f input -4 127.0.0.1  -m -n 1000 -v
```

Using program with loopback device may result to corruption in server out file,
due to the fact that [Linux turns off IP checksumming on the loopback device for performance](https://lkml.iu.edu/hypermail/linux/kernel/0308.1/2205.html).
