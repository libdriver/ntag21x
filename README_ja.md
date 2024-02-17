[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver NTAG21X

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ntag21x/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

NTAG 213、NTAG 215、およびNTAG 216は、NXP®Semiconductorsによって、小売、ゲーム、家庭用電化製品などのマスマーケットアプリケーションで使用される標準のNFCタグICとして、NFCデバイスまたはNFC準拠の近接結合と組み合わせて開発されました。デバイス。 NTAG 213、NTAG 215、およびNTAG 216（以降、一般にNTAG 21xと呼ばれる）は、NFCフォーラムタイプ2タグおよびISO /IEC14443タイプA仕様に完全に準拠するように設計されています。対象アプリケーションには、家庭外および印刷メディアスマートが含まれます。広告、SoLoMoアプリケーション、製品認証、NFCシェルフラベル、およびモバイルコンパニオンタグ。対象となるユースケースには、家庭外スマート広告、製品認証、モバイルコンパニオンタグ、Bluetooth®またはWi-Fiペアリング、電子シェルフラベル、およびビジネスが含まれます。カード。 NTAG 21xメモリは、同時に複数のアプリケーションを実装するためにセグメント化することもできます。高い入力容量のおかげで、NTAG 21xタグICは、パフォーマンスを犠牲にすることなく、小さなフットプリントを必要とするアプリケーション向けに特に調整されています。小さなNFCタグは、製品ラベルや電子機器などに簡単に埋め込むことができます。NTAG21xの機械的および電気的仕様は、インレイおよびタグメーカーの要件を満たすように調整されています。

LibDriver NTAG21Xは、LibDriverによって起動されたNTAG21Xの全機能ドライバーであり、ページの読み取り、ページの書き込みなどの機能を提供します。 LibDriverはMISRAに準拠しています。

このリポジトリにはサブモジュールが含まれており、「--recursive」を追加してプロジェクト全体を複製する必要があります。

```shell
git clone https://github.com/libdriver/ntag21x.git --recursive
```

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver NTAG21Xのソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver NTAG21X用のプラットフォームに依存しないRFバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver NTAG21Xドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver NTAG21Xプログラミング例が含まれています。

/ docディレクトリには、LibDriver NTAG21Xオフラインドキュメントが含まれています。

/ datasheetディレクトリには、NTAG21Xデータシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないRFバステンプレートを参照して、指定したプラットフォームのRFバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

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

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/ntag21x/index.html](https://www.libdriver.com/docs/ntag21x/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。