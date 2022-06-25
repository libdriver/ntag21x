### 1. chip

#### 1.1 chip info

chip name : Raspberry Pi 4B

iic pin: SCL/SDA GPIO3/GPIO2

spi pin: SCLK/MOSI/MISO/CS GPIO11/GPIO10/GPIO9/GPIO8

uart pin: TX/RX GPIO14/GPIO15

gpio pin: INT GPIO17

reset pin: RESET GPIO26

### 2. install

#### 2.1 install info

```shell
sudo apt-get install libgpiod-dev

make
```

### 3. ntag21x

#### 3.1 command Instruction

​           ntag21x is a basic command which can test all ntag21x driver function:

​           -i          show ntag21x chip and driver information.

​           -h        show ntag21x help.

​           -p        show ntag21x pin connections of the current board.

​           -t card        run ntag21x card test. 

​           -c (halt | wake_up | read <page> | read_pages <startpage> <stoppage> | read4 <startpage> | write <page> | version | counter | signature | serial_number | set (-pwd <pwd> <pack> | -dynamic <lock> | -static <lock> | -mirror <mirror> <byte> <page> | -mode <mode> | -protect <page> | -limitation <limit> | -access <access> <enable>) | authenticate <pwd> <pack>)

​           -c halt        chip halt.

​           -c wake_up        chip wake up.

​           -c read <page>        chip read page.page is the read page.

​           -c read_pages <startpage> <stoppage>        chip read pages.startpage is the start page,stoppage is the stop page.

​           -c read4 <startpage>        chip read four pages.startpage is the start page.

​           -c write <page>        chip write page.page is the written page.

​           -c version        get the version.

​           -c counter        get the chip read counter.

​           -c signature        get the chip signature.

​           -c serial_number        get the chip serial number.

​           -c set -pwd <pwd> <pack>        set the password.pwd is the password,pack is the checked pack.

​           -c set -dynamic <lock>        set the dynamic lock.lock is the set lock.

​           -c set -static <lock>        set the static lock.lock is the set lock.

​           -c set -mirror <mirror> <byte> <page>        set the chip mirror.mirror is the type and it can be "NO_ASCII", "UID_ASCII", "NFC_COUNTER_ASCII" or "UID_NFC_COUNTER_ASCII".byte is the page byte and it can be 0, 1, 2, 3.page the mirror page.

​           -c set -mode <mode>        set the mode.mode can be "NORMAL" or "STRONG".

​           -c set -protect <page>        set the start protect page.page is the start page.

​           -c set -limitation <limit>        set the authenticate limitation.limit is the set limit and it can be 0 - 7.

​           -c set -access <access> <enable>        set the access.access can be "READ_PROTECTION", "USER_CONF_PROTECTION", "NFC_COUNTER" or "NFC_COUNTER_PASSWORD_PROTECTION".enable is the set bool and it can be 0 or 1.

​           -c authenticate <pwd> <pack>        password authenticate .pwd is the password,pack is the checked pack.

#### 3.2 command example

```shell
./ntag21x -i

ntag21x: chip is NXP NTAG213/5/6.
ntag21x: manufacturer is NXP.
ntag21x: interface is RF.
ntag21x: driver version is 1.0.
ntag21x: min supply voltage is 3.3V.
ntag21x: max supply voltage is 4.0V.
ntag21x: max current is 30.00mA.
ntag21x: max temperature is 70.0C.
ntag21x: min temperature is -25.0C.
```

```shell
./ntag21x -p

ntag21x: SPI interface SCK connected to GPIO11(BCM).
ntag21x: SPI interface MISO connected to GPIO9(BCM).
ntag21x: SPI interface MOSI connected to GPIO10(BCM).
ntag21x: SPI interface CS connected to GPIO8(BCM).
ntag21x: IIC interface SCL connected to GPIO3(BCM).
ntag21x: IIC interface SDA connected to GPIO2(BCM).
ntag21x: UART interface TX connected to GPIO14(BCM).
ntag21x: UART interface RX connected to GPIO15(BCM).
ntag21x: INT connected to GPIO17(BCM).
ntag21x: RESET connected to GPIO26(BCM).
```

