# bind2device
This tool binds an app to a different network device

## Usage 

A bash script is provided for convenience:
```bash
$ ./bind2device.sh <device> <path_to_exe> <arguments>
```

LD_PRELOAD:
```bash
$ BIND2DEVICE=<device> LD_PRELOAD=./bind2device.so <path_to_exe> <arguments>
```

## Example

A bash script is provided for convenience:

```
$ curl https://ifconfig.me
140.82.116.4
$ curl --interface tun0 https://ifconfig.me
204.79.197.200
$ ./bind2device.sh tun0 curl https://ifconfig.me
204.79.197.200
```


## How
A shared library is provided which can be used with LD_PRELOAD to change application behavior at run-time. The 'socket' call is intercepted and rewired to use the specified interface (SO_BINDTODEVICE).

## Build
```
$ make
```

## Credits
https://unix.stackexchange.com/a/648791
