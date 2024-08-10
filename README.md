[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver NTAG21X

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ntag21x/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

NTAG 213, NTAG 215, and NTAG 216 have been developed by NXP® Semiconductors as standard NFC tag ICs to be used in mass-market applications such as retail, gaming, and consumer electronics, in combination with NFC devices or NFC-compliant Proximity Coupling Devices. NTAG 213, NTAG 215, and NTAG 216 (from now on, generally called NTAG 21x) are designed to fully comply to NFC Forum Type 2 Tag and ISO/IEC14443 Type A specifications.Target applications include Out-of-Home and print media smart advertisement, SoLoMo applications, product authentication, NFC shelf labels, and mobile companion tags.Target use cases include Out-of-Home smart advertisement, product authentication, mobile companion tags, Bluetooth® or Wi-Fi pairing, electronic shelf labels, and business cards. NTAG 21x memory can also be segmented to implement multiple applications at the same time.Thanks to the high input capacitance, NTAG 21x tag ICs are particularly tailored for applications requiring small footprints, without compromise on performance. Small NFC tags can be more easily embedded into, e.g., product labels or electronic devices.The mechanical and electrical specifications of NTAG 21x are tailored to meet the requirements of inlay and tag manufacturers.

LibDriver NTAG21X is the full function driver of NTAG21X launched by LibDriver.It provides the function of read page, write page and so on. LibDriver is MISRA compliant.

This repository includes submodule and you shold add "--recursive" to clone the whole project.

```shell
git clone https://github.com/libdriver/ntag21x.git --recursive
```

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver NTAG21X source files.

/interface includes LibDriver NTAG21X RF platform independent template.

/test includes LibDriver NTAG21X driver test code and this code can test the chip necessary function simply.

/example includes LibDriver NTAG21X sample code.

/doc includes LibDriver NTAG21X offline document.

/datasheet includes NTAG21X datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

### Install

Reference /interface RF platform independent template and finish your platform RF driver.

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

#### example basic

```C
#include "driver_ntag21x_basic.h"

uint8_t res;
uint8_t i;
uint8_t id[8];
ntag21x_capability_container_t type;
ntag21x_version_t version;
uint32_t cnt;
uint8_t signature[32];
uint8_t number[7];
uint8_t data[16];
uint16_t len;
uint8_t pwd[4];
uint8_t pack[2];

/* init */
res = ntag21x_basic_init();
if (res != 0)
{
    return 1;
}

/* search */
res = ntag21x_basic_search(&type, id, 50);
if (res != 0)
{
    (void)ntag21x_basic_deinit();

    return 1;
}

/* ouput the type */
if (type == NTAG21X_CAPABILITY_CONTAINER_144_BYTE_NTAG213)
{
    ntag21x_interface_debug_print("ntag21x: find ntag213 card.\n");
}
else if (type == NTAG21X_CAPABILITY_CONTAINER_496_BYTE_NTAG215)
{
    ntag21x_interface_debug_print("ntag21x: find ntag215 card.\n");
}
else if (type == NTAG21X_CAPABILITY_CONTAINER_872_BYTE_NTAG216)
{
    ntag21x_interface_debug_print("ntag21x: find ntag216 card.\n");
}
else
{
    ntag21x_interface_debug_print("ntag21x: invalid type.\n");
}
ntag21x_interface_debug_print("ntag21x: id is ");
for (i = 0; i < 8; i++)
{
    ntag21x_interface_debug_print("0x%02X ", id[i]);
}
ntag21x_interface_debug_print("\n");

...
    
/* get the version */
res = ntag21x_basic_get_version(&version);
if (res != 0)
{
    (void)ntag21x_basic_deinit();

    return 1;
}
ntag21x_interface_debug_print("ntag21x: fixed_header is 0x%02X\n", version.fixed_header);
ntag21x_interface_debug_print("ntag21x: vendor_id is 0x%02X\n", version.vendor_id);
ntag21x_interface_debug_print("ntag21x: product_type is 0x%02X\n", version.product_type);
ntag21x_interface_debug_print("ntag21x: product_subtype is 0x%02X\n", version.product_subtype);
ntag21x_interface_debug_print("ntag21x: major_product_version is 0x%02X\n", version.major_product_version);
ntag21x_interface_debug_print("ntag21x: minor_product_version is 0x%02X\n", version.minor_product_version);
ntag21x_interface_debug_print("ntag21x: storage_size is 0x%02X\n", version.storage_size);
ntag21x_interface_debug_print("ntag21x: protocol_type is 0x%02X\n", version.protocol_type);

...

/* read the counter */
res = ntag21x_basic_read_counter(&cnt);
if (res != 0)
{
    (void)ntag21x_basic_deinit();

    return 1;
}
ntag21x_interface_debug_print("read counter %d.\n", cnt);

...

/* read the signature */
res = ntag21x_basic_read_signature(signature);
if (res != 0)
{
    (void)ntag21x_basic_deinit();

    return 1;
}
ntag21x_interface_debug_print("ntag21x: signature is ");
for (i = 0; i < 32; i++)
{
    ntag21x_interface_debug_print("0x%02X ", signature[i]);
}
ntag21x_interface_debug_print("\n");

...

/* get the serial number */
res = ntag21x_basic_get_serial_number(number);
if (res != 0)
{
    (void)ntag21x_basic_deinit();

    return 1;
}
ntag21x_interface_debug_print("ntag21x: serial number is ");
for (i = 0; i < 7; i++)
{
    ntag21x_interface_debug_print("0x%02X ", id[i]);
}
ntag21x_interface_debug_print("\n");

...

/* read data */
res = ntag21x_basic_read(0, data);
if (res != 0)
{
    (void)ntag21x_basic_deinit();

    return 1;
}
ntag21x_interface_debug_print("ntag21x: read page %d: ", 0);
for (i = 0; i < 4; i++)
{
    ntag21x_interface_debug_print("0x%02X ", data[i]);
}
ntag21x_interface_debug_print("\n");

...

/* write data */
res = ntag21x_basic_write(20, data);
if (res != 0)
{
    (void)ntag21x_basic_deinit();

    return 1;
}
ntag21x_interface_debug_print("ntag21x: write page %d: ", 20);
for (i = 0; i < 4; i++)
{
    ntag21x_interface_debug_print("0x%02X ", data[i]);
}
ntag21x_interface_debug_print("\n");

...
    
/* read data */
res = ntag21x_basic_read_four_pages(0, data);
if (res != 0)
{
    (void)ntag21x_basic_deinit();

    return 1;
}
ntag21x_interface_debug_print("ntag21x: read page %d: ", 0);
for (i = 0; i < 16; i++)
{
    ntag21x_interface_debug_print("0x%02X ", data[i]);
}
ntag21x_interface_debug_print("\n");

...

/* read pages */
len = 16;
res = ntag21x_basic_read_pages(0, 0, data, &len);
if (res != 0)
{
    (void)ntag21x_basic_deinit();

    return 1;
}
ntag21x_interface_debug_print("ntag21x: read pages %d - %d: ", 0, 0);
for (i = 0; i < len; i++)
{
    ntag21x_interface_debug_print("0x%02X ", data[i]);
}
ntag21x_interface_debug_print("\n");

...

/* authenticate */
res = ntag21x_basic_authenticate(pwd, pack);
if (res != 0)
{
    (void)ntag21x_basic_deinit();

    return 1;
}

...
    
(void)ntag21x_basic_deinit();

return 0;
```

### Document

Online documents: [https://www.libdriver.com/docs/ntag21x/index.html](https://www.libdriver.com/docs/ntag21x/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please send an e-mail to lishifenging@outlook.com.