```shell
./ntag21x -t card

ntag21x: chip is NXP NTAG213/5/6.
ntag21x: manufacturer is NXP.
ntag21x: interface is RF.
ntag21x: driver version is 1.0.
ntag21x: min supply voltage is 3.3V.
ntag21x: max supply voltage is 4.0V.
ntag21x: max current is 30.00mA.
ntag21x: max temperature is 70.0C.
ntag21x: min temperature is -25.0C.
ntag21x: start card test.
ntag21x: contactless transceiver failed.
ntag21x: wake up failed.
ntag21x: run failed.
ntag21x: chip is NXP NTAG213/5/6.
ntag21x: manufacturer is NXP.
ntag21x: interface is RF.
ntag21x: driver version is 1.0.
ntag21x: min supply voltage is 3.3V.
ntag21x: max supply voltage is 4.0V.
ntag21x: max current is 30.00mA.
ntag21x: max temperature is 70.0C.
ntag21x: min temperature is -25.0C.
ntag21x: start card test.
ntag21x: find ntag213/5/6 card.
ntag21x: id1 is 0x88 0x04 0x66 0x67.
ntag21x: id2 is 0x9A 0x90 0x6B 0x81.
ntag21x: get capability container ntag216.
ntag21x: serial number is 0x04 0x66 0x67 0x9A 0x90 0x6B 0x81 
ntag21x: signature is 0x54 0x75 0x89 0xC5 0x53 0x88 0x36 0xAA 0x10 0xAB 0x79 0x5D 0x52 0x23 0x4D 0x42 0x74 0x79 0x96 0xB2 0x51 0xA5 0x92 0xE9 0x86 0xE4 0x5D 0xCA 0xD7 0x46 0x0A 0x38 
ntag21x: read four pages from page 0.
0x04 0x66 0x67 0x8D 0x9A 0x90 0x6B 0x81 0xE0 0x48 0x00 0x00 0xE1 0x10 0x6D 0x00 
ntag21x: read page 3.
0xE1 0x10 0x6D 0x00 
ntag21x: fast read page from page 0 - page 5.
0x04 0x66 0x67 0x8D 0x9A 0x90 0x6B 0x81 0xE0 0x48 0x00 0x00 0xE1 0x10 0x6D 0x00 0x03 0x00 0xFE 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 
ntag21x: compatibility write page 16.
ntag21x: check page ok.
ntag21x: write page 17.
ntag21x: check page ok.
ntag21x: authenticate.
ntag21x: fixed_header is 0x00
ntag21x: vendor_id is 0x04
ntag21x: product_type is 0x04
ntag21x: product_subtype is 0x02
ntag21x: major_product_version is 0x01
ntag21x: minor_product_version is 0x00
ntag21x: storage_size is 0x13
ntag21x: protocol_type is 0x03
ntag21x: set dynamic lock 0x00 0x00 0x00
ntag21x: get dynamic lock 0x00 0x00 0x00
ntag21x: set static lock 0x00 0x00
ntag21x: get static lock 0x00 0x00
ntag21x: set uid and nfc counter mirror.
ntag21x: check mirror ok.
ntag21x: set nfc counter mirror.
ntag21x: check mirror ok.
ntag21x: set uid ascii mirror.
ntag21x: check mirror ok.
ntag21x: set no ascii mirror.
ntag21x: check mirror ok.
ntag21x: set mirror byte 3.
ntag21x: check mirror byte ok.
ntag21x: set mirror byte 2.
ntag21x: check mirror byte ok.
ntag21x: set mirror byte 1.
ntag21x: check mirror byte ok.
ntag21x: set mirror byte 0.
ntag21x: check mirror byte ok.
ntag21x: set normal mode.
ntag21x: check modulation mode ok.
ntag21x: set strong mode.
ntag21x: check modulation mode ok.
ntag21x: set mirror page 0x00.
ntag21x: check mirror page ok.
ntag21x: set protect start page 0xFF.
ntag21x: check protect start page ok.
ntag21x: read access protection disable.
ntag21x: check read access protection ok.
ntag21x: user configuration protection disable.
ntag21x: check user configuration protection ok.
ntag21x: nfc counter protection enable.
ntag21x: check nfc counter protection ok.
ntag21x: nfc counter password protection enable.
ntag21x: check nfc counter password protection ok.
ntag21x: set authenticate limitation 7.
ntag21x: check authenticate limitation ok.
ntag21x: read counter 2.
ntag21x: finish card test.
```

