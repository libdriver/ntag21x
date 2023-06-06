### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

IIC Pin: SCL/SDA PB8/PB9.

SPI Pin: SCK/MISO/MOSI/CS  PA5/PA6/PA7/PA4.

GPIO Pin: INT PB0.

RESET Pin: RESET PA8.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. NTAG21X

#### 3.1 Command Instruction

1. Show ntag21x chip and driver information.

   ```shell
   ntag21x (-i | --information)
   ```

2. Show ntag21x help.

   ```shell
   ntag21x (-h | --help)
   ```

3. Show ntag21x pin connections of the current board.

   ```shell
   ntag21x (-p | --port)
   ```

4. Run ntag21x card test.

   ```shell
   ntag21x (-t card | --test=card)
   ```

5. Run chip halt function.

   ```shell
   ntag21x (-e halt | --example=halt)
   ```

6. Run chip wake up function.

   ```shell
   ntag21x (-e wake-up | --example=wake-up)
   ```

7. Run chip read page function, addr is the read page address.

   ```shell
   ntag21x (-e read | --example=read) [--page=<addr>]
   ```

8. Run chip read pages function, taddr is the start page address, paddr is the stop page address.

   ```shell
   ntag21x (-e read-pages | --example=read-pages) [--start=<taddr>] [--stop=<paddr>]
   ```

9. Run chip read four pages function, addr is the start page address.

   ```shell
   ntag21x (-e read4 | --example=read4) [--page=<addr>]
   ```

10. Run chip write page function, addr is the write page address, hex is the write data.

    ```shell
    ntag21x (-e write | --example=write) [--page=<addr>] [--data=<hex>]
    ```

11. Run get version function.

    ```shell
    ntag21x (-e version | --example=version)
    ```

12. Run get chip read counter function.

    ```shell
    ntag21x (-e counter | --example=counter) [--addr=<0 | 1 | 2>]
    ```

13. Run get chip signature function.

    ```shell
    ntag21x (-e signature | --example=signature)
    ```

14. Run get chip serial number function.

    ```shell
    ntag21x (-e serial | --example=serial)
    ```

15. Run set password function, password is the password, pak is the checked pack.

    ```shell
    ntag21x (-e set-pwd | --example=set-pwd) [--pwd=<password>] [--pack=<pak>]
    ```

16. Run set dynamic lock function, hex is the set lock.

    ```shell
    ntag21x (-e lock | --example=lock) [--lock-type=<DYNAMIC | STATIC>] [--lock=<hex>]
    ```

17. Run set chip mirror function, addr is the mirror page address.

    ```shell
    ntag21x (-e mirror | --example=mirror) [--mirror-type=<NO_ASCII | UID_ASCII | NFC_COUNTER_ASCII | UID_NFC_COUNTER_ASCII>] [--mirror-byte=<0 | 1 | 2 | 3>] [--mirror-page=<addr>]
    ```

18. Run set mode function.

    ```shell
    ntag21x (-e set-mode | --example=set-mode) [--mode=<NORMAL | STRONG>]
    ```

19. Run set start protect page function, addr is the start page address.

    ```shell
    ntag21x (-e set-protect | --example=set-protect) [--page=<addr>]
    ```

20. Run set authenticate limitation function.

    ```shell
    ntag21x (-e set-limit | --example=set-limit) [--limit=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7>]
    ```

21. Run set access function.

    ```shell
    ntag21x (-e set-access | --example=set-access) [--access=<READ_PROTECTION | USER_CONF_PROTECTION | NFC_COUNTER | NFC_COUNTER_PASSWORD_PROTECTION>] [--enable=<true | false>]
    ```

22. Run password authenticate function, password is the password, pak is the checked pack.

    ```shell
    ntag21x (-e authenticate | --example=authenticate) [--pwd=<password>] [--pack=<pak>]
    ```

#### 3.2 Command Example

