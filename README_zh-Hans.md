[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver NTAG21X

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ntag21x/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

NTAG213、NTAG215和NTAG216由恩智浦半导体公司开发，并作为标准NFC标签IC与NFC设备或NFC兼容型近距离耦合设备配合，广泛用于大众市场应用，如零售、游戏和消费电子等。NTAG213、NTAG215和NTAG216 (以下统称为NTAG21x)的设计完全符合NFC Forum NFC Forum Type 2标签和ISO/IEC14443 Type A规范。目标应用包括户外和印刷媒体智能广告、SoLoMo应用、产品验证、NFC货架标签、手机配套标签。目标用例包括户外智能广告、产品认证、手机配套标签、Bluetooth®或Wi-Fi配对、电子货架标签和名片。还可对NTAG21x存储器进行分段，以同时部署多个应用。由于NTAG21x标签IC具有高输入电容，因此该产品尤其适用于要求具有小尺寸的应用，而不会降低性能。小型NFC标签可更轻松地嵌入产品标签或电子设备等目标中。NTAG21x的机械和电气规格可满足镶嵌要求，并且可以按照标签制造商的要求而量身定制。

LibDriver NTAG21X是LibDriver推出的NTAG21X全功能驱动，该驱动提供页读取，页写入等功能并且它符合MISRA标准。

本仓库包含子模块，您需要增加“--recursive”选项完整克隆整个工程。

```shell
git clone https://github.com/libdriver/ntag21x.git --recursive
```

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic](#example-basic)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver NTAG21X的源文件。

/interface目录包含了LibDriver NTAG21X与平台无关的RF总线模板。

/test目录包含了LibDriver NTAG21X驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver NTAG21X编程范例。

/doc目录包含了LibDriver NTAG21X离线文档。

/datasheet目录包含了NTAG21X数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

/misra目录包含了LibDriver MISRA代码扫描结果。

### 安装

参考/interface目录下与平台无关的RF总线模板，完成指定平台的RF总线驱动。

将/src目录，您使用平台的接口驱动和您开发的驱动加入工程，如果您想要使用默认的范例驱动，可以将/example目录加入您的工程。

### 使用

您可以参考/example目录下的编程范例完成适合您的驱动，如果您想要使用默认的编程范例，以下是它们的使用方法。

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

### 文档

在线文档: [https://www.libdriver.com/docs/ntag21x/index.html](https://www.libdriver.com/docs/ntag21x/index.html)。

离线文档: /doc/html/index.html。

### 贡献

请参考CONTRIBUTING.md。

### 版权

版权 (c) 2015 - 现在 LibDriver 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com。