```shell
./ntag21x -c halt

ntag21x: halt.
```

```shell
./ntag21x -c wake_up

ntag21x: wake up.
```

```shell
./ntag21x -c read 0

ntag21x: find ntag216 card.
ntag21x: id is 0x88 0x04 0x66 0x67 0x9A 0x90 0x6B 0x81 
ntag21x: read page 0: 0x04 0x66 0x67 0x8D 
```

```shell
./ntag21x -c read_pages 0 3

ntag21x: find ntag216 card.
ntag21x: id is 0x88 0x04 0x66 0x67 0x9A 0x90 0x6B 0x81 
ntag21x: read pages 0 - 3: 0x04 0x66 0x67 0x8D 0x9A 0x90 0x6B 0x81 0xE0 0x48 0x00 0x00 0xE1 0x10 0x6D 0x00 
```

```shell
./ntag21x -c read4 0

ntag21x: find ntag216 card.
ntag21x: id is 0x88 0x04 0x66 0x67 0x9A 0x90 0x6B 0x81 
ntag21x: read page 0: 0x04 0x66 0x67 0x8D 0x9A 0x90 0x6B 0x81 0xE0 0x48 0x00 0x00 0xE1 0x10 0x6D 0x00 
```

```shell
./ntag21x -c write 20 12345678 

ntag21x: find ntag216 card.
ntag21x: id is 0x88 0x04 0x66 0x67 0x9A 0x90 0x6B 0x81 
ntag21x: write page 20: 0x12 0x34 0x56 0x78 
```

```shell
./ntag21x -c version

ntag21x: find ntag216 card.
ntag21x: id is 0x88 0x04 0x66 0x67 0x9A 0x90 0x6B 0x81 
ntag21x: fixed_header is 0x00
ntag21x: vendor_id is 0x04
ntag21x: product_type is 0x04
ntag21x: product_subtype is 0x02
ntag21x: major_product_version is 0x01
ntag21x: minor_product_version is 0x00
ntag21x: storage_size is 0x13
ntag21x: protocol_type is 0x03
```

```shell
./ntag21x -c counter

ntag21x: find ntag216 card.
ntag21x: id is 0x88 0x04 0x66 0x67 0x9A 0x90 0x6B 0x81 
read counter 5.
```

```shell
./ntag21x -c signature

ntag21x: find ntag216 card.
ntag21x: id is 0x88 0x04 0x66 0x67 0x9A 0x90 0x6B 0x81 
ntag21x: signature is 0x54 0x75 0x89 0xC5 0x53 0x88 0x36 0xAA 0x10 0xAB 0x79 0x5D 0x52 0x23 0x4D 0x42 0x74 0x79 0x96 0xB2 0x51 0xA5 0x92 0xE9 0x86 0xE4 0x5D 0xCA 0xD7 0x46 0x0A 0x38 
```

```shell
./ntag21x -c serial_number

ntag21x: find ntag216 card.
ntag21x: id is 0x88 0x04 0x66 0x67 0x9A 0x90 0x6B 0x81 
ntag21x: serial number is 0x88 0x04 0x66 0x67 0x9A 0x90 0x6B 
```

```shell
./ntag21x -c set -pwd FFFFFFFF 0000

ntag21x: find ntag216 card.
ntag21x: id is 0x88 0x04 0x66 0x67 0x9A 0x90 0x6B 0x81 
ntag21x: set password 0xFF 0xFF 0xFF 0xFF ok.
ntag21x: set pack 0x00 0x00 ok.
```

```shell
./ntag21x -c set -dynamic 000000

ntag21x: find ntag216 card.
ntag21x: id is 0x88 0x04 0x66 0x67 0x9A 0x90 0x6B 0x81 
ntag21x: dynamic lock: 0x00 0x00 0x00 
```