```shell
ntag21x -i

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
ntag21x -p

ntag21x: SPI interface SCK connected to GPIOA PIN5.
ntag21x: SPI interface MISO connected to GPIOA PIN6.
ntag21x: SPI interface MOSI connected to GPIOA PIN7.
ntag21x: SPI interface CS connected to GPIOA PIN4.
ntag21x: IIC interface SCL connected to GPIOB PIN8.
ntag21x: IIC interface SDA connected to GPIOB PIN9.
ntag21x: UART interface TX connected to GPIOA PIN2.
ntag21x: UART interface RX connected to GPIOA PIN3.
ntag21x: INT connected to GPIOB PIN0.
ntag21x: RESET connected to GPIOA PIN8.
```

```shell
ntag21x -t card

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
ntag21x: id1 is 0x88 0x04 0xFA 0x66.
ntag21x: id2 is 0x12 0x28 0x73 0x80.
ntag21x: get capability container ntag213.
ntag21x: serial number is 0x04 0xFA 0x66 0x12 0x28 0x73 0x80 
ntag21x: signature is 0x67 0x97 0x25 0xAD 0x10 0x71 0x46 0xA0 0x7A 0xE1 0x15 0xEB 0xE5 0x27 0x8D 0x2F 0x75 0x26 0x30 0xBF 0xD1 0xF1 0xB3 0x0E 0xAC 0xCD 0xE0 0xDF 0xAA 0xD9 0x19 0xF0 
ntag21x: read four pages from page 0.
0x04 0xFA 0x66 0x10 0x12 0x28 0x73 0x80 0xC9 0x48 0x00 0x00 0xE1 0x10 0x12 0x00 
ntag21x: read page 3.
0xE1 0x10 0x12 0x00 
ntag21x: fast read page from page 0 - page 5.
0x04 0xFA 0x66 0x10 0x12 0x28 0x73 0x80 0xC9 0x48 0x00 0x00 0xE1 0x10 0x12 0x00 0x01 0x03 0xA0 0x0C 0x34 0x03 0x00 0xFE 0x0B 0x00 0x08 0x00 0x00 0x00 0x00 0xA7 
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
ntag21x: storage_size is 0x0F
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
ntag21x: read counter 23.
ntag21x: finish card test.
```

```shell
ntag21x -e halt

ntag21x: halt.
```

```shell
ntag21x -e wake-up

ntag21x: wake up.
```

```shell
ntag21x -e read --page=0

ntag21x: find ntag213 card.
ntag21x: id is 0x88 0x04 0xFA 0x66 0x12 0x28 0x73 0x80 
ntag21x: read page 0: 0x04 0xFA 0x66 0x10 
```

```shell
ntag21x -e read-pages --start=0 --stop=3

ntag21x: find ntag213 card.
ntag21x: id is 0x88 0x04 0xFA 0x66 0x12 0x28 0x73 0x80 
ntag21x: read pages 0 - 3: 0x04 0xFA 0x66 0x10 0x12 0x28 0x73 0x80 0xC9 0x48 0x00 0x00 0xE1 0x10 0x12 0x00 
```

```shell
ntag21x -e read4 --page=0

ntag21x: find ntag213 card.
ntag21x: id is 0x88 0x04 0xFA 0x66 0x12 0x28 0x73 0x80 
ntag21x: read page 0: 0x04 0xFA 0x66 0x10 0x12 0x28 0x73 0x80 0xC9 0x48 0x00 0x00 0xE1 0x10 0x12 0x00 
```

```shell
ntag21x -e write --page=20 --data=12345678

ntag21x: find ntag213 card.
ntag21x: id is 0x88 0x04 0xFA 0x66 0x12 0x28 0x73 0x80 
ntag21x: write page 20: 0x12 0x34 0x56 0x78 
```

```shell
ntag21x -e version

ntag21x: find ntag213 card.
ntag21x: id is 0x88 0x04 0xFA 0x66 0x12 0x28 0x73 0x80 
ntag21x: fixed_header is 0x00
ntag21x: vendor_id is 0x04
ntag21x: product_type is 0x04
ntag21x: product_subtype is 0x02
ntag21x: major_product_version is 0x01
ntag21x: minor_product_version is 0x00
ntag21x: storage_size is 0x0F
ntag21x: protocol_type is 0x03
```

