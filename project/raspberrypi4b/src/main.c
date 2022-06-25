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
 * @date      2022-05-31
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/05/31  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_ntag21x_basic.h"
#include "driver_ntag21x_card_test.h"
#include <stdlib.h>

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
    if (argc == 1)
    {
        goto help;
    }
    else if (argc == 2)
    {
        if (strcmp("-i", argv[1]) == 0)
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
        else if (strcmp("-p", argv[1]) == 0)
        {
            /* print pin connection */
            ntag21x_interface_debug_print("ntag21x: SPI interface SCK connected to GPIO11(BCM).\n");
            ntag21x_interface_debug_print("ntag21x: SPI interface MISO connected to GPIO9(BCM).\n");
            ntag21x_interface_debug_print("ntag21x: SPI interface MOSI connected to GPIO10(BCM).\n");
            ntag21x_interface_debug_print("ntag21x: SPI interface CS connected to GPIO8(BCM).\n");
            ntag21x_interface_debug_print("ntag21x: IIC interface SCL connected to GPIO3(BCM).\n");
            ntag21x_interface_debug_print("ntag21x: IIC interface SDA connected to GPIO2(BCM).\n");
            ntag21x_interface_debug_print("ntag21x: UART interface TX connected to GPIO14(BCM).\n");
            ntag21x_interface_debug_print("ntag21x: UART interface RX connected to GPIO15(BCM).\n");
            ntag21x_interface_debug_print("ntag21x: INT connected to GPIO17(BCM).\n");
            ntag21x_interface_debug_print("ntag21x: RESET connected to GPIO26(BCM).\n");
            
            return 0;
        }
        else if (strcmp("-h", argv[1]) == 0)
        {
            /* show ntag21x help */
            help:
            
            ntag21x_interface_debug_print("ntag21x -i\n\tshow ntag21x chip and driver information.\n");
            ntag21x_interface_debug_print("ntag21x -h\n\tshow ntag21x help.\n");
            ntag21x_interface_debug_print("ntag21x -p\n\tshow ntag21x pin connections of the current board.\n");
            ntag21x_interface_debug_print("ntag21x -t card\n\trun ntag21x card test.\n");
            ntag21x_interface_debug_print("ntag21x -c halt\n\tchip halt.\n");
            ntag21x_interface_debug_print("ntag21x -c wake_up\n\tchip wake up.\n");
            ntag21x_interface_debug_print("ntag21x -c read <page>\n\tchip read page.page is the read page.\n");
            ntag21x_interface_debug_print("ntag21x -c read_pages <startpage> <stoppage>\n\tchip read pages.startpage is the start page,stoppage is the stop page.\n");
            ntag21x_interface_debug_print("ntag21x -c read4 <startpage>\n\tchip read four pages.startpage is the start page.\n");
            ntag21x_interface_debug_print("ntag21x -c write <page>\n\tchip write page.page is the written page.\n");
            ntag21x_interface_debug_print("ntag21x -c version\n\tget the version.\n");
            ntag21x_interface_debug_print("ntag21x -c counter\n\tget the chip read counter.\n");
            ntag21x_interface_debug_print("ntag21x -c signature\n\tget the chip signature.\n");
            ntag21x_interface_debug_print("ntag21x -c serial_number\n\tget the chip serial number.\n");
            ntag21x_interface_debug_print("ntag21x -c set -pwd <pwd> <pack>\n\tset the password.pwd is the password,pack is the checked pack.\n");
            ntag21x_interface_debug_print("ntag21x -c set -dynamic <lock>\n\tset the dynamic lock.lock is the set lock.\n");
            ntag21x_interface_debug_print("ntag21x -c set -static <lock>\n\tset the static lock.lock is the set lock.\n");
            ntag21x_interface_debug_print("ntag21x -c set -mirror <mirror> <byte> <page>\n\tset the chip mirror.mirror is the type and "
                                          "it can be \"NO_ASCII\", \"UID_ASCII\", \"NFC_COUNTER_ASCII\" or \"UID_NFC_COUNTER_ASCII\"");
            ntag21x_interface_debug_print(".byte is the page byte and it can be 0, 1, 2, 3.page the mirror page.\n");
            ntag21x_interface_debug_print("ntag21x -c set -mode <mode>\n\tset the mode.mode can be \"NORMAL\" or \"STRONG\".\n");
            ntag21x_interface_debug_print("ntag21x -c set -protect <page>\n\tset the start protect page.page is the start page.\n");
            ntag21x_interface_debug_print("ntag21x -c set -limitation <limit>\n\tset the authenticate limitation.limit is the set limit and it can be 0 - 7.\n");
            ntag21x_interface_debug_print("ntag21x -c set -access <access> <enable>\n\tset the access.access can be \"READ_PROTECTION\", \"USER_CONF_PROTECTION\","
                                          "\"NFC_COUNTER\" or \"NFC_COUNTER_PASSWORD_PROTECTION\".enable is the set bool and it can be 0 or 1.\n");
            ntag21x_interface_debug_print("ntag21x -c authenticate <pwd> <pack>\n\tpassword authenticate .pwd is the password,pack is the checked pack.\n");
            
            return 0;
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 3)
    {
        if (strcmp("-t", argv[1]) == 0)
        {
            if (strcmp("card", argv[2]) == 0)
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
            else
            {
                return 5;
            }
        }
        else if (strcmp("-c", argv[1]) == 0)
        {
            if (strcmp("halt", argv[2]) == 0)
            {
                uint8_t res;
                
                /* init */
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
                ntag21x_interface_debug_print("ntag21x: halt.\n");
                
                (void)ntag21x_basic_deinit();
                
                return 0;
            }
            else if (strcmp("wake_up", argv[2]) == 0)
            {
                uint8_t res;
                
                /* init */
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
                ntag21x_interface_debug_print("ntag21x: wake up.\n");
                
                (void)ntag21x_basic_deinit();
                
                return 0;
            }
            else if (strcmp("version", argv[2]) == 0)
            {
                uint8_t res;
                uint8_t i;
                uint8_t id[8];
                ntag21x_capability_container_t type;
                ntag21x_version_t version;
                
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
                
                (void)ntag21x_basic_deinit();
                
                return 0;
            }
            else if (strcmp("counter", argv[2]) == 0)
            {
                uint8_t res;
                uint8_t i;
                uint8_t id[8];
                ntag21x_capability_container_t type;
                uint32_t cnt;
                
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
                
                /* read the counter */
                res = ntag21x_basic_read_counter(&cnt);
                if (res != 0)
                {
                    (void)ntag21x_basic_deinit();
                    
                    return 1;
                }
                ntag21x_interface_debug_print("read counter %d.\n", cnt);
                
                (void)ntag21x_basic_deinit();
                
                return 0;
            }
            else if (strcmp("signature", argv[2]) == 0)
            {
                uint8_t res;
                uint8_t i;
                uint8_t id[8];
                ntag21x_capability_container_t type;
                uint8_t signature[32];
                
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
                
                (void)ntag21x_basic_deinit();
                
                return 0;
            }
            else if (strcmp("serial_number", argv[2]) == 0)
            {
                uint8_t res;
                uint8_t i;
                uint8_t id[8];
                ntag21x_capability_container_t type;
                uint8_t number[7];
                
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
                
                (void)ntag21x_basic_deinit();
                
                return 0;
            }
            else
            {
                return 5;
            }
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 4)
    {
        if (strcmp("-c", argv[1]) == 0)
        {
            if (strcmp("read", argv[2]) == 0)
            {
                uint8_t res;
                uint8_t i;
                uint8_t id[8];
                uint8_t data[4];
                ntag21x_capability_container_t type;
                
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
                
                /* read data */
                res = ntag21x_basic_read((uint8_t)atoi(argv[3]), data);
                if (res != 0)
                {
                    (void)ntag21x_basic_deinit();
                    
                    return 1;
                }
                ntag21x_interface_debug_print("ntag21x: read page %d: ", atoi(argv[3]));
                for (i = 0; i < 4; i++)
                {
                    ntag21x_interface_debug_print("0x%02X ", data[i]);
                }
                ntag21x_interface_debug_print("\n");
                
                (void)ntag21x_basic_deinit();
                
                return 0;
            }
            else if (strcmp("read4", argv[2]) == 0)
            {
                uint8_t res;
                uint8_t i;
                uint8_t id[8];
                uint8_t data[16];
                ntag21x_capability_container_t type;
                
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
                
                /* read data */
                res = ntag21x_basic_read_four_pages((uint8_t)atoi(argv[3]), data);
                if (res != 0)
                {
                    (void)ntag21x_basic_deinit();
                    
                    return 1;
                }
                ntag21x_interface_debug_print("ntag21x: read page %d: ", atoi(argv[3]));
                for (i = 0; i < 16; i++)
                {
                    ntag21x_interface_debug_print("0x%02X ", data[i]);
                }
                ntag21x_interface_debug_print("\n");
                
                (void)ntag21x_basic_deinit();
                
                return 0;
            }
            else
            {
                return 5;
            }
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 5)
    {
        if (strcmp("-c", argv[1]) == 0)
        {
            if (strcmp("write", argv[2]) == 0)
            {
                uint8_t res;
                uint8_t i;
                uint32_t size;
                uint8_t id[8];
                uint8_t data[4];
                ntag21x_capability_container_t type;
                
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
                
                /* get the data */
                size = (uint8_t)strlen(argv[4]);
                for (i = 0; i < size; i += 2)
                {
                    if (('A' <= argv[4][i]) && (argv[4][i] <= 'Z'))
                    {
                        data[i / 2] = argv[4][i] - 'A' + 10;
                        data[i / 2] *= 16;
                    }
                    else if (('a' <= argv[4][i]) && (argv[4][i] <= 'z'))
                    {
                        data[i / 2] = argv[4][i] - 'a' + 10;
                        data[i / 2] *= 16;
                    }
                    else if (('0' <= argv[4][i]) && (argv[4][i] <= '9'))
                    {
                        data[i / 2] = argv[4][i] - '0';
                        data[i / 2] *= 16;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    if (('A' <= argv[4][i + 1]) && (argv[4][i + 1] <= 'Z'))
                    {
                        data[i / 2 ] += argv[4][i + 1] - 'A' + 10;
                    }
                    else if (('a' <= argv[4][i + 1]) && (argv[4][i + 1] <= 'z'))
                    {
                        data[i / 2 ] += argv[4][i + 1] - 'a' + 10;
                    }
                    else if (('0' <= argv[4][i + 1]) && (argv[4][i + 1] <= '9'))
                    {
                        data[i / 2 ] += argv[4][i + 1] - '0';
                    }
                    else
                    {
                        return 5;
                    }
                }

                /* write data */
                res = ntag21x_basic_write((uint8_t)atoi(argv[3]), data);
                if (res != 0)
                {
                    (void)ntag21x_basic_deinit();
                    
                    return 1;
                }
                ntag21x_interface_debug_print("ntag21x: write page %d: ", atoi(argv[3]));
                for (i = 0; i < 4; i++)
                {
                    ntag21x_interface_debug_print("0x%02X ", data[i]);
                }
                ntag21x_interface_debug_print("\n");
                
                (void)ntag21x_basic_deinit();
                
                return 0;
            }
            else if (strcmp("read_pages", argv[2]) == 0)
            {
                uint8_t res;
                uint8_t i;
                uint8_t id[8];
                uint8_t data[60];
                uint16_t len;
                ntag21x_capability_container_t type;
                
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
                
                /* read pages */
                len = 60;
                res = ntag21x_basic_read_pages((uint8_t)atoi(argv[3]), (uint8_t)atoi(argv[4]), data, &len);
                if (res != 0)
                {
                    (void)ntag21x_basic_deinit();
                    
                    return 1;
                }
                ntag21x_interface_debug_print("ntag21x: read pages %d - %d: ", atoi(argv[3]), atoi(argv[4]));
                for (i = 0; i < len; i++)
                {
                    ntag21x_interface_debug_print("0x%02X ", data[i]);
                }
                ntag21x_interface_debug_print("\n");
                
                (void)ntag21x_basic_deinit();
                
                return 0;
            }
            else if (strcmp("set", argv[2]) == 0)
            {
                if (strcmp("-dynamic", argv[3]) == 0)
                {
                    uint8_t res;
                    uint8_t i;
                    uint32_t size;
                    uint8_t id[8];
                    uint8_t lock[6];
                    ntag21x_capability_container_t type;
                    
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
                    
                    /* get the data */
                    size = (uint8_t)strlen(argv[4]);
                    for (i = 0; i < size; i += 2)
                    {
                        if (('A' <= argv[4][i]) && (argv[4][i] <= 'Z'))
                        {
                            lock[i / 2] = argv[4][i] - 'A' + 10;
                            lock[i / 2] *= 16;
                        }
                        else if (('a' <= argv[4][i]) && (argv[4][i] <= 'z'))
                        {
                            lock[i / 2] = argv[4][i] - 'a' + 10;
                            lock[i / 2] *= 16;
                        }
                        else if (('0' <= argv[4][i]) && (argv[4][i] <= '9'))
                        {
                            lock[i / 2] = argv[4][i] - '0';
                            lock[i / 2] *= 16;
                        }
                        else
                        {
                            return 5;
                        }
                        
                        if (('A' <= argv[4][i + 1]) && (argv[4][i + 1] <= 'Z'))
                        {
                            lock[i / 2 ] += argv[4][i + 1] - 'A' + 10;
                        }
                        else if (('a' <= argv[4][i + 1]) && (argv[4][i + 1] <= 'z'))
                        {
                            lock[i / 2 ] += argv[4][i + 1] - 'a' + 10;
                        }
                        else if (('0' <= argv[4][i + 1]) && (argv[4][i + 1] <= '9'))
                        {
                            lock[i / 2 ] += argv[4][i + 1] - '0';
                        }
                        else
                        {
                            return 5;
                        }
                    }
                    
                    /* set the dynamic lock */
                    res = ntag21x_basic_set_dynamic_lock(lock);
                    if (res != 0)
                    {
                        (void)ntag21x_basic_deinit();
                        
                        return 1;
                    }
                    
                    ntag21x_interface_debug_print("ntag21x: dynamic lock: ");
                    for (i = 0; i < 3; i++)
                    {
                        ntag21x_interface_debug_print("0x%02X ", lock[i]);
                    }
                    ntag21x_interface_debug_print("\n");
                    
                    (void)ntag21x_basic_deinit();
                    
                    return 0;
                }
                else if (strcmp("-static", argv[3]) == 0)
                {
                    uint8_t res;
                    uint8_t i;
                    uint32_t size;
                    uint8_t id[8];
                    uint8_t lock[4];
                    ntag21x_capability_container_t type;
                    
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
                    
                    /* get the data */
                    size = (uint8_t)strlen(argv[4]);
                    for (i = 0; i < size; i += 2)
                    {
                        if (('A' <= argv[4][i]) && (argv[4][i] <= 'Z'))
                        {
                            lock[i / 2] = argv[4][i] - 'A' + 10;
                            lock[i / 2] *= 16;
                        }
                        else if (('a' <= argv[4][i]) && (argv[4][i] <= 'z'))
                        {
                            lock[i / 2] = argv[4][i] - 'a' + 10;
                            lock[i / 2] *= 16;
                        }
                        else if (('0' <= argv[4][i]) && (argv[4][i] <= '9'))
                        {
                            lock[i / 2] = argv[4][i] - '0';
                            lock[i / 2] *= 16;
                        }
                        else
                        {
                            return 5;
                        }
                        
                        if (('A' <= argv[4][i + 1]) && (argv[4][i + 1] <= 'Z'))
                        {
                            lock[i / 2 ] += argv[4][i + 1] - 'A' + 10;
                        }
                        else if (('a' <= argv[4][i + 1]) && (argv[4][i + 1] <= 'z'))
                        {
                            lock[i / 2 ] += argv[4][i + 1] - 'a' + 10;
                        }
                        else if (('0' <= argv[4][i + 1]) && (argv[4][i + 1] <= '9'))
                        {
                            lock[i / 2 ] += argv[4][i + 1] - '0';
                        }
                        else
                        {
                            return 5;
                        }
                    }
                    
                    /* set the static lock */
                    res = ntag21x_basic_set_static_lock(lock);
                    if (res != 0)
                    {
                        (void)ntag21x_basic_deinit();
                        
                        return 1;
                    }
                    
                    ntag21x_interface_debug_print("ntag21x: static lock: ");
                    for (i = 0; i < 2; i++)
                    {
                        ntag21x_interface_debug_print("0x%02X ", lock[i]);
                    }
                    ntag21x_interface_debug_print("\n");
                    
                    (void)ntag21x_basic_deinit();
                    
                    return 0;
                }
                else if (strcmp("-mode", argv[3]) == 0)
                {
                    uint8_t res;
                    uint8_t i;
                    uint8_t id[8];
                    ntag21x_modulation_mode_t mode;
                    ntag21x_capability_container_t type;
                    
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
                    
                    /* get the mode */
                    if (strcmp("NORMAL", argv[4]) == 0)
                    {
                        ntag21x_interface_debug_print("ntag21x: set the normal mode.\n");
                        mode = NTAG21X_MODULATION_MODE_NORMAL;
                    }
                    else if (strcmp("STRONG", argv[4]) == 0)
                    {
                        ntag21x_interface_debug_print("ntag21x: set the strong mode.\n");
                        mode = NTAG21X_MODULATION_MODE_STRONG;
                    }
                    else
                    {
                        (void)ntag21x_basic_deinit();
                        
                        return 5;
                    }

                    /* set the static lock */
                    res = ntag21x_basic_set_modulation_mode(mode);
                    if (res != 0)
                    {
                        (void)ntag21x_basic_deinit();
                        
                        return 1;
                    }
                    
                    (void)ntag21x_basic_deinit();
                    
                    return 0;
                }
                else if (strcmp("-protect", argv[3]) == 0)
                {
                    uint8_t res;
                    uint8_t i;
                    uint8_t id[8];
                    ntag21x_capability_container_t type;
                    
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
                    

                    /* set protect start page */
                    res = ntag21x_basic_set_protect_start_page((uint8_t)atoi(argv[4]));
                    if (res != 0)
                    {
                        (void)ntag21x_basic_deinit();
                        
                        return 1;
                    }
                    ntag21x_interface_debug_print("ntag21x: set protect start page %d.\n", atoi(argv[4]));
                    
                    (void)ntag21x_basic_deinit();
                    
                    return 0;
                }
                else if (strcmp("-limitation", argv[3]) == 0)
                {
                    uint8_t res;
                    uint8_t i;
                    uint8_t id[8];
                    ntag21x_capability_container_t type;
                    
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
                    

                    /* set authenticate limitation */
                    res = ntag21x_basic_set_authenticate_limitation((uint8_t)atoi(argv[4]));
                    if (res != 0)
                    {
                        (void)ntag21x_basic_deinit();
                        
                        return 1;
                    }
                    ntag21x_interface_debug_print("ntag21x: set authenticate limitation %d.\n", atoi(argv[4]));
                    
                    (void)ntag21x_basic_deinit();
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            if (strcmp("authenticate", argv[2]) == 0)
            {
                uint8_t res;
                uint8_t i;
                uint8_t id[8];
                uint8_t pwd[4];
                uint8_t pack[2];
                uint32_t size;
                ntag21x_capability_container_t type;
                
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
                
                /* get the pwd */
                size = (uint8_t)strlen(argv[3]);
                for (i = 0; i < size; i += 2)
                {
                    if (('A' <= argv[3][i]) && (argv[3][i] <= 'Z'))
                    {
                        pwd[i / 2] = argv[3][i] - 'A' + 10;
                        pwd[i / 2] *= 16;
                    }
                    else if (('a' <= argv[3][i]) && (argv[3][i] <= 'z'))
                    {
                        pwd[i / 2] = argv[3][i] - 'a' + 10;
                        pwd[i / 2] *= 16;
                    }
                    else if (('0' <= argv[3][i]) && (argv[3][i] <= '9'))
                    {
                        pwd[i / 2] = argv[3][i] - '0';
                        pwd[i / 2] *= 16;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    if (('A' <= argv[3][i + 1]) && (argv[3][i + 1] <= 'Z'))
                    {
                        pwd[i / 2 ] += argv[3][i + 1] - 'A' + 10;
                    }
                    else if (('a' <= argv[3][i + 1]) && (argv[3][i + 1] <= 'z'))
                    {
                        pwd[i / 2 ] += argv[3][i + 1] - 'a' + 10;
                    }
                    else if (('0' <= argv[3][i + 1]) && (argv[3][i + 1] <= '9'))
                    {
                        pwd[i / 2 ] += argv[3][i + 1] - '0';
                    }
                    else
                    {
                        return 5;
                    }
                }
                
                /* get the pack */
                size = (uint8_t)strlen(argv[4]);
                for (i = 0; i < size; i += 2)
                {
                    if (('A' <= argv[4][i]) && (argv[4][i] <= 'Z'))
                    {
                        pack[i / 2] = argv[4][i] - 'A' + 10;
                        pack[i / 2] *= 16;
                    }
                    else if (('a' <= argv[4][i]) && (argv[4][i] <= 'z'))
                    {
                        pack[i / 2] = argv[4][i] - 'a' + 10;
                        pack[i / 2] *= 16;
                    }
                    else if (('0' <= argv[4][i]) && (argv[4][i] <= '9'))
                    {
                        pack[i / 2] = argv[4][i] - '0';
                        pack[i / 2] *= 16;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    if (('A' <= argv[4][i + 1]) && (argv[4][i + 1] <= 'Z'))
                    {
                        pack[i / 2 ] += argv[4][i + 1] - 'A' + 10;
                    }
                    else if (('a' <= argv[4][i + 1]) && (argv[4][i + 1] <= 'z'))
                    {
                        pack[i / 2 ] += argv[4][i + 1] - 'a' + 10;
                    }
                    else if (('0' <= argv[4][i + 1]) && (argv[4][i + 1] <= '9'))
                    {
                        pack[i / 2 ] += argv[4][i + 1] - '0';
                    }
                    else
                    {
                        return 5;
                    }
                }
                
                /* authenticate */
                res = ntag21x_basic_authenticate(pwd, pack);
                if (res != 0)
                {
                    (void)ntag21x_basic_deinit();
                    
                    return 1;
                }
                
                ntag21x_interface_debug_print("ntag21x: authenticate password 0x%02X 0x%02X 0x%02X 0x%02X ok.\n",
                                              pwd[0], pwd[1], pwd[2], pwd[3]);
                ntag21x_interface_debug_print("ntag21x: authenticate pack 0x%02X 0x%02X ok.\n", pack[0], pack[1]);
                
                (void)ntag21x_basic_deinit();
                
                return 0;
            }
            else
            {
                return 5;
            }
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 6)
    {
        if (strcmp("-c", argv[1]) == 0)
        {
            if (strcmp("set", argv[2]) == 0)
            {
                if (strcmp("-access", argv[3]) == 0)
                {
                    uint8_t res;
                    uint8_t i;
                    uint8_t id[8];
                    ntag21x_capability_container_t type;
                    ntag21x_access_t access_param;
                    ntag21x_bool_t enable;
                    
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
                    
                    if (strcmp("READ_PROTECTION", argv[4]) == 0)
                    {
                        access_param = NTAG21X_ACCESS_READ_PROTECTION;
                        ntag21x_interface_debug_print("ntag21x: set access read protection ");
                    }
                    else if (strcmp("USER_CONF_PROTECTION", argv[4]) == 0)
                    {
                        access_param = NTAG21X_ACCESS_USER_CONF_PROTECTION;
                        ntag21x_interface_debug_print("ntag21x: set access user configuration protection ");
                    }
                    else if (strcmp("NFC_COUNTER", argv[4]) == 0)
                    {
                        access_param = NTAG21X_ACCESS_NFC_COUNTER;
                        ntag21x_interface_debug_print("ntag21x: set access nfc counter protection ");
                    }
                    else if (strcmp("NFC_COUNTER_PASSWORD_PROTECTION", argv[4]) == 0)
                    {
                        access_param = NTAG21X_ACCESS_NFC_COUNTER_PASSWORD_PROTECTION;
                        ntag21x_interface_debug_print("ntag21x: set access nfc counter password protection ");
                    }
                    else
                    {
                        (void)ntag21x_basic_deinit();
                        
                        return 5;
                    }
                    
                    if (strcmp("0", argv[5]) == 0)
                    {
                        enable = NTAG21X_BOOL_FALSE;
                        ntag21x_interface_debug_print("false.\n");
                    }
                    else if (strcmp("1", argv[5]) == 0)
                    {
                        enable = NTAG21X_BOOL_TRUE;
                        ntag21x_interface_debug_print("true.\n");
                    }
                    else
                    {
                        (void)ntag21x_basic_deinit();
                        
                        return 5;
                    }
                    
                    /* set the access */
                    res = ntag21x_basic_set_access(access_param, enable);
                    if (res != 0)
                    {
                        (void)ntag21x_basic_deinit();
                        
                        return 1;
                    }
                    
                    (void)ntag21x_basic_deinit();
                    
                    return 0;
                }
                else if (strcmp("-pwd", argv[3]) == 0)
                {
                    uint8_t res;
                    uint8_t i;
                    uint32_t size;
                    uint8_t id[8];
                    uint8_t pwd[4];
                    uint8_t pack[2];
                    ntag21x_capability_container_t type;
                    
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
                    
                    /* get the pwd */
                    size = (uint8_t)strlen(argv[4]);
                    for (i = 0; i < size; i += 2)
                    {
                        if (('A' <= argv[4][i]) && (argv[4][i] <= 'Z'))
                        {
                            pwd[i / 2] = argv[4][i] - 'A' + 10;
                            pwd[i / 2] *= 16;
                        }
                        else if (('a' <= argv[4][i]) && (argv[4][i] <= 'z'))
                        {
                            pwd[i / 2] = argv[4][i] - 'a' + 10;
                            pwd[i / 2] *= 16;
                        }
                        else if (('0' <= argv[4][i]) && (argv[4][i] <= '9'))
                        {
                            pwd[i / 2] = argv[4][i] - '0';
                            pwd[i / 2] *= 16;
                        }
                        else
                        {
                            return 5;
                        }
                        
                        if (('A' <= argv[4][i + 1]) && (argv[4][i + 1] <= 'Z'))
                        {
                            pwd[i / 2 ] += argv[4][i + 1] - 'A' + 10;
                        }
                        else if (('a' <= argv[4][i + 1]) && (argv[4][i + 1] <= 'z'))
                        {
                            pwd[i / 2 ] += argv[4][i + 1] - 'a' + 10;
                        }
                        else if (('0' <= argv[4][i + 1]) && (argv[4][i + 1] <= '9'))
                        {
                            pwd[i / 2 ] += argv[4][i + 1] - '0';
                        }
                        else
                        {
                            return 5;
                        }
                    }
                    
                    /* get the pack */
                    size = (uint8_t)strlen(argv[5]);
                    for (i = 0; i < size; i += 2)
                    {
                        if (('A' <= argv[5][i]) && (argv[5][i] <= 'Z'))
                        {
                            pack[i / 2] = argv[5][i] - 'A' + 10;
                            pack[i / 2] *= 16;
                        }
                        else if (('a' <= argv[5][i]) && (argv[5][i] <= 'z'))
                        {
                            pack[i / 2] = argv[5][i] - 'a' + 10;
                            pack[i / 2] *= 16;
                        }
                        else if (('0' <= argv[5][i]) && (argv[5][i] <= '9'))
                        {
                            pack[i / 2] = argv[5][i] - '0';
                            pack[i / 2] *= 16;
                        }
                        else
                        {
                            return 5;
                        }
                        
                        if (('A' <= argv[5][i + 1]) && (argv[5][i + 1] <= 'Z'))
                        {
                            pack[i / 2 ] += argv[5][i + 1] - 'A' + 10;
                        }
                        else if (('a' <= argv[5][i + 1]) && (argv[5][i + 1] <= 'z'))
                        {
                            pack[i / 2 ] += argv[5][i + 1] - 'a' + 10;
                        }
                        else if (('0' <= argv[5][i + 1]) && (argv[5][i + 1] <= '9'))
                        {
                            pack[i / 2 ] += argv[5][i + 1] - '0';
                        }
                        else
                        {
                            return 5;
                        }
                    }
                    
                    /* set the password pack */
                    res = ntag21x_basic_set_password_pack(pwd, pack);
                    if (res != 0)
                    {
                        (void)ntag21x_basic_deinit();
                        
                        return 1;
                    }
                    
                    ntag21x_interface_debug_print("ntag21x: set password 0x%02X 0x%02X 0x%02X 0x%02X ok.\n",
                                                  pwd[0], pwd[1], pwd[2], pwd[3]);
                    ntag21x_interface_debug_print("ntag21x: set pack 0x%02X 0x%02X ok.\n", pack[0], pack[1]);
                    
                    (void)ntag21x_basic_deinit();
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else
            {
                return 5;
            }
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 7)
    {
        if (strcmp("-c", argv[1]) == 0)
        {
            if (strcmp("set", argv[2]) == 0)
            {
                if (strcmp("-mirror", argv[3]) == 0)
                {
                    uint8_t res;
                    uint8_t i;
                    uint8_t id[8];
                    ntag21x_capability_container_t type;
                    ntag21x_mirror_t mirror;
                    ntag21x_mirror_byte_t byte;
                    uint8_t page;
                    
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
                    
                    if (strcmp("NO_ASCII", argv[4]) == 0)
                    {
                        mirror = NTAG21X_MIRROR_NO_ASCII;
                        ntag21x_interface_debug_print("ntag21x: set no ascii mirror.\n");
                    }
                    else if (strcmp("UID_ASCII", argv[4]) == 0)
                    {
                        mirror = NTAG21X_MIRROR_UID_ASCII;
                        ntag21x_interface_debug_print("ntag21x: set uid ascii mirror.\n");
                    }
                    else if (strcmp("NFC_COUNTER_ASCII", argv[4]) == 0)
                    {
                        mirror = NTAG21X_MIRROR_NFC_COUNTER_ASCII;
                        ntag21x_interface_debug_print("ntag21x: set nfc counter mirror.\n");
                    }
                    else if (strcmp("UID_NFC_COUNTER_ASCII", argv[4]) == 0)
                    {
                        mirror = NTAG21X_MIRROR_UID_NFC_COUNTER_ASCII;
                        ntag21x_interface_debug_print("ntag21x: set uid and nfc counter mirror.\n");
                    }
                    else
                    {
                        ntag21x_interface_debug_print("ntag21x: invalid mirror.\n");
                        (void)ntag21x_basic_deinit();
                        
                        return 0;
                    }
                    
                    if (strcmp("0", argv[5]) == 0)
                    {
                        byte = NTAG21X_MIRROR_BYTE_0;
                        ntag21x_interface_debug_print("ntag21x: set mirror byte 0.\n");
                    }
                    else if (strcmp("1", argv[5]) == 0)
                    {
                        byte = NTAG21X_MIRROR_BYTE_1;
                        ntag21x_interface_debug_print("ntag21x: set mirror byte 1.\n");
                    }
                    else if (strcmp("2", argv[5]) == 0)
                    {
                        byte = NTAG21X_MIRROR_BYTE_2;
                        ntag21x_interface_debug_print("ntag21x: set mirror byte 2.\n");
                    }
                    else if (strcmp("3", argv[5]) == 0)
                    {
                        byte = NTAG21X_MIRROR_BYTE_3;
                        ntag21x_interface_debug_print("ntag21x: set mirror byte 3.\n");
                    }
                    else
                    {
                        ntag21x_interface_debug_print("ntag21x: invalid mirror byte.\n");
                        (void)ntag21x_basic_deinit();
                        
                        return 0;
                    }
                    
                    page = (uint8_t)atoi(argv[6]);
                    ntag21x_interface_debug_print("ntag21x: set mirror page %d.\n", page);
                    
                    /* set the mirror */
                    res = ntag21x_basic_set_mirror(mirror, byte, page);
                    if (res != 0)
                    {
                        (void)ntag21x_basic_deinit();
                        
                        return 1;
                    }
                    
                    (void)ntag21x_basic_deinit();
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else
            {
                return 5;
            }
        }
        else
        {
            return 5;
        }
    }
    /* param is invalid */
    else
    {
        return 5;
    }
}

/**
 * @brief     main function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 * @note      none
 */
int main(uint8_t argc, char **argv)
{
    uint8_t res;

    res = ntag21x(argc, argv);
    if (res == 0)
    {
        /* run success */
    }
    else if (res == 1)
    {
        ntag21x_interface_debug_print("ntag21x: run failed.\n");
    }
    else if (res == 5)
    {
        ntag21x_interface_debug_print("ntag21x: param is invalid.\n");
    }
    else
    {
        ntag21x_interface_debug_print("ntag21x: unknow status code.\n");
    }

    return 0;
}
