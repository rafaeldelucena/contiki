# attom

## Setup your development environment

### Instant Contiki
This is a virtual machine for VMWare client, that contains the toolchain and some tools created from Contiki main developers.

Keep in mind that que ~/contiki version from this method doesn't have the most update master branch.

### Install the Toolchain

This commands bellow installs the toolchain from [Emdebian](https://wiki.debian.org/EmdebianToolchain).

```
apt-get install binutils-arm-none-eabi gcc-arm-none-eabi gdb-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib
```


The toolchain must be at 4.8.4 or higher.
```
rafael@backbone:~/work/atto/attom$ apt-cache show gcc-arm-none-eabi
Package: gcc-arm-none-eabi
Source: gcc-arm-none-eabi (11-1)
Version: 4.8.4-1+11-1
Installed-Size: 106255
Maintainer: Agustin Henze <tin@debian.org>
Architecture: amd64
Depends: libc6 (>= 2.14), libcloog-isl4 (>= 0.17), libgcc1 (>= 1:4.1.1), libgmp10, libisl10 (>= 0.10), libmpc3, libmpfr4 (>= 3.1.2), libstdc++6 (>= 4.1.1), zlib1g (>= 1:1.1.4), binutils-arm-none-eabi
Recommends: libnewlib-arm-none-eabi
Description-en: GCC cross compiler for ARM Cortex-A/R/M processors
 Bare metal compiler for embedded ARM chips using Cortex-M0/M0+/M3/M4,
  Cortex-R4/R5/R7 and Cortex-A* processors.
  Description-md5: 85395b75a50917cc4773abfc4610209d
  Homepage: http://gcc.gnu.org/
  Built-Using: gcc-4.8 (= 4.8.4-1)
  Section: devel
  Priority: extra
  Filename: pool/main/g/gcc-arm-none-eabi/gcc-arm-none-eabi_4.8.4-1+11-1_amd64.deb
  Size: 16440544
  MD5sum: 9e60f4e7d4a3ee92b7a41f1ab858d55c
  SHA1: 1fb9417712d3ca34774a3d21be5093014dcfd104
  SHA256: 4f3db7980291e3666d69c05dde0ff62f2d0510db1fdfcb5953786fb0f77262cc
```

### Contiki 3.0 Source Code

You must have the Contiki 3.0, you can get with the follow commands.
```
git clone https://github.com/contiki-os/contiki
cd contiki
git submodule update --init
```

## Build Instructions

### Build for attom platform
These make commands are executed inside a choosen project from default contiki examples directory.

```
make TARGET=srf06-cc26xx BOARD=attom/cc1310
```

### Build for cc13xx with development board

```
make TARGET=srf06-cc26xx BOARD=srf06/cc13xx
```

## Some Tips
Make sure that your border router and the motes are on same rf channel.

### Change RF Channel

#### Mote

For the mote applications you must change the macro on the header and rebuild the project, for example (~/contiki/examples/ipv6/slip-radio/project-conf.h)

```
#define RF_CORE_CONF_CHANNEL 25
```

### Change RF Frequency

Edit the file (~/contiki/cpu/cc26xx-cc13xx/rf-core/dot-15-4g.h) and change for another choosen frequency. The header file has a list of the pre-defined frequencies supported for this RF.

```
#define DOT_15_4G_CONF_FREQUENCY_BAND_ID DOT_15_4G_FREQUENCY_BAND_915
```