```shell
ntag21x -e counter

ntag21x: find ntag213 card.
ntag21x: id is 0x88 0x04 0xFA 0x66 0x12 0x28 0x73 0x80 
read counter 29 with addr 0.
```

```shell
ntag21x -e signature

ntag21x: find ntag213 card.
ntag21x: id is 0x88 0x04 0xFA 0x66 0x12 0x28 0x73 0x80 
ntag21x: signature is 0x67 0x97 0x25 0xAD 0x10 0x71 0x46 0xA0 0x7A 0xE1 0x15 0xEB 0xE5 0x27 0x8D 0x2F 0x75 0x26 0x30 0xBF 0xD1 0xF1 0xB3 0x0E 0xAC 0xCD 0xE0 0xDF 0xAA 0xD9 0x19 0xF0 
```

```shell
ntag21x -e serial

ntag21x: find ntag213 card.
ntag21x: id is 0x88 0x04 0xFA 0x66 0x12 0x28 0x73 0x80 
ntag21x: serial number is 0x04 0xFA 0x66 0x12 0x28 0x73 0x80 
```

```shell
ntag21x -e set-pwd --pwd=0xFFFFFFFF --pack=0x0000

ntag21x: find ntag213 card.
ntag21x: id is 0x88 0x04 0xFA 0x66 0x12 0x28 0x73 0x80 
ntag21x: set password 0xFF 0xFF 0xFF 0xFF ok.
ntag21x: set pack 0x00 0x00 ok.
```

```shell
ntag21x -e lock --lock-type=DYNAMIC --lock=0x000000

ntag21x: find ntag213 card.
ntag21x: id is 0x88 0x04 0xFA 0x66 0x12 0x28 0x73 0x80 
ntag21x: dynamic lock: 0x00 0x00 0x00 
```

```shell
ntag21x -e lock --lock-type=STATIC --lock=0x0000

ntag21x: find ntag213 card.
ntag21x: id is 0x88 0x04 0xFA 0x66 0x12 0x28 0x73 0x80 
ntag21x: static lock: 0x00 0x00 
```

```shell
ntag21x -e mirror --mirror-type=NO_ASCII --mirror-byte=0 --mirror-page=0

ntag21x: find ntag213 card.
ntag21x: id is 0x88 0x04 0xFA 0x66 0x12 0x28 0x73 0x80 
```

```shell
ntag21x -e set-mode --mode=STRONG

ntag21x: find ntag213 card.
ntag21x: id is 0x88 0x04 0xFA 0x66 0x12 0x28 0x73 0x80 
```

```shell
ntag21x -e set-protect --page=255

ntag21x: find ntag213 card.
ntag21x: id is 0x88 0x04 0xFA 0x66 0x12 0x28 0x73 0x80 
ntag21x: set protect start page 255.
```

```shell
ntag21x -e set-limit --limit=7

ntag21x: find ntag213 card.
ntag21x: id is 0x88 0x04 0xFA 0x66 0x12 0x28 0x73 0x80 
ntag21x: set authenticate limitation 7.
```

```shell
ntag21x -e set-access --access=NFC_COUNTER --enable=true

ntag21x: find ntag213 card.
ntag21x: id is 0x88 0x04 0xFA 0x66 0x12 0x28 0x73 0x80 
```

```shell
ntag21x -e authenticate --pwd=0xFFFFFFFF --pack=0x0000

ntag21x: find ntag213 card.
ntag21x: id is 0x88 0x04 0xFA 0x66 0x12 0x28 0x73 0x80 
ntag21x: authenticate password 0xFF 0xFF 0xFF 0xFF ok.
ntag21x: authenticate pack 0x00 0x00 ok.
```


