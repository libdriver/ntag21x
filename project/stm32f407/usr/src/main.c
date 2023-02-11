/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @file      main.c
 * @brief     main source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2022-06-30
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022/06/30  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_ntag21x_basic.h"
#include "driver_ntag21x_card_test.h"
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "uart.h"
#include "gpio.h"
#include "getopt.h"
#include <math.h>
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];             /**< uart buffer */
volatile uint16_t g_len;        /**< uart buffer length */

/**
 * @brief     ntag21x full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 *             - 1 run failed
 *             - 5 param is invalid
 * @note      none
 */
uint8_t ntag21x(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"access", required_argument, NULL, 1},
        {"addr", required_argument, NULL, 2},
        {"data", required_argument, NULL, 3},
        {"enable", required_argument, NULL, 4},
        {"mirror-byte", required_argument, NULL, 5},
        {"mirror-type", required_argument, NULL, 6},
        {"mirror-page", required_argument, NULL, 7},
        {"mode", required_argument, NULL, 8},
        {"limit", required_argument, NULL, 9},
        {"lock", required_argument, NULL, 10},
        {"lock-type", required_argument, NULL, 11},
        {"pack", required_argument, NULL, 12},
        {"page", required_argument, NULL, 13},
        {"pwd", required_argument, NULL, 14},
        {"start", required_argument, NULL, 15},
        {"stop", required_argument, NULL, 16},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    ntag21x_access_t access = NTAG21X_ACCESS_READ_PROTECTION;
    ntag21x_modulation_mode_t mode = NTAG21X_MODULATION_MODE_STRONG;
    ntag21x_bool_t enable = NTAG21X_BOOL_FALSE;
    uint8_t addr = 0;
    uint8_t page = 10;
    uint8_t start = 0;
    uint8_t stop = 3;
    uint8_t limit = 7;
    uint32_t dat = 0x00000000;
    ntag21x_mirror_byte_t mirror_byte = NTAG21X_MIRROR_BYTE_0;
    ntag21x_mirror_t mirror_type = NTAG21X_MIRROR_NO_ASCII;
    uint8_t mirror_page = 0;
    uint8_t pack[2] = {0x00, 0x00};
    uint8_t pwd[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    uint32_t lock = 0;
    uint8_t lock_type = 0;

    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }

    /* init 0 */
    optind = 0;

    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);

        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");

                break;
            }

            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");

                break;
            }

            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");

                break;
            }

            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);

                break;
            }

            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);

                break;
            }

            /* access */
            case 1 :
            {
                /* set the access */
                if (strcmp("READ_PROTECTION", optarg) == 0)
                {
                    access = NTAG21X_ACCESS_READ_PROTECTION;
                }
                else if (strcmp("USER_CONF_PROTECTION", optarg) == 0)
                {
                    access = NTAG21X_ACCESS_USER_CONF_PROTECTION;
                }
                else if (strcmp("NFC_COUNTER", optarg) == 0)
                {
                    access = NTAG21X_ACCESS_NFC_COUNTER;
                }
                else if (strcmp("NFC_COUNTER_PASSWORD_PROTECTION", optarg) == 0)
                {
                    access = NTAG21X_ACCESS_NFC_COUNTER_PASSWORD_PROTECTION;
                }
                else
                {
                    return 5;
                }

                break;
            }

            /* addr */
            case 2 :
            {
                /* set the addr */
                if (strcmp("0", optarg) == 0)
                {
                    addr = 0;
                }
                else if (strcmp("1", optarg) == 0)
                {
                    addr = 1;
                }
                else if (strcmp("2", optarg) == 0)
                {
                    addr = 2;
                }
                else
                {
                    return 5;
                }

                break;
            }

            /* data */
            case 3 :
            {
                char *p;
                uint16_t l;
                uint16_t i;
                uint64_t hex_data;

                /* set the data */
                l = strlen(optarg);

                /* check the header */
                if (l >= 2)
                {
                    if (strncmp(optarg, "0x", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else if (strncmp(optarg, "0X", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else
                    {
                        p = optarg;
                    }
                }
                else
                {
                    p = optarg;
                }

                /* init 0 */
                hex_data = 0;

                /* loop */
                for (i = 0; i < l; i++)
                {
                    if ((p[i] <= '9') && (p[i] >= '0'))
                    {
                        hex_data += (p[i] - '0') * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'F') && (p[i] >= 'A'))
                    {
                        hex_data += ((p[i] - 'A') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'f') && (p[i] >= 'a'))
                    {
                        hex_data += ((p[i] - 'a') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else
                    {
                        return 5;
                    }
                }
                dat = hex_data & 0xFFFFFFFF;

                break;
            }

            /* enable */
            case 4 :
            {
                /* set the bool */
                if (strcmp("true", optarg) == 0)
                {
                    enable = NTAG21X_BOOL_TRUE;
                }
                else if (strcmp("false", optarg) == 0)
                {
                    enable = NTAG21X_BOOL_FALSE;
                }
                else
                {
                    return 5;
                }

                break;
            }

            /* mirror-byte */
            case 5 :
            {
                /* set mirror byte */
                if (strcmp("0", optarg) == 0)
                {
                    mirror_byte = NTAG21X_MIRROR_BYTE_0;
                }
                else if (strcmp("1", optarg) == 0)
                {
                    mirror_byte = NTAG21X_MIRROR_BYTE_1;
                }
                else if (strcmp("2", optarg) == 0)
                {
                    mirror_byte = NTAG21X_MIRROR_BYTE_2;
                }
                else if (strcmp("3", optarg) == 0)
                {
                    mirror_byte = NTAG21X_MIRROR_BYTE_3;
                }
                else
                {
                    return 5;
                }

                break;
            }

            /* mirror-type */
            case 6 :
            {
                /* set mirror type */
                if (strcmp("NO_ASCII", optarg) == 0)
                {
                    mirror_type = NTAG21X_MIRROR_NO_ASCII;
                }
                else if (strcmp("UID_ASCII", optarg) == 0)
                {
                    mirror_type = NTAG21X_MIRROR_UID_ASCII;
                }
                else if (strcmp("NFC_COUNTER_ASCII", optarg) == 0)
                {
                    mirror_type = NTAG21X_MIRROR_NFC_COUNTER_ASCII;
                }
                else if (strcmp("UID_NFC_COUNTER_ASCII", optarg) == 0)
                {
                    mirror_type = NTAG21X_MIRROR_UID_NFC_COUNTER_ASCII;
                }
                else
                {
                    return 5;
                }

                break;
            }

            /* mirror-page */
            case 7 :
            {
                /* set mirror page */
                mirror_page = atoi(optarg);

                break;
            }

            /* mode */
            case 8 :
            {
                /* set the mode */
                if (strcmp("NORMAL", optarg) == 0)
                {
                    mode = NTAG21X_MODULATION_MODE_NORMAL;
                }
                else if (strcmp("STRONG", optarg) == 0)
                {
                    mode = NTAG21X_MODULATION_MODE_STRONG;
                }
                else
                {
                    return 5;
                }

                break;
            }

            /* limit */
            case 9 :
            {
                /* set the limit */
                limit = atol(optarg);
                if (limit > 7)
                {
                    return 5;
                }

                break;
            }

            /* lock */
            case 10 :
            {
                char *p;
                uint16_t l;
                uint16_t i;
                uint64_t hex_data;

                /* set the data */
                l = strlen(optarg);

                /* check the header */
                if (l >= 2)
                {
                    if (strncmp(optarg, "0x", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else if (strncmp(optarg, "0X", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else
                    {
                        p = optarg;
                    }
                }
                else
                {
                    p = optarg;
                }

                /* init 0 */
                hex_data = 0;

                /* loop */
                for (i = 0; i < l; i++)
                {
                    if ((p[i] <= '9') && (p[i] >= '0'))
                    {
                        hex_data += (p[i] - '0') * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'F') && (p[i] >= 'A'))
                    {
                        hex_data += ((p[i] - 'A') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'f') && (p[i] >= 'a'))
                    {
                        hex_data += ((p[i] - 'a') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else
                    {
                        return 5;
                    }
                }
                lock = hex_data & 0xFFFFFFFF;

                break;
            }

            /* lock-type */
            case 11 :
            {
                /* set the lock type */
                if (strcmp("DYNAMIC", optarg) == 0)
                {
                    lock_type = 0;
                }
                else if (strcmp("STATIC", optarg) == 0)
                {
                    lock_type = 1;
                }
                else
                {
                    return 5;
                }

                break;
            }

            /* pack */
            case 12 :
            {
                char *p;
                uint16_t l;
                uint16_t i;
                uint64_t hex_data;

                /* set the data */
                l = strlen(optarg);

                /* check the header */
                if (l >= 2)
                {
                    if (strncmp(optarg, "0x", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else if (strncmp(optarg, "0X", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else
                    {
                        p = optarg;
                    }
                }
                else
                {
                    p = optarg;
                }

                /* init 0 */
                hex_data = 0;

                /* loop */
                for (i = 0; i < l; i++)
                {
                    if ((p[i] <= '9') && (p[i] >= '0'))
                    {
                        hex_data += (p[i] - '0') * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'F') && (p[i] >= 'A'))
                    {
                        hex_data += ((p[i] - 'A') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'f') && (p[i] >= 'a'))
                    {
                        hex_data += ((p[i] - 'a') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else
                    {
                        return 5;
                    }
                }
                pack[1] = (hex_data >> 0) & 0xFF;
                pack[0] = (hex_data >> 8) & 0xFF;

                break;
            }

            /* page */
            case 13 :
            {
                /* set the page */
                page = atol(optarg);

                break;
            }

            /* pwd */
            case 14 :
            {
                char *p;
                uint16_t l;
                uint16_t i;
                uint64_t hex_data;

                /* set the data */
                l = strlen(optarg);

                /* check the header */
                if (l >= 2)
                {
                    if (strncmp(optarg, "0x", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else if (strncmp(optarg, "0X", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else
                    {
                        p = optarg;
                    }
                }
                else
                {
                    p = optarg;
                }

                /* init 0 */
                hex_data = 0;

                /* loop */
                for (i = 0; i < l; i++)
                {
                    if ((p[i] <= '9') && (p[i] >= '0'))
                    {
                        hex_data += (p[i] - '0') * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'F') && (p[i] >= 'A'))
                    {
                        hex_data += ((p[i] - 'A') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'f') && (p[i] >= 'a'))
                    {
                        hex_data += ((p[i] - 'a') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else
                    {
                        return 5;
                    }
                }
                pwd[3] = (hex_data >> 0) & 0xFF;
                pwd[2] = (hex_data >> 8) & 0xFF;
                pwd[1] = (hex_data >> 16) & 0xFF;
                pwd[0] = (hex_data >> 24) & 0xFF;

                break;
            }

            /* start */
            case 15 :
            {
                /* set the start address */
                start = atol(optarg);

                break;
            }

            /* stop */
            case 16 :
            {
                /* set the stop address */
                stop = atol(optarg);

                break;
            }

            /* the end */
            case -1 :
            {
                break;
            }

            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_card", type) == 0)
    {
        uint8_t res;

        /* run the card test */
        res = ntag21x_card_test();
        if (res != 0)
        {
            return 1;
        }

        return 0;
    }
    else if (strcmp("e_halt", type) == 0)
    {
        uint8_t res;

        /* basic init */
        res = ntag21x_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* halt */
        res = ntag21x_basic_halt();
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* output */
        ntag21x_interface_debug_print("ntag21x: halt.\n");

        /* basic deinit */
        (void)ntag21x_basic_deinit();

        return 0;
    }
    else if (strcmp("e_wake-up", type) == 0)
    {
        uint8_t res;

        /* basic init */
        res = ntag21x_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* wake_up */
        res = ntag21x_basic_wake_up();
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* output */
        ntag21x_interface_debug_print("ntag21x: wake up.\n");

        /* basic deinit */
        (void)ntag21x_basic_deinit();

        return 0;
    }
    else if (strcmp("e_read", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        uint8_t data[4];
        ntag21x_capability_container_t type_s;

        /* basic init */
        res = ntag21x_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = ntag21x_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == NTAG21X_CAPABILITY_CONTAINER_144_BYTE_NTAG213)
        {
            ntag21x_interface_debug_print("ntag21x: find ntag213 card.\n");
        }
        else if (type_s == NTAG21X_CAPABILITY_CONTAINER_496_BYTE_NTAG215)
        {
            ntag21x_interface_debug_print("ntag21x: find ntag215 card.\n");
        }
        else if (type_s == NTAG21X_CAPABILITY_CONTAINER_872_BYTE_NTAG216)
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

        /* read data */
        res = ntag21x_basic_read(page, data);
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* output */
        ntag21x_interface_debug_print("ntag21x: read page %d: ", page);
        for (i = 0; i < 4; i++)
        {
            ntag21x_interface_debug_print("0x%02X ", data[i]);
        }
        ntag21x_interface_debug_print("\n");

        /* basic deinit */
        (void)ntag21x_basic_deinit();

        return 0;
    }
    else if (strcmp("e_read-pages", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        uint8_t data[60];
        uint16_t len;
        ntag21x_capability_container_t type_s;

        /* basic init */
        res = ntag21x_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = ntag21x_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == NTAG21X_CAPABILITY_CONTAINER_144_BYTE_NTAG213)
        {
            ntag21x_interface_debug_print("ntag21x: find ntag213 card.\n");
        }
        else if (type_s == NTAG21X_CAPABILITY_CONTAINER_496_BYTE_NTAG215)
        {
            ntag21x_interface_debug_print("ntag21x: find ntag215 card.\n");
        }
        else if (type_s == NTAG21X_CAPABILITY_CONTAINER_872_BYTE_NTAG216)
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

        /* read pages */
        len = 60;
        res = ntag21x_basic_read_pages(start, stop, data, &len);
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* output */
        ntag21x_interface_debug_print("ntag21x: read pages %d - %d: ", start, stop);
        for (i = 0; i < len; i++)
        {
            ntag21x_interface_debug_print("0x%02X ", data[i]);
        }
        ntag21x_interface_debug_print("\n");

        /* basic deinit */
        (void)ntag21x_basic_deinit();

        return 0;
    }
    else if (strcmp("e_read4", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        uint8_t data[16];
        ntag21x_capability_container_t type_s;

        /* basic init */
        res = ntag21x_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = ntag21x_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == NTAG21X_CAPABILITY_CONTAINER_144_BYTE_NTAG213)
        {
            ntag21x_interface_debug_print("ntag21x: find ntag213 card.\n");
        }
        else if (type_s == NTAG21X_CAPABILITY_CONTAINER_496_BYTE_NTAG215)
        {
            ntag21x_interface_debug_print("ntag21x: find ntag215 card.\n");
        }
        else if (type_s == NTAG21X_CAPABILITY_CONTAINER_872_BYTE_NTAG216)
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

        /* read data */
        res = ntag21x_basic_read_four_pages(page, data);
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* output */
        ntag21x_interface_debug_print("ntag21x: read page %d: ", page);
        for (i = 0; i < 16; i++)
        {
            ntag21x_interface_debug_print("0x%02X ", data[i]);
        }
        ntag21x_interface_debug_print("\n");

        /* basic deinit */
        (void)ntag21x_basic_deinit();

        return 0;
    }
    else if (strcmp("e_write", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        uint8_t data[4];
        ntag21x_capability_container_t type_s;

        /* basic init */
        res = ntag21x_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = ntag21x_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == NTAG21X_CAPABILITY_CONTAINER_144_BYTE_NTAG213)
        {
            ntag21x_interface_debug_print("ntag21x: find ntag213 card.\n");
        }
        else if (type_s == NTAG21X_CAPABILITY_CONTAINER_496_BYTE_NTAG215)
        {
            ntag21x_interface_debug_print("ntag21x: find ntag215 card.\n");
        }
        else if (type_s == NTAG21X_CAPABILITY_CONTAINER_872_BYTE_NTAG216)
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

        /* write data */
        data[0]  = (dat >> 24) & 0xFF;
        data[1]  = (dat >> 16) & 0xFF;
        data[2]  = (dat >> 8) & 0xFF;
        data[3]  = (dat >> 0) & 0xFF;
        res = ntag21x_basic_write(page, data);
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* output */
        ntag21x_interface_debug_print("ntag21x: write page %d: ", page);
        for (i = 0; i < 4; i++)
        {
            ntag21x_interface_debug_print("0x%02X ", data[i]);
        }
        ntag21x_interface_debug_print("\n");

        /* basic deinit */
        (void)ntag21x_basic_deinit();

        return 0;
    }
    else if (strcmp("e_version", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        ntag21x_capability_container_t type_s;
        ntag21x_version_t version;

        /* basic init */
        res = ntag21x_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = ntag21x_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == NTAG21X_CAPABILITY_CONTAINER_144_BYTE_NTAG213)
        {
            ntag21x_interface_debug_print("ntag21x: find ntag213 card.\n");
        }
        else if (type_s == NTAG21X_CAPABILITY_CONTAINER_496_BYTE_NTAG215)
        {
            ntag21x_interface_debug_print("ntag21x: find ntag215 card.\n");
        }
        else if (type_s == NTAG21X_CAPABILITY_CONTAINER_872_BYTE_NTAG216)
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

        /* get the version */
        res = ntag21x_basic_get_version(&version);
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* output */
        ntag21x_interface_debug_print("ntag21x: fixed_header is 0x%02X\n", version.fixed_header);
        ntag21x_interface_debug_print("ntag21x: vendor_id is 0x%02X\n", version.vendor_id);
        ntag21x_interface_debug_print("ntag21x: product_type is 0x%02X\n", version.product_type);
        ntag21x_interface_debug_print("ntag21x: product_subtype is 0x%02X\n", version.product_subtype);
        ntag21x_interface_debug_print("ntag21x: major_product_version is 0x%02X\n", version.major_product_version);
        ntag21x_interface_debug_print("ntag21x: minor_product_version is 0x%02X\n", version.minor_product_version);
        ntag21x_interface_debug_print("ntag21x: storage_size is 0x%02X\n", version.storage_size);
        ntag21x_interface_debug_print("ntag21x: protocol_type is 0x%02X\n", version.protocol_type);

        /* basic deinit */
        (void)ntag21x_basic_deinit();

        return 0;
    }
    else if (strcmp("e_counter", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        ntag21x_capability_container_t type_s;
        uint32_t cnt;

        /* basic init */
        res = ntag21x_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = ntag21x_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == NTAG21X_CAPABILITY_CONTAINER_144_BYTE_NTAG213)
        {
            ntag21x_interface_debug_print("ntag21x: find ntag213 card.\n");
        }
        else if (type_s == NTAG21X_CAPABILITY_CONTAINER_496_BYTE_NTAG215)
        {
            ntag21x_interface_debug_print("ntag21x: find ntag215 card.\n");
        }
        else if (type_s == NTAG21X_CAPABILITY_CONTAINER_872_BYTE_NTAG216)
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

        /* read the counter */
        res = ntag21x_basic_read_counter(&cnt);
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* output */
        ntag21x_interface_debug_print("read counter %d with addr %d.\n", cnt, addr);

        /* basic deinit */
        (void)ntag21x_basic_deinit();

        return 0;
    }
    else if (strcmp("e_signature", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        ntag21x_capability_container_t type_s;
        uint8_t signature[32];

        /* basic init */
        res = ntag21x_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = ntag21x_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == NTAG21X_CAPABILITY_CONTAINER_144_BYTE_NTAG213)
        {
            ntag21x_interface_debug_print("ntag21x: find ntag213 card.\n");
        }
        else if (type_s == NTAG21X_CAPABILITY_CONTAINER_496_BYTE_NTAG215)
        {
            ntag21x_interface_debug_print("ntag21x: find ntag215 card.\n");
        }
        else if (type_s == NTAG21X_CAPABILITY_CONTAINER_872_BYTE_NTAG216)
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

        /* read the signature */
        res = ntag21x_basic_read_signature(signature);
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* output */
        ntag21x_interface_debug_print("ntag21x: signature is ");
        for (i = 0; i < 32; i++)
        {
            ntag21x_interface_debug_print("0x%02X ", signature[i]);
        }
        ntag21x_interface_debug_print("\n");

        /* basic deinit */
        (void)ntag21x_basic_deinit();

        return 0;
    }
    else if (strcmp("e_serial", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        ntag21x_capability_container_t type_s;
        uint8_t number[7];

        /* basic init */
        res = ntag21x_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = ntag21x_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == NTAG21X_CAPABILITY_CONTAINER_144_BYTE_NTAG213)
        {
            ntag21x_interface_debug_print("ntag21x: find ntag213 card.\n");
        }
        else if (type_s == NTAG21X_CAPABILITY_CONTAINER_496_BYTE_NTAG215)
        {
            ntag21x_interface_debug_print("ntag21x: find ntag215 card.\n");
        }
        else if (type_s == NTAG21X_CAPABILITY_CONTAINER_872_BYTE_NTAG216)
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

        /* get the serial number */
        res = ntag21x_basic_get_serial_number(number);
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* output */
        ntag21x_interface_debug_print("ntag21x: serial number is ");
        for (i = 0; i < 7; i++)
        {
            ntag21x_interface_debug_print("0x%02X ", number[i]);
        }
        ntag21x_interface_debug_print("\n");

        /* basic deinit */
        (void)ntag21x_basic_deinit();

        return 0;
    }
    else if (strcmp("e_set-pwd", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        ntag21x_capability_container_t type_s;

        /* basic init */
        res = ntag21x_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = ntag21x_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == NTAG21X_CAPABILITY_CONTAINER_144_BYTE_NTAG213)
        {
            ntag21x_interface_debug_print("ntag21x: find ntag213 card.\n");
        }
        else if (type_s == NTAG21X_CAPABILITY_CONTAINER_496_BYTE_NTAG215)
        {
            ntag21x_interface_debug_print("ntag21x: find ntag215 card.\n");
        }
        else if (type_s == NTAG21X_CAPABILITY_CONTAINER_872_BYTE_NTAG216)
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

        /* set the password pack */
        res = ntag21x_basic_set_password_pack(pwd, pack);
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* output */
        ntag21x_interface_debug_print("ntag21x: set password 0x%02X 0x%02X 0x%02X 0x%02X ok.\n",
                                      pwd[0], pwd[1], pwd[2], pwd[3]);
        ntag21x_interface_debug_print("ntag21x: set pack 0x%02X 0x%02X ok.\n", pack[0], pack[1]);

        /* basic deinit */
        (void)ntag21x_basic_deinit();

        return 0;
    }
    else if (strcmp("e_lock", type) == 0)
    {
        if (lock_type == 0)
        {
            uint8_t res;
            uint8_t i;
            uint8_t id[8];
            ntag21x_capability_container_t type_s;
            uint8_t lock_buf[3];

            /* basic init */
            res = ntag21x_basic_init();
            if (res != 0)
            {
                return 1;
            }

            /* search */
            res = ntag21x_basic_search(&type_s, id, 50);
            if (res != 0)
            {
                (void)ntag21x_basic_deinit();

                return 1;
            }

            /* output type */
            if (type_s == NTAG21X_CAPABILITY_CONTAINER_144_BYTE_NTAG213)
            {
                ntag21x_interface_debug_print("ntag21x: find ntag213 card.\n");
            }
            else if (type_s == NTAG21X_CAPABILITY_CONTAINER_496_BYTE_NTAG215)
            {
                ntag21x_interface_debug_print("ntag21x: find ntag215 card.\n");
            }
            else if (type_s == NTAG21X_CAPABILITY_CONTAINER_872_BYTE_NTAG216)
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

            /* set the dynamic lock */
            lock_buf[0] = (lock >> 16) & 0xFF;
            lock_buf[1] = (lock >> 8) & 0xFF;
            lock_buf[2] = (lock >> 0) & 0xFF;
            res = ntag21x_basic_set_dynamic_lock(lock_buf);
            if (res != 0)
            {
                (void)ntag21x_basic_deinit();

                return 1;
            }

            /* output */
            ntag21x_interface_debug_print("ntag21x: dynamic lock: ");
            for (i = 0; i < 3; i++)
            {
                ntag21x_interface_debug_print("0x%02X ", lock_buf[i]);
            }
            ntag21x_interface_debug_print("\n");

            /* basic deinit */
            (void)ntag21x_basic_deinit();

            return 0;
        }
        else
        {
            uint8_t res;
            uint8_t i;
            uint8_t id[8];
            uint8_t lock_buf[2];
            ntag21x_capability_container_t type_s;

            /* basic init */
            res = ntag21x_basic_init();
            if (res != 0)
            {
                return 1;
            }

            /* search */
            res = ntag21x_basic_search(&type_s, id, 50);
            if (res != 0)
            {
                (void)ntag21x_basic_deinit();

                return 1;
            }

            /* output type */
            if (type_s == NTAG21X_CAPABILITY_CONTAINER_144_BYTE_NTAG213)
            {
                ntag21x_interface_debug_print("ntag21x: find ntag213 card.\n");
            }
            else if (type_s == NTAG21X_CAPABILITY_CONTAINER_496_BYTE_NTAG215)
            {
                ntag21x_interface_debug_print("ntag21x: find ntag215 card.\n");
            }
            else if (type_s == NTAG21X_CAPABILITY_CONTAINER_872_BYTE_NTAG216)
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

            /* set the static lock */
            lock_buf[0] = (lock >> 8) & 0xFF;
            lock_buf[1] = (lock >> 0) & 0xFF;
            res = ntag21x_basic_set_static_lock(lock_buf);
            if (res != 0)
            {
                (void)ntag21x_basic_deinit();

                return 1;
            }

            /* output */
            ntag21x_interface_debug_print("ntag21x: static lock: ");
            for (i = 0; i < 2; i++)
            {
                ntag21x_interface_debug_print("0x%02X ", lock_buf[i]);
            }
            ntag21x_interface_debug_print("\n");

            /* basic deinit */
            (void)ntag21x_basic_deinit();

            return 0;
        }
    }
    else if (strcmp("e_mirror", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        ntag21x_capability_container_t type_s;

        /* basic init */
        res = ntag21x_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = ntag21x_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == NTAG21X_CAPABILITY_CONTAINER_144_BYTE_NTAG213)
        {
            ntag21x_interface_debug_print("ntag21x: find ntag213 card.\n");
        }
        else if (type_s == NTAG21X_CAPABILITY_CONTAINER_496_BYTE_NTAG215)
        {
            ntag21x_interface_debug_print("ntag21x: find ntag215 card.\n");
        }
        else if (type_s == NTAG21X_CAPABILITY_CONTAINER_872_BYTE_NTAG216)
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

        /* set the mirror */
        res = ntag21x_basic_set_mirror(mirror_type, mirror_byte, mirror_page);
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* basic deinit */
        (void)ntag21x_basic_deinit();

        return 0;
    }
    else if (strcmp("e_set-mode", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        ntag21x_capability_container_t type_s;

        /* basic init */
        res = ntag21x_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = ntag21x_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == NTAG21X_CAPABILITY_CONTAINER_144_BYTE_NTAG213)
        {
            ntag21x_interface_debug_print("ntag21x: find ntag213 card.\n");
        }
        else if (type_s == NTAG21X_CAPABILITY_CONTAINER_496_BYTE_NTAG215)
        {
            ntag21x_interface_debug_print("ntag21x: find ntag215 card.\n");
        }
        else if (type_s == NTAG21X_CAPABILITY_CONTAINER_872_BYTE_NTAG216)
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

        /* set the modulation mode */
        res = ntag21x_basic_set_modulation_mode(mode);
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* basic deinit */
        (void)ntag21x_basic_deinit();

        return 0;
    }
    else if (strcmp("e_set-protect", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        ntag21x_capability_container_t type_s;

        /* basic init */
        res = ntag21x_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = ntag21x_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == NTAG21X_CAPABILITY_CONTAINER_144_BYTE_NTAG213)
        {
            ntag21x_interface_debug_print("ntag21x: find ntag213 card.\n");
        }
        else if (type_s == NTAG21X_CAPABILITY_CONTAINER_496_BYTE_NTAG215)
        {
            ntag21x_interface_debug_print("ntag21x: find ntag215 card.\n");
        }
        else if (type_s == NTAG21X_CAPABILITY_CONTAINER_872_BYTE_NTAG216)
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

        /* set protect start page */
        res = ntag21x_basic_set_protect_start_page(page);
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* output */
        ntag21x_interface_debug_print("ntag21x: set protect start page %d.\n", page);

        (void)ntag21x_basic_deinit();

        return 0;
    }
    else if (strcmp("e_set-limit", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        ntag21x_capability_container_t type_s;

        /* basic init */
        res = ntag21x_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = ntag21x_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == NTAG21X_CAPABILITY_CONTAINER_144_BYTE_NTAG213)
        {
            ntag21x_interface_debug_print("ntag21x: find ntag213 card.\n");
        }
        else if (type_s == NTAG21X_CAPABILITY_CONTAINER_496_BYTE_NTAG215)
        {
            ntag21x_interface_debug_print("ntag21x: find ntag215 card.\n");
        }
        else if (type_s == NTAG21X_CAPABILITY_CONTAINER_872_BYTE_NTAG216)
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

        /* set authenticate limitation */
        res = ntag21x_basic_set_authenticate_limitation(limit);
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }
        ntag21x_interface_debug_print("ntag21x: set authenticate limitation %d.\n", limit);

        /* basic deinit */
        (void)ntag21x_basic_deinit();

        return 0;
    }
    else if (strcmp("e_set-access", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        ntag21x_capability_container_t type_s;

        /* basic init */
        res = ntag21x_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = ntag21x_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == NTAG21X_CAPABILITY_CONTAINER_144_BYTE_NTAG213)
        {
            ntag21x_interface_debug_print("ntag21x: find ntag213 card.\n");
        }
        else if (type_s == NTAG21X_CAPABILITY_CONTAINER_496_BYTE_NTAG215)
        {
            ntag21x_interface_debug_print("ntag21x: find ntag215 card.\n");
        }
        else if (type_s == NTAG21X_CAPABILITY_CONTAINER_872_BYTE_NTAG216)
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

        /* set the access */
        res = ntag21x_basic_set_access(access, enable);
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* basic deinit */
        (void)ntag21x_basic_deinit();

        return 0;
    }
    else if (strcmp("e_authenticate", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        ntag21x_capability_container_t type_s;

        /* basic init */
        res = ntag21x_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = ntag21x_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == NTAG21X_CAPABILITY_CONTAINER_144_BYTE_NTAG213)
        {
            ntag21x_interface_debug_print("ntag21x: find ntag213 card.\n");
        }
        else if (type_s == NTAG21X_CAPABILITY_CONTAINER_496_BYTE_NTAG215)
        {
            ntag21x_interface_debug_print("ntag21x: find ntag215 card.\n");
        }
        else if (type_s == NTAG21X_CAPABILITY_CONTAINER_872_BYTE_NTAG216)
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

        /* authenticate */
        res = ntag21x_basic_authenticate(pwd, pack);
        if (res != 0)
        {
            (void)ntag21x_basic_deinit();

            return 1;
        }

        /* output */
        ntag21x_interface_debug_print("ntag21x: authenticate password 0x%02X 0x%02X 0x%02X 0x%02X ok.\n",
                                      pwd[0], pwd[1], pwd[2], pwd[3]);
        ntag21x_interface_debug_print("ntag21x: authenticate pack 0x%02X 0x%02X ok.\n", pack[0], pack[1]);

        /* basic deinit */
        (void)ntag21x_basic_deinit();

        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        ntag21x_interface_debug_print("Usage:\n");
        ntag21x_interface_debug_print("  ntag21x (-i | --information)\n");
        ntag21x_interface_debug_print("  ntag21x (-h | --help)\n");
        ntag21x_interface_debug_print("  ntag21x (-p | --port)\n");
        ntag21x_interface_debug_print("  ntag21x (-t card | --test=card)\n");
        ntag21x_interface_debug_print("  ntag21x (-e halt | --example=halt)\n");
        ntag21x_interface_debug_print("  ntag21x (-e wake-up | --example=wake-up)\n");
        ntag21x_interface_debug_print("  ntag21x (-e read | --example=read) [--page=<addr>]\n");
        ntag21x_interface_debug_print("  ntag21x (-e read-pages | --example=read-pages) [--start=<taddr>] [--stop=<paddr>]\n");
        ntag21x_interface_debug_print("  ntag21x (-e read4 | --example=read4) [--page=<addr>]\n");
        ntag21x_interface_debug_print("  ntag21x (-e write | --example=write) [--page=<addr>] [--data=<hex>]\n");
        ntag21x_interface_debug_print("  ntag21x (-e version | --example=version)\n");
        ntag21x_interface_debug_print("  ntag21x (-e counter | --example=counter) [--addr=<0 | 1 | 2>]\n");
        ntag21x_interface_debug_print("  ntag21x (-e signature | --example=signature)\n");
        ntag21x_interface_debug_print("  ntag21x (-e serial | --example=serial)\n");
        ntag21x_interface_debug_print("  ntag21x (-e set-pwd | --example=set-pwd) [--pwd=<password>] [--pack=<pak>]\n");
        ntag21x_interface_debug_print("  ntag21x (-e lock | --example=lock) [--lock-type=<DYNAMIC | STATIC>] [--lock=<hex>]\n");
        ntag21x_interface_debug_print("  ntag21x (-e mirror | --example=mirror)\n");
        ntag21x_interface_debug_print("          [--mirror-type=<NO_ASCII | UID_ASCII | NFC_COUNTER_ASCII | UID_NFC_COUNTER_ASCII>]\n");
        ntag21x_interface_debug_print("          [--mirror-byte=<0 | 1 | 2 | 3>] [--mirror-page=<addr>]\n");
        ntag21x_interface_debug_print("  ntag21x (-e set-mode | --example=set-mode) [--mode=<NORMAL | STRONG>]\n");
        ntag21x_interface_debug_print("  ntag21x (-e set-protect | --example=set-protect) [--page=<addr>]\n");
        ntag21x_interface_debug_print("  ntag21x (-e set-limit | --example=set-limit) [--limit=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7>]\n");
        ntag21x_interface_debug_print("  ntag21x (-e set-access | --example=set-access)\n");
        ntag21x_interface_debug_print("          [--access=<READ_PROTECTION | USER_CONF_PROTECTION | NFC_COUNTER | NFC_COUNTER_PASSWORD_PROTECTION>]\n");
        ntag21x_interface_debug_print("          [--enable=<true | false>]\n");
        ntag21x_interface_debug_print("  ntag21x (-e authenticate | --example=authenticate) [--pwd=<password>] [--pack=<pak>]\n");
        ntag21x_interface_debug_print("\n");
        ntag21x_interface_debug_print("Options:\n");
        ntag21x_interface_debug_print("      --access=<READ_PROTECTION | USER_CONF_PROTECTION | NFC_COUNTER | NFC_COUNTER_PASSWORD_PROTECTION>\n");
        ntag21x_interface_debug_print("                                 Set access mode.([default: READ_PROTECTION])\n");
        ntag21x_interface_debug_print("      --addr=<0 | 1 | 2>         Set counter address.([default: 0])\n");
        ntag21x_interface_debug_print("      --data=<hex>               Set opt write data and it is hexadecimal.([default: 0x00000000])\n");
        ntag21x_interface_debug_print("  -e <halt | wake-up | read | read-pages | read4 | write | version | counter | signature | serial | set-pwd\n");
        ntag21x_interface_debug_print("     | lock | mirror | set-mode | set-protect | set-limit | set-access | authenticate>, --example=<halt\n");
        ntag21x_interface_debug_print("     | wake-up | read | read-pages | read4 | write | version | counter | signature | serial | set-pwd\n");
        ntag21x_interface_debug_print("     | lock | mirror | set-mode | set-protect | set-limit | set-access | authenticate>\n");
        ntag21x_interface_debug_print("                                 Run the driver example.\n");
        ntag21x_interface_debug_print("      --enable=<true | false>    Set access bool.([default: false])\n");
        ntag21x_interface_debug_print("  -h, --help                     Show the help.\n");
        ntag21x_interface_debug_print("  -i, --information              Show the chip information.\n");
        ntag21x_interface_debug_print("      --mirror-byte=<0 | 1 | 2 | 3>\n");
        ntag21x_interface_debug_print("                                 Set the mirror byte.([default: 0])\n");
        ntag21x_interface_debug_print("      --mirror-type=<NO_ASCII | UID_ASCII | NFC_COUNTER_ASCII | UID_NFC_COUNTER_ASCII>\n");
        ntag21x_interface_debug_print("                                 Set the mirror type.([default: NO_ASCII])\n");
        ntag21x_interface_debug_print("      --mirror-page=<addr>       Set the mirror page.([default: 0])\n");
        ntag21x_interface_debug_print("      --mode=<NORMAL | STRONG>   Set chip mode.([default: STRONG])\n");
        ntag21x_interface_debug_print("      --limit=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7>\n");
        ntag21x_interface_debug_print("                                 Set the limit times.([default: 7])\n");
        ntag21x_interface_debug_print("      --lock=<hex>               Set the lock data.([default: 0x0000000000])\n");
        ntag21x_interface_debug_print("      --lock-type=<DYNAMIC | STATIC>\n");
        ntag21x_interface_debug_print("                                 Set the lock type.([default: DYNAMIC])\n");
        ntag21x_interface_debug_print("  -p, --port                     Display the pin connections of the current board.\n");
        ntag21x_interface_debug_print("      --pack=<pak>               Set the pack authentication and it is hexadecimal.([default: 0x0000])\n");
        ntag21x_interface_debug_print("      --page=<addr>              Set read or write page address.([default: 10])\n");
        ntag21x_interface_debug_print("      --pwd=<password>           Set the password authentication and it is hexadecimal.([default: 0xFFFFFFFF])\n");
        ntag21x_interface_debug_print("      --start=<taddr>            Set read pages start address.([default: 0])\n");
        ntag21x_interface_debug_print("      --stop=<paddr>             Set read pages stop address.([default: 3])\n");
        ntag21x_interface_debug_print("  -t <card>, --test=<card>       Run the driver test.\n");

        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        ntag21x_info_t info;

        /* print ntag21x info */
        ntag21x_info(&info);
        ntag21x_interface_debug_print("ntag21x: chip is %s.\n", info.chip_name);
        ntag21x_interface_debug_print("ntag21x: manufacturer is %s.\n", info.manufacturer_name);
        ntag21x_interface_debug_print("ntag21x: interface is %s.\n", info.interface);
        ntag21x_interface_debug_print("ntag21x: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        ntag21x_interface_debug_print("ntag21x: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        ntag21x_interface_debug_print("ntag21x: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        ntag21x_interface_debug_print("ntag21x: max current is %0.2fmA.\n", info.max_current_ma);
        ntag21x_interface_debug_print("ntag21x: max temperature is %0.1fC.\n", info.temperature_max);
        ntag21x_interface_debug_print("ntag21x: min temperature is %0.1fC.\n", info.temperature_min);

        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        ntag21x_interface_debug_print("ntag21x: SPI interface SCK connected to GPIOA PIN5.\n");
        ntag21x_interface_debug_print("ntag21x: SPI interface MISO connected to GPIOA PIN6.\n");
        ntag21x_interface_debug_print("ntag21x: SPI interface MOSI connected to GPIOA PIN7.\n");
        ntag21x_interface_debug_print("ntag21x: SPI interface CS connected to GPIOA PIN4.\n");
        ntag21x_interface_debug_print("ntag21x: IIC interface SCL connected to GPIOB PIN8.\n");
        ntag21x_interface_debug_print("ntag21x: IIC interface SDA connected to GPIOB PIN9.\n");
        ntag21x_interface_debug_print("ntag21x: UART interface TX connected to GPIOA PIN2.\n");
        ntag21x_interface_debug_print("ntag21x: UART interface RX connected to GPIOA PIN3.\n");
        ntag21x_interface_debug_print("ntag21x: INT connected to GPIOB PIN0.\n");
        ntag21x_interface_debug_print("ntag21x: RESET connected to GPIOA PIN8.\n");

        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    uint8_t res;

    /* stm32f407 clock init and hal init */
    clock_init();

    /* delay init */
    delay_init();

    /* uart init */
    uart_init(115200);

    /* shell init && register ntag21x function */
    shell_init();
    shell_register("ntag21x", ntag21x);
    uart_print("ntag21x: welcome to libdriver ntag21x.\n");

    while (1)
    {
        /* read uart */
        g_len = uart_read(g_buf, 256);
        if (g_len != 0)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart_print("ntag21x: run failed.\n");
            }
            else if (res == 2)
            {
                uart_print("ntag21x: unknown command.\n");
            }
            else if (res == 3)
            {
                uart_print("ntag21x: length is too long.\n");
            }
            else if (res == 4)
            {
                uart_print("ntag21x: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart_print("ntag21x: param is invalid.\n");
            }
            else
            {
                uart_print("ntag21x: unknown status code.\n");
            }
            uart_flush();
        }
        delay_ms(100);
    }
}