```shell
./ntag21x -c set -static 0000

ntag21x: find ntag216 card.
ntag21x: id is 0x88 0x04 0x66 0x67 0x9A 0x90 0x6B 0x81 
ntag21x: static lock: 0x00 0x00 
```

```shell
./ntag21x -c set -mirror NO_ASCII 0 0

ntag21x: find ntag216 card.
ntag21x: id is 0x88 0x04 0x66 0x67 0x9A 0x90 0x6B 0x81 
ntag21x: set no ascii mirror.
ntag21x: set mirror byte 0.
ntag21x: set mirror page 0.
```

```shell
./ntag21x -c set -mode STRONG

ntag21x: find ntag216 card.
ntag21x: id is 0x88 0x04 0x66 0x67 0x9A 0x90 0x6B 0x81 
ntag21x: set the strong mode.
```

```shell
./ntag21x -c set -protect 255

ntag21x: find ntag216 card.
ntag21x: id is 0x88 0x04 0x66 0x67 0x9A 0x90 0x6B 0x81 
ntag21x: set protect start page 255.
```

```shell
./ntag21x -c set -limitation 7

ntag21x: find ntag216 card.
ntag21x: id is 0x88 0x04 0x66 0x67 0x9A 0x90 0x6B 0x81 
ntag21x: set authenticate limitation 7.
```

```shell
./ntag21x -c set -access NFC_COUNTER 1

ntag21x: find ntag216 card.
ntag21x: id is 0x88 0x04 0x66 0x67 0x9A 0x90 0x6B 0x81 
ntag21x: set access nfc counter protection true.
```

```shell
./ntag21x -c authenticate FFFFFFFF 0000

ntag21x: find ntag216 card.
ntag21x: id is 0x88 0x04 0x66 0x67 0x9A 0x90 0x6B 0x81 
ntag21x: authenticate password 0xFF 0xFF 0xFF 0xFF ok.
ntag21x: authenticate pack 0x00 0x00 ok.
```


```shell
./ntag21x -h

ntag21x -i
	show ntag21x chip and driver information.
ntag21x -h
	show ntag21x help.
ntag21x -p
	show ntag21x pin connections of the current board.
ntag21x -t card
	run ntag21x card test.
ntag21x -c halt
	chip halt.
ntag21x -c wake_up
	chip wake up.
ntag21x -c read <page>
	chip read page.page is the read page.
ntag21x -c read_pages <startpage> <stoppage>
	chip read pages.startpage is the start page,stoppage is the stop page.
ntag21x -c read4 <startpage>
	chip read four pages.startpage is the start page.
ntag21x -c write <page>
	chip write page.page is the written page.
ntag21x -c version
	get the version.
ntag21x -c counter
	get the chip read counter.
ntag21x -c signature
	get the chip signature.
ntag21x -c serial_number
	get the chip serial number.
ntag21x -c set -pwd <pwd> <pack>
	set the password.pwd is the password,pack is the checked pack.
ntag21x -c set -dynamic <lock>
	set the dynamic lock.lock is the set lock.
ntag21x -c set -static <lock>
	set the static lock.lock is the set lock.
ntag21x -c set -mirror <mirror> <byte> <page>
	set the chip mirror.mirror is the type and it can be "NO_ASCII", "UID_ASCII", "NFC_COUNTER_ASCII" or "UID_NFC_COUNTER_ASCII".byte is the page byte and it can be 0, 1, 2, 3.page the mirror page.
ntag21x -c set -mode <mode>
	set the mode.mode can be "NORMAL" or "STRONG".
ntag21x -c set -protect <page>
	set the start protect page.page is the start page.
ntag21x -c set -limitation <limit>
	set the authenticate limitation.limit is the set limit and it can be 0 - 7.
ntag21x -c set -access <access> <enable>
	set the access.access can be "READ_PROTECTION", "USER_CONF_PROTECTION","NFC_COUNTER" or "NFC_COUNTER_PASSWORD_PROTECTION".enable is the set bool and it can be 0 or 1.
ntag21x -c authenticate <pwd> <pack>
	password authenticate .pwd is the password,pack is the checked pack.
```