```shell
ntag21x -h

Usage:
  ntag21x (-i | --information)
  ntag21x (-h | --help)
  ntag21x (-p | --port)
  ntag21x (-t card | --test=card)
  ntag21x (-e halt | --example=halt)
  ntag21x (-e wake-up | --example=wake-up)
  ntag21x (-e read | --example=read) [--page=<addr>]
  ntag21x (-e read-pages | --example=read-pages) [--start=<taddr>] [--stop=<paddr>]
  ntag21x (-e read4 | --example=read4) [--page=<addr>]
  ntag21x (-e write | --example=write) [--page=<addr>] [--data=<hex>]
  ntag21x (-e version | --example=version)
  ntag21x (-e counter | --example=counter) [--addr=<0 | 1 | 2>]
  ntag21x (-e signature | --example=signature)
  ntag21x (-e serial | --example=serial)
  ntag21x (-e set-pwd | --example=set-pwd) [--pwd=<password>] [--pack=<pak>]
  ntag21x (-e lock | --example=lock) [--lock-type=<DYNAMIC | STATIC>] [--lock=<hex>]
  ntag21x (-e mirror | --example=mirror)
          [--mirror-type=<NO_ASCII | UID_ASCII | NFC_COUNTER_ASCII | UID_NFC_COUNTER_ASCII>]
          [--mirror-byte=<0 | 1 | 2 | 3>] [--mirror-page=<addr>]
  ntag21x (-e set-mode | --example=set-mode) [--mode=<NORMAL | STRONG>]
  ntag21x (-e set-protect | --example=set-protect) [--page=<addr>]
  ntag21x (-e set-limit | --example=set-limit) [--limit=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7>]
  ntag21x (-e set-access | --example=set-access)
          [--access=<READ_PROTECTION | USER_CONF_PROTECTION | NFC_COUNTER | NFC_COUNTER_PASSWORD_PROTECTION>]
          [--enable=<true | false>]
  ntag21x (-e authenticate | --example=authenticate) [--pwd=<password>] [--pack=<pak>]

Options:
      --access=<READ_PROTECTION | USER_CONF_PROTECTION | NFC_COUNTER | NFC_COUNTER_PASSWORD_PROTECTION>
                                 Set access mode.([default: READ_PROTECTION])
      --addr=<0 | 1 | 2>         Set counter address.([default: 0])
      --data=<hex>               Set opt write data and it is hexadecimal.([default: 0x00000000])
  -e <halt | wake-up | read | read-pages | read4 | write | version | counter | signature | serial | set-pwd
     | lock | mirror | set-mode | set-protect | set-limit | set-access | authenticate>, --example=<halt
     | wake-up | read | read-pages | read4 | write | version | counter | signature | serial | set-pwd
     | lock | mirror | set-mode | set-protect | set-limit | set-access | authenticate>
                                 Run the driver example.
      --enable=<true | false>    Set access bool.([default: false])
  -h, --help                     Show the help.
  -i, --information              Show the chip information.
      --mirror-byte=<0 | 1 | 2 | 3>
                                 Set the mirror byte.([default: 0])
      --mirror-type=<NO_ASCII | UID_ASCII | NFC_COUNTER_ASCII | UID_NFC_COUNTER_ASCII>
                                 Set the mirror type.([default: NO_ASCII])
      --mirror-page=<addr>       Set the mirror page.([default: 0])
      --mode=<NORMAL | STRONG>   Set chip mode.([default: STRONG])
      --limit=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7>
                                 Set the limit times.([default: 7])
      --lock=<hex>               Set the lock data.([default: 0x0000000000])
      --lock-type=<DYNAMIC | STATIC>
                                 Set the lock type.([default: DYNAMIC])
  -p, --port                     Display the pin connections of the current board.
      --pack=<pak>               Set the pack authentication and it is hexadecimal.([default: 0x0000])
      --page=<addr>              Set read or write page address.([default: 10])
      --pwd=<password>           Set the password authentication and it is hexadecimal.([default: 0xFFFFFFFF])
      --start=<taddr>            Set read pages start address.([default: 0])
      --stop=<paddr>             Set read pages stop address.([default: 3])
  -t <card>, --test=<card>       Run the driver test.
```
