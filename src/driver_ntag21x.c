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
 * @file      driver_ntag21x.c
 * @brief     driver ntag21x source file
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

#include "driver_ntag21x.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "NXP NTAG213/5/6"        /**< chip name */
#define MANUFACTURER_NAME         "NXP"                    /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        3.3f                     /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        4.0f                     /**< chip max supply voltage */
#define MAX_CURRENT               30.0f                    /**< chip max current */
#define TEMPERATURE_MIN           -25.0f                   /**< chip min operating temperature */
#define TEMPERATURE_MAX           70.0f                    /**< chip max operating temperature */
#define DRIVER_VERSION            1000                     /**< driver version */

/**
 * @brief chip command definition
 */
#define NTAG21X_COMMAND_REQUEST                  0x26           /**< request command */
#define NTAG21X_COMMAND_WAKE_UP                  0x52           /**< wake up command */
#define NTAG21X_COMMAND_ANTICOLLISION_CL1        0x9320U        /**< anti collision cl1 command */
#define NTAG21X_COMMAND_SELECT_CL1               0x9370U        /**< select cl1 command */
#define NTAG21X_COMMAND_ANTICOLLISION_CL2        0x9520U        /**< anti collision cl2 command */
#define NTAG21X_COMMAND_SELECT_CL2               0x9570U        /**< select cl2 command */
#define NTAG21X_COMMAND_HALT                     0x5000U        /**< halt command */
#define NTAG21X_COMMAND_GET_VERSION              0x60           /**< get version command */
#define NTAG21X_COMMAND_READ                     0x30           /**< read command */
#define NTAG21X_COMMAND_FAST_READ                0x3A           /**< fast read command */
#define NTAG21X_COMMAND_WRITE                    0xA2           /**< write command */
#define NTAG21X_COMMAND_COMP_WRITE               0xA0           /**< comp write command */
#define NTAG21X_COMMAND_READ_CNT                 0x39           /**< read cnt command */
#define NTAG21X_COMMAND_PWD_AUTH                 0x1B           /**< pwd auth command */
#define NTAG21X_COMMAND_READ_SIG                 0x3C           /**< read sig command */

/**
 * @brief     crc calculation
 * @param[in] *p points to a data buffer
 * @param[in] len is the data length
 * @note      none
 */
static void a_ntag21x_iso14443a_crc(uint8_t *p, uint8_t len, uint8_t output[2])
{
    uint32_t w_crc = 0x6363;
    
    do 
    {
        uint8_t  bt;
        
        bt = *p++;                                                                                        /* get one byte */
        bt = (bt ^ (uint8_t)(w_crc & 0x00FF));                                                            /* xor */
        bt = (bt ^ (bt << 4));                                                                            /* xor */
        w_crc = (w_crc >> 8) ^ ((uint32_t) bt << 8) ^ ((uint32_t) bt << 3) ^ ((uint32_t) bt >> 4);        /* get the crc */
    } while (--len);                                                                                      /* len-- */

    output[0] = (uint8_t)(w_crc & 0xFF);                                                                  /* lsb */
    output[1] = (uint8_t)((w_crc >> 8) & 0xFF);                                                           /* msb */
}

/**
 * @brief      ntag21x read conf
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[in]  page is the page of read
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read conf failed
 * @note       none
 */
static uint8_t a_ntag21x_conf_read(ntag21x_handle_t *handle, uint8_t page, uint8_t data[4])
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[5];
    uint8_t output_len;
    uint8_t output_buf[6];
    uint8_t crc_buf[2];
    
    input_len = 5;                                                                               /* set the input length */
    input_buf[0] = NTAG21X_COMMAND_FAST_READ;                                                    /* set the command */
    input_buf[1] = page;                                                                         /* set the start page */
    input_buf[2] = page;                                                                         /* set the stop page */
    a_ntag21x_iso14443a_crc(input_buf , 3, input_buf + 3);                                       /* get the crc */
    output_len = 6;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("ntag21x: contactless transceiver failed.\n");                       /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 6)                                                                         /* check the output_len */
    {
        handle->debug_print("ntag21x: output_len is invalid.\n");                                /* output_len is invalid */
        
        return 1;                                                                                /* return error */
    }
    a_ntag21x_iso14443a_crc(output_buf, 4, crc_buf);                                             /* get the crc */
    if ((output_buf[4] == crc_buf[0]) && (output_buf[5] == crc_buf[1]))                          /* check the crc */
    {
        memcpy(data, output_buf, 4);                                                             /* copy the data */
        
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        handle->debug_print("ntag21x: crc error.\n");                                            /* crc error */
        
        return 1;                                                                                /* return error */
    }
}

/**
 * @brief     ntag21x write conf
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] page is the page of write
 * @param[in] *data points to a data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write conf failed
 * @note      none
 */
static uint8_t a_ntag21x_conf_write(ntag21x_handle_t *handle, uint8_t page, uint8_t data[4])
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[8];
    uint8_t output_len;
    uint8_t output_buf[1];
    
    input_len = 8;                                                                               /* set the input length */
    input_buf[0] = NTAG21X_COMMAND_WRITE;                                                        /* set the command */
    input_buf[1] = page;                                                                         /* set the setting page */
    input_buf[2] = data[0];                                                                      /* set data0 */
    input_buf[3] = data[1];                                                                      /* set data1 */
    input_buf[4] = data[2];                                                                      /* set data2 */
    input_buf[5] = data[3];                                                                      /* set data3 */
    a_ntag21x_iso14443a_crc(input_buf, 6, input_buf + 6);                                        /* get the crc */
    output_len = 1;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("ntag21x: contactless transceiver failed.\n");                       /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 1)                                                                         /* check the output_len */
    {
        handle->debug_print("ntag21x: output_len is invalid.\n");                                /* output_len is invalid */
        
        return 1;                                                                                /* return error */
    }
    if (output_buf[0] != 0xA)                                                                    /* check the result */
    {
        handle->debug_print("ntag21x: ack error.\n");                                            /* ack error */
        
        return 1;                                                                                /* return error */
    }
    
    return 0;                                                                                    /* success return 0 */
}

/**
 * @brief     initialize the chip
 * @param[in] *handle points to an ntag21x handle structure
 * @return    status code
 *            - 0 success
 *            - 1 contactless initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 * @note      none
 */
uint8_t ntag21x_init(ntag21x_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->debug_print == NULL)                                               /* check debug_print */
    {
        return 3;                                                                  /* return error */
    }
    if (handle->contactless_init == NULL)                                          /* check contactless_init */
    {
        handle->debug_print("ntag21x: contactless_init is null.\n");               /* contactless_init is null */
        
        return 3;                                                                  /* return error */
    }
    if (handle->contactless_deinit == NULL)                                        /* check contactless_deinit */
    {
        handle->debug_print("ntag21x: contactless_deinit is null.\n");             /* contactless_deinit is null */
        
        return 3;                                                                  /* return error */
    }
    if (handle->contactless_transceiver == NULL)                                   /* check contactless_transceiver */
    {
        handle->debug_print("ntag21x: contactless_transceiver is null.\n");        /* contactless_transceiver is null */
        
        return 3;                                                                  /* return error */
    }
    if (handle->delay_ms == NULL)                                                  /* check delay_ms */
    {
        handle->debug_print("ntag21x: delay_ms is null.\n");                       /* delay_ms is null */
        
        return 3;                                                                  /* return error */
    }
    
    res = handle->contactless_init();                                              /* contactless init */
    if (res != 0)                                                                  /* check the result */
    {
        handle->debug_print("ntag21x: contactless init failed.\n");                /* contactless init failed */
        
        return 1;                                                                  /* return error */
    }
    handle->type = NTAG21X_TYPE_INVALID;                                           /* set the invalid type */
    handle->end_page = 0xFF;                                                       /* set 0xFF */
    handle->inited = 1;                                                            /* flag inited */
    
    return 0;                                                                      /* success return 0 */
}

/**
 * @brief     close the chip
 * @param[in] *handle points to an ntag21x handle structure
 * @return    status code
 *            - 0 success
 *            - 1 contactless deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ntag21x_deinit(ntag21x_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }
    
    res = handle->contactless_deinit();                                      /* contactless deinit */
    if (res != 0)                                                            /* check the result */
    {
        handle->debug_print("ntag21x: contactless deinit failed.\n");        /* contactless deinit failed */
        
        return 1;                                                            /* return error */
    }
    handle->inited = 0;                                                      /* flag closed */
    
    return 0;                                                                /* success return 0 */
}

/**
 * @brief      ntag21x request
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *type points to a type buffer
 * @return     status code
 *             - 0 success
 *             - 1 request failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 type is invalid
 * @note       none
 */
uint8_t ntag21x_request(ntag21x_handle_t *handle, ntag21x_type_t *type)
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[1];
    uint8_t output_len;
    uint8_t output_buf[2];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 1;                                                                               /* set the input length */
    input_buf[0] = NTAG21X_COMMAND_REQUEST;                                                      /* set the command */
    output_len = 2;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("ntag21x: contactless transceiver failed.\n");                       /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 2)                                                                         /* check the output_len */
    {
        handle->debug_print("ntag21x: output_len is invalid.\n");                                /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    if ((output_buf[0] == 0x44) && (output_buf[1] == 0x00))                                      /* check classic type */
    {
        *type = NTAG21X_TYPE_213_5_6;                                                            /* ntag213/5/6 */
        handle->type = *type;                                                                    /* save the type */
        
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        *type = NTAG21X_TYPE_INVALID;                                                            /* invalid */
        handle->type = *type;                                                                    /* save the type */
        handle->debug_print("ntag21x: type is invalid.\n");                                      /* type is invalid */
        
        return 5;                                                                                /* return error */
    }
}

/**
 * @brief      ntag21x wake up
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *type points to a type buffer
 * @return     status code
 *             - 0 success
 *             - 1 wake up failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 type is invalid
 * @note       none
 */
uint8_t ntag21x_wake_up(ntag21x_handle_t *handle, ntag21x_type_t *type)
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[1];
    uint8_t output_len;
    uint8_t output_buf[2];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    handle->delay_ms(1);                                                                         /* delay 1ms */
    input_len = 1;                                                                               /* set the input length */
    input_buf[0] = NTAG21X_COMMAND_WAKE_UP;                                                      /* set the command */
    output_len = 2;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("ntag21x: contactless transceiver failed.\n");                       /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 2)                                                                         /* check the output_len */
    {
        handle->debug_print("ntag21x: output_len is invalid.\n");                                /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    if ((output_buf[0] == 0x44) && (output_buf[1] == 0x00))                                      /* check classic type */
    {
        *type = NTAG21X_TYPE_213_5_6;                                                            /* ntag213/5/6 */
        handle->type = *type;                                                                    /* save the type */
        
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        *type = NTAG21X_TYPE_INVALID;                                                            /* invalid */
        handle->type = *type;                                                                    /* save the type */
        handle->debug_print("ntag21x: type is invalid.\n");                                      /* type is invalid */
        
        return 5;                                                                                /* return error */
    }
}

/**
 * @brief     ntag21x halt
 * @param[in] *handle points to an ntag21x handle structure
 * @return    status code
 *            - 0 success
 *            - 1 halt failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ntag21x_halt(ntag21x_handle_t *handle)
{
    uint8_t input_len;
    uint8_t input_buf[4];
    uint8_t output_len;
    uint8_t output_buf[1];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 4;                                                                               /* set the input length */
    input_buf[0] = (NTAG21X_COMMAND_HALT >> 8) & 0xFF;                                           /* set the command */
    input_buf[1] = (NTAG21X_COMMAND_HALT >> 0) & 0xFF;                                           /* set the command */
    a_ntag21x_iso14443a_crc(input_buf, 2, input_buf + 2);                                        /* get the crc */
    output_len = 1;                                                                              /* set the output length */
    (void)handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    
    return 0;                                                                                    /* success return 0 */
}

/**
 * @brief      ntag21x anti collision cl1
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *id points to an id buffer
 * @return     status code
 *             - 0 success
 *             - 1 anti collision cl1 failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 check error
 * @note       none
 */
uint8_t ntag21x_anticollision_cl1(ntag21x_handle_t *handle, uint8_t id[4])
{
    uint8_t res;
    uint8_t i;
    uint8_t check;
    uint8_t input_len;
    uint8_t input_buf[2];
    uint8_t output_len;
    uint8_t output_buf[5];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 2;                                                                               /* set the input length */
    input_buf[0] = (NTAG21X_COMMAND_ANTICOLLISION_CL1 >> 8) & 0xFF;                              /* set the command */
    input_buf[1] = (NTAG21X_COMMAND_ANTICOLLISION_CL1 >> 0) & 0xFF;                              /* set the command */
    output_len = 5;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("ntag21x: contactless transceiver failed.\n");                       /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 5)                                                                         /* check the output_len */
    {
        handle->debug_print("ntag21x: output_len is invalid.\n");                                /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    check = 0;                                                                                   /* init 0 */
    for (i = 0; i < 4; i++)                                                                      /* run 4 times */
    {
        id[i] = output_buf[i];                                                                   /* get one id */
        check ^= output_buf[i];                                                                  /* xor */
    }
    if (check != output_buf[4])                                                                  /* check the result */
    {
        handle->debug_print("ntag21x: check error.\n");                                          /* check error */
        
        return 5;                                                                                /* return error */
    }
    
    return 0;                                                                                    /* success return 0 */
}

/**
 * @brief      ntag21x anti collision cl2
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *id points to an id buffer
 * @return     status code
 *             - 0 success
 *             - 1 anti collision cl2 failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 check error
 * @note       none
 */
uint8_t ntag21x_anticollision_cl2(ntag21x_handle_t *handle, uint8_t id[4])
{
    uint8_t res;
    uint8_t i;
    uint8_t check;
    uint8_t input_len;
    uint8_t input_buf[2];
    uint8_t output_len;
    uint8_t output_buf[5];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 2;                                                                               /* set the input length */
    input_buf[0] = (NTAG21X_COMMAND_ANTICOLLISION_CL2 >> 8) & 0xFF;                              /* set the command */
    input_buf[1] = (NTAG21X_COMMAND_ANTICOLLISION_CL2 >> 0) & 0xFF;                              /* set the command */
    output_len = 5;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("ntag21x: contactless transceiver failed.\n");                       /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 5)                                                                         /* check the output_len */
    {
        handle->debug_print("ntag21x: output_len is invalid.\n");                                /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    check = 0;                                                                                   /* init 0 */
    for (i = 0; i < 4; i++)                                                                      /* run 4 times */
    {
        id[i] = output_buf[i];                                                                   /* get one id */
        check ^= output_buf[i];                                                                  /* xor */
    }
    if (check != output_buf[4])                                                                  /* check the result */
    {
        handle->debug_print("ntag21x: check error.\n");                                          /* check error */
        
        return 5;                                                                                /* return error */
    }
    
    return 0;                                                                                    /* success return 0 */
}

/**
 * @brief     ntag21x select cl1
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] *id points to an id buffer
 * @return    status code
 *            - 0 success
 *            - 1 select cl1 failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 output_len is invalid
 *            - 5 sak error
 * @note      none
 */
uint8_t ntag21x_select_cl1(ntag21x_handle_t *handle, uint8_t id[4])
{
    uint8_t res;
    uint8_t i;
    uint8_t input_len;
    uint8_t input_buf[9];
    uint8_t output_len;
    uint8_t output_buf[1];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 9;                                                                               /* set the input length */
    input_buf[0] = (NTAG21X_COMMAND_SELECT_CL1 >> 8) & 0xFF;                                     /* set the command */
    input_buf[1] = (NTAG21X_COMMAND_SELECT_CL1 >> 0) & 0xFF;                                     /* set the command */
    input_buf[6] = 0;                                                                            /* init 0 */
    for (i = 0; i < 4; i++)                                                                      /* run 4 times */
    {
        input_buf[2 + i] = id[i];                                                                /* get one id */
        input_buf[6] ^= id[i];                                                                   /* xor */
    }
    a_ntag21x_iso14443a_crc(input_buf, 7, input_buf + 7);                                        /* get the crc */
    output_len = 1;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("ntag21x: contactless transceiver failed.\n");                       /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 1)                                                                         /* check the output_len */
    {
        handle->debug_print("ntag21x: output_len is invalid.\n");                                /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    if (output_buf[0] == 0x04)                                                                   /* check the sak */
    {
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        handle->debug_print("ntag21x: sak error.\n");                                            /* sak error */
        
        return 5;                                                                                /* return error */
    }
}

/**
 * @brief     ntag21x select cl2
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] *id points to an id buffer
 * @return    status code
 *            - 0 success
 *            - 1 select cl2 failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 output_len is invalid
 *            - 5 sak error
 * @note      none
 */
uint8_t ntag21x_select_cl2(ntag21x_handle_t *handle, uint8_t id[4])
{
    uint8_t res;
    uint8_t i;
    uint8_t input_len;
    uint8_t input_buf[9];
    uint8_t output_len;
    uint8_t output_buf[1];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 9;                                                                               /* set the input length */
    input_buf[0] = (NTAG21X_COMMAND_SELECT_CL2 >> 8) & 0xFF;                                     /* set the command */
    input_buf[1] = (NTAG21X_COMMAND_SELECT_CL2 >> 0) & 0xFF;                                     /* set the command */
    input_buf[6] = 0;                                                                            /* init 0 */
    for (i = 0; i < 4; i++)                                                                      /* run 4 times */
    {
        input_buf[2 + i] = id[i];                                                                /* get one id */
        input_buf[6] ^= id[i];                                                                   /* xor */
    }
    a_ntag21x_iso14443a_crc(input_buf, 7, input_buf + 7);                                        /* get the crc */
    output_len = 1;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("ntag21x: contactless transceiver failed.\n");                       /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 1)                                                                         /* check the output_len */
    {
        handle->debug_print("ntag21x: output_len is invalid.\n");                                /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    if (output_buf[0] == 0x00)                                                                   /* check the sak */
    {
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        handle->debug_print("ntag21x: sak error.\n");                                            /* sak error */
        
        return 5;                                                                                /* return error */
    }
}

/**
 * @brief      ntag21x get the version
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *version points to a version structure
 * @return     status code
 *             - 0 success
 *             - 1 get version failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 crc error
 * @note       none
 */
uint8_t ntag21x_get_version(ntag21x_handle_t *handle, ntag21x_version_t *version)
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[3];
    uint8_t output_len;
    uint8_t output_buf[10];
    uint8_t crc_buf[2];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 3;                                                                               /* set the input length */
    input_buf[0] = NTAG21X_COMMAND_GET_VERSION;                                                  /* set the command */
    a_ntag21x_iso14443a_crc(input_buf, 1, input_buf + 1);                                        /* get the crc */
    output_len = 10;                                                                             /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("ntag21x: contactless transceiver failed.\n");                       /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 10)                                                                        /* check the output_len */
    {
        handle->debug_print("ntag21x: output_len is invalid.\n");                                /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    a_ntag21x_iso14443a_crc(output_buf, 8, crc_buf);                                             /* get the crc */
    if ((output_buf[8] == crc_buf[0]) && (output_buf[9] == crc_buf[1]))                          /* check the crc */
    {
        version->fixed_header = output_buf[0];                                                   /* fixed header */
        version->vendor_id = output_buf[1];                                                      /* vendor id */
        version->product_type = output_buf[2];                                                   /* product type */
        version->product_subtype = output_buf[3];                                                /* product subtype */
        version->major_product_version = output_buf[4];                                          /* major product version */
        version->minor_product_version = output_buf[5];                                          /* minor product version */
        version->storage_size = output_buf[6];                                                   /* storage size */
        if (version->storage_size == 0x0F)                                                       /* ntag213 */
        {
            handle->end_page = 0x2C;                                                             /* set the last page */
        }
        else if (version->storage_size == 0x11)                                                  /* ntag215 */
        {
            handle->end_page = 0x86;                                                             /* set the last page */
        }
        else if (version->storage_size == 0x13)                                                  /* ntag216 */
        {
            handle->end_page = 0xE6;                                                             /* set the last page */
        }
        else                                                                                     /* unknown */
        {
            handle->end_page = 0xFF;                                                             /* set 0xFF */
        }
        version->protocol_type = output_buf[7];                                                  /* protocol type */
        
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        handle->debug_print("ntag21x: crc error.\n");                                            /* crc error */
        
        return 5;                                                                                /* return error */
    }
}

/**
 * @brief      ntag21x read the counter
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *cnt points to a counter buffer
 * @return     status code
 *             - 0 success
 *             - 1 read counter failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 crc error
 * @note       none
 */
uint8_t ntag21x_read_counter(ntag21x_handle_t *handle, uint32_t *cnt)
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[4];
    uint8_t output_len;
    uint8_t output_buf[5];
    uint8_t crc_buf[2];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 4;                                                                               /* set the input length */
    input_buf[0] = NTAG21X_COMMAND_READ_CNT;                                                     /* set the command */
    input_buf[1] = 0x02;                                                                         /* set the address */
    a_ntag21x_iso14443a_crc(input_buf, 2, input_buf + 2);                                        /* get the crc */
    output_len = 5;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("ntag21x: contactless transceiver failed.\n");                       /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 5)                                                                         /* check the output_len */
    {
        handle->debug_print("ntag21x: output_len is invalid.\n");                                /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    a_ntag21x_iso14443a_crc(output_buf, 3, crc_buf);                                             /* get the crc */
    if ((output_buf[3] == crc_buf[0]) && (output_buf[4] == crc_buf[1]))                          /* check the result */
    {
        *cnt = ((uint32_t)output_buf[2] << 16) | ((uint32_t)output_buf[1] << 8) |
               ((uint32_t)output_buf[0] << 0);                                                   /* set the counter */
        
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        handle->debug_print("ntag21x: crc error.\n");                                            /* crc error */
        
        return 5;                                                                                /* return error */
    }
}

/**
 * @brief      ntag21x read the signature
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *signature points to a signature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read signature failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 crc error
 * @note       none
 */
uint8_t ntag21x_read_signature(ntag21x_handle_t *handle, uint8_t signature[32])
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[4];
    uint8_t output_len;
    uint8_t output_buf[34];
    uint8_t crc_buf[2];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 4;                                                                               /* set the input length */
    input_buf[0] = NTAG21X_COMMAND_READ_SIG;                                                     /* set the command */
    input_buf[1] = 0x00;                                                                         /* set the address */
    a_ntag21x_iso14443a_crc(input_buf, 2, input_buf + 2);                                        /* get the crc */
    output_len = 34;                                                                             /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("ntag21x: contactless transceiver failed.\n");                       /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 34)                                                                        /* check the output_len */
    {
        handle->debug_print("ntag21x: output_len is invalid.\n");                                /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    a_ntag21x_iso14443a_crc(output_buf, 32, crc_buf);                                            /* get the crc */
    if ((output_buf[32] == crc_buf[0]) && (output_buf[33] == crc_buf[1]))                        /* check the result */
    {
        memcpy(signature, output_buf, 32);                                                       /* copy the data */
        
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        handle->debug_print("ntag21x: crc error.\n");                                            /* crc error */
        
        return 5;                                                                                /* return error */
    }
}

/**
 * @brief      ntag21x get the serial number
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *number points to a number buffer
 * @return     status code
 *             - 0 success
 *             - 1 get serial number failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 crc error
 * @note       none
 */
uint8_t ntag21x_get_serial_number(ntag21x_handle_t *handle, uint8_t number[7])
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[4];
    uint8_t output_len;
    uint8_t output_buf[18];
    uint8_t crc_buf[2];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 4;                                                                               /* set the input length */
    input_buf[0] = NTAG21X_COMMAND_READ;                                                         /* set the command */
    input_buf[1] = 0x00;                                                                         /* set the read page */
    a_ntag21x_iso14443a_crc(input_buf , 2, input_buf + 2);                                       /* get the crc */
    output_len = 18;                                                                             /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("ntag21x: contactless transceiver failed.\n");                       /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 18)                                                                        /* check the output_len */
    {
        handle->debug_print("ntag21x: output_len is invalid.\n");                                /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    a_ntag21x_iso14443a_crc(output_buf, 16, crc_buf);                                            /* get the crc */
    if ((output_buf[16] == crc_buf[0]) && (output_buf[17] == crc_buf[1]))                        /* check the crc */
    {
        number[0] = output_buf[0];                                                               /* set the number 0 */
        number[1] = output_buf[1];                                                               /* set the number 1 */
        number[2] = output_buf[2];                                                               /* set the number 2 */
        number[3] = output_buf[4];                                                               /* set the number 3 */
        number[4] = output_buf[5];                                                               /* set the number 4 */
        number[5] = output_buf[6];                                                               /* set the number 5 */
        number[6] = output_buf[7];                                                               /* set the number 6 */
        
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        handle->debug_print("ntag21x: crc error.\n");                                            /* crc error */
        
        return 5;                                                                                /* return error */
    }
}

/**
 * @brief      ntag21x get the capability container
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *container points to a capability container buffer
 * @return     status code
 *             - 0 success
 *             - 1 get capability container failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 crc error
 *             - 6 data is invalid
 * @note       none
 */
uint8_t ntag21x_get_capability_container(ntag21x_handle_t *handle, ntag21x_capability_container_t *container)
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[4];
    uint8_t output_len;
    uint8_t output_buf[18];
    uint8_t crc_buf[2];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 4;                                                                               /* set the input length */
    input_buf[0] = NTAG21X_COMMAND_READ;                                                         /* set the command */
    input_buf[1] = 0x00;                                                                         /* set the read page */
    a_ntag21x_iso14443a_crc(input_buf , 2, input_buf + 2);                                       /* get the crc */
    output_len = 18;                                                                             /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("ntag21x: contactless transceiver failed.\n");                       /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 18)                                                                        /* check the output_len */
    {
        handle->debug_print("ntag21x: output_len is invalid.\n");                                /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    a_ntag21x_iso14443a_crc(output_buf, 16, crc_buf);                                            /* get the crc */
    if ((output_buf[16] == crc_buf[0]) && (output_buf[17] == crc_buf[1]))                        /* check the crc */
    {
        if ((output_buf[12] == 0xE1) && (output_buf[13] == 0x10) &&
            (output_buf[15] == 0x00)                                                             /* check the data */
           )
        {
            if (output_buf[14] == 0x12)                                                          /* ntag213 */
            {
                *container = NTAG21X_CAPABILITY_CONTAINER_144_BYTE_NTAG213;                      /* set the ntag213 */
                handle->end_page = 0x2C;                                                         /* set the end page */
            }
            else if (output_buf[14] == 0x3E)                                                     /* ntag215 */
            {
                *container = NTAG21X_CAPABILITY_CONTAINER_496_BYTE_NTAG215;                      /* set the ntag215 */
                handle->end_page = 0x86;                                                         /* set the end page */
            }
            else if (output_buf[14] == 0x6D)                                                     /* ntag216 */
            {
                *container = NTAG21X_CAPABILITY_CONTAINER_872_BYTE_NTAG216;                      /* set the ntag216 */
                handle->end_page = 0xE6;                                                         /* set the end page */
            }
            else
            {
                handle->debug_print("ntag21x: data is invalid.\n");                              /* data is invalid */
                
                return 6;                                                                        /* return error */
            }
            
            return 0;                                                                            /* success return 0 */
        }
        else
        {
            handle->debug_print("ntag21x: data is invalid.\n");                                  /* data is invalid */
            
            return 6;                                                                            /* return error */
        }
    }
    else
    {
        handle->debug_print("ntag21x: crc error.\n");                                            /* crc error */
        
        return 5;                                                                                /* return error */
    }
}

/**
 * @brief      ntag21x read four pages
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[in]  start_page is the start page of read
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read four pages failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 crc error
 * @note       none
 */
uint8_t ntag21x_read_four_pages(ntag21x_handle_t *handle, uint8_t start_page, uint8_t data[16])
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[4];
    uint8_t output_len;
    uint8_t output_buf[18];
    uint8_t crc_buf[2];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 4;                                                                               /* set the input length */
    input_buf[0] = NTAG21X_COMMAND_READ;                                                         /* set the command */
    input_buf[1] = start_page;                                                                   /* set the page */
    a_ntag21x_iso14443a_crc(input_buf , 2, input_buf + 2);                                       /* get the crc */
    output_len = 18;                                                                             /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("ntag21x: contactless transceiver failed.\n");                       /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 18)                                                                        /* check the output_len */
    {
        handle->debug_print("ntag21x: output_len is invalid.\n");                                /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    a_ntag21x_iso14443a_crc(output_buf, 16, crc_buf);                                            /* get the crc */
    if ((output_buf[16] == crc_buf[0]) && (output_buf[17] == crc_buf[1]))                        /* check the crc */
    {
        memcpy(data, output_buf, 16);                                                            /* copy the data */
        
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        handle->debug_print("ntag21x: crc error.\n");                                            /* crc error */
        
        return 5;                                                                                /* return error */
    }
}

/**
 * @brief      ntag21x read page
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[in]  page is the page of read
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read page failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 crc error
 * @note       none
 */
uint8_t ntag21x_read_page(ntag21x_handle_t *handle, uint8_t page, uint8_t data[4])
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[4];
    uint8_t output_len;
    uint8_t output_buf[18];
    uint8_t crc_buf[2];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 4;                                                                               /* set the input length */
    input_buf[0] = NTAG21X_COMMAND_READ;                                                         /* set the command */
    input_buf[1] = page;                                                                         /* set the page */
    a_ntag21x_iso14443a_crc(input_buf , 2, input_buf + 2);                                       /* get the crc */
    output_len = 18;                                                                             /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("ntag21x: contactless transceiver failed.\n");                       /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 18)                                                                        /* check the output_len */
    {
        handle->debug_print("ntag21x: output_len is invalid.\n");                                /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    a_ntag21x_iso14443a_crc(output_buf, 16, crc_buf);                                            /* get the crc */
    if ((output_buf[16] == crc_buf[0]) && (output_buf[17] == crc_buf[1]))                        /* check the crc */
    {
        memcpy(data, output_buf, 4);                                                             /* copy the data */
        
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        handle->debug_print("ntag21x: crc error.\n");                                            /* crc error */
        
        return 5;                                                                                /* return error */
    }
}

/**
 * @brief         ntag21x fast read page
 * @param[in]     *handle points to an ntag21x handle structure
 * @param[in]     start_page is the start page
 * @param[in]     stop_page is the stop page
 * @param[out]    *data points to a data buffer
 * @param[in,out] *len points to a data length buffer
 * @return        status code
 *                - 0 success
 *                - 1 fast read page failed
 *                - 2 handle is NULL
 *                - 3 handle is not initialized
 *                - 4 stop_page < start_page
 *                - 5 stop_page - start_page + 1 is over 15
 *                - 6 len is invalid
 *                - 7 output_len is invalid
 *                - 8 crc error
 * @note          stop_page - start_page + 1 <= 15
 *                4 * (stop_page - start_page + 1) < len
 */
uint8_t ntag21x_fast_read_page(ntag21x_handle_t *handle, uint8_t start_page, uint8_t stop_page, uint8_t *data, uint16_t *len)
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[5];
    uint8_t output_len;
    uint8_t cal_len;
    uint8_t output_buf[64];
    uint8_t crc_buf[2];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    if (stop_page < start_page)                                                                  /* check start and stop page */
    {
        handle->debug_print("ntag21x: stop_page < start_page.\n");                               /* stop_page < start_page */
        
        return 4;                                                                                /* return error */
    }
    if (stop_page - start_page + 1 > 15)                                                         /* check start and stop page */
    {
        handle->debug_print("ntag21x: stop_page - start_page + 1 is over 15.\n");                /* stop_page - start_page + 1 is over 15 */
        
        return 5;                                                                                /* return error */
    }
    if ((*len) < (4 * (stop_page - start_page + 1)))                                             /* check the length */
    {
        handle->debug_print("ntag21x: len < %d.\n", 4 * (stop_page - start_page + 1));           /* len is invalid */
        
        return 6;                                                                                /* return error */
    }
    
    input_len = 5;                                                                               /* set the input length */
    input_buf[0] = NTAG21X_COMMAND_FAST_READ;                                                    /* set the command */
    input_buf[1] = start_page;                                                                   /* set the start page */
    input_buf[2] = stop_page;                                                                    /* set the stop page */
    a_ntag21x_iso14443a_crc(input_buf , 3, input_buf + 3);                                       /* get the crc */
    cal_len = 4 * (stop_page - start_page + 1);                                                  /* set the cal length */
    output_len = (uint8_t)(cal_len + 2);                                                         /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("ntag21x: contactless transceiver failed.\n");                       /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != (cal_len + 2))                                                             /* check the output_len */
    {
        handle->debug_print("ntag21x: output_len is invalid.\n");                                /* output_len is invalid */
        
        return 7;                                                                                /* return error */
    }
    a_ntag21x_iso14443a_crc(output_buf, (uint8_t)cal_len, crc_buf);                              /* get the crc */
    if ((output_buf[cal_len] == crc_buf[0]) && (output_buf[cal_len + 1] == crc_buf[1]))          /* check the crc */
    {
        memcpy(data, output_buf, cal_len);                                                       /* copy the data */
        *len = cal_len;                                                                          /* set the length */
        
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        handle->debug_print("ntag21x: crc error.\n");                                            /* crc error */
        
        return 8;                                                                                /* return error */
    }
}

/**
 * @brief     ntag21x compatibility write page
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] page is the page of write
 * @param[in] *data points to a data buffer
 * @return    status code
 *            - 0 success
 *            - 1 compatibility write page failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 output_len is invalid
 *            - 5 ack error
 * @note      none
 */
uint8_t ntag21x_compatibility_write_page(ntag21x_handle_t *handle, uint8_t page, uint8_t data[4])
{
    uint8_t res;
    uint8_t i;
    uint8_t input_len;
    uint8_t input_buf[18];
    uint8_t output_len;
    uint8_t output_buf[1];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 4;                                                                               /* set the input length */
    input_buf[0] = NTAG21X_COMMAND_COMP_WRITE;                                                   /* set the command */
    input_buf[1] = page;                                                                         /* set the page */
    a_ntag21x_iso14443a_crc(input_buf, 2, input_buf + 2);                                        /* get the crc */
    output_len = 1;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("ntag21x: contactless transceiver failed.\n");                       /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 1)                                                                         /* check the output_len */
    {
        handle->debug_print("ntag21x: output_len is invalid.\n");                                /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    if (output_buf[0] != 0xA)                                                                    /* check the result */
    {
        handle->debug_print("ntag21x: ack error.\n");                                            /* ack error */
        
        return 5;                                                                                /* return error */
    }
    
    for (i = 0; i < 4; i ++)                                                                     /* 4 times */
    {
        input_buf[i] = data[i];                                                                  /* copy data */
    }
    for (i = 0; i < 12; i ++)                                                                    /* 12 times */
    {
        input_buf[4 + i] = 0x00;                                                                 /* copy data */
    }
    a_ntag21x_iso14443a_crc(input_buf, 16, input_buf + 16);                                      /* get the crc */
    input_len = 18;                                                                              /* set the input length */
    output_len = 1;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("ntag21x: contactless transceiver failed.\n");                       /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_buf[0] != 0xA)                                                                    /* check the result */
    {
        handle->debug_print("ntag21x: ack error.\n");                                            /* ack error */
        
        return 5;                                                                                /* return error */
    }
    
    return 0;                                                                                    /* success return 0 */
}

/**
 * @brief     ntag21x write page
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] page is the page of write
 * @param[in] *data points to a data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write page failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 output_len is invalid
 *            - 5 ack error
 * @note      none
 */
uint8_t ntag21x_write_page(ntag21x_handle_t *handle, uint8_t page, uint8_t data[4])
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[8];
    uint8_t output_len;
    uint8_t output_buf[1];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 8;                                                                               /* set the input length */
    input_buf[0] = NTAG21X_COMMAND_WRITE;                                                        /* set the command */
    input_buf[1] = page;                                                                         /* set the page */
    input_buf[2] = data[0];                                                                      /* set data0 */
    input_buf[3] = data[1];                                                                      /* set data1 */
    input_buf[4] = data[2];                                                                      /* set data2 */
    input_buf[5] = data[3];                                                                      /* set data3 */
    a_ntag21x_iso14443a_crc(input_buf, 6, input_buf + 6);                                        /* get the crc */
    output_len = 1;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("ntag21x: contactless transceiver failed.\n");                       /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 1)                                                                         /* check the output_len */
    {
        handle->debug_print("ntag21x: output_len is invalid.\n");                                /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    if (output_buf[0] != 0xA)                                                                    /* check the result */
    {
        handle->debug_print("ntag21x: ack error.\n");                                            /* ack error */
        
        return 5;                                                                                /* return error */
    }
    
    return 0;                                                                                    /* success return 0 */
}

/**
 * @brief     ntag21x authenticate
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] *pwd points to a pwd buffer
 * @param[in] *pack points to a pack buffer
 * @return    status code
 *            - 0 success
 *            - 1 authenticate failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 output_len is invalid
 *            - 5 crc error
 *            - 6 pack check failed
 * @note      none
 */
uint8_t ntag21x_authenticate(ntag21x_handle_t *handle, uint8_t pwd[4], uint8_t pack[2])
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[7];
    uint8_t output_len;
    uint8_t output_buf[4];
    uint8_t crc_buf[2];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 7;                                                                               /* set the input length */
    input_buf[0] = NTAG21X_COMMAND_PWD_AUTH;                                                     /* set the command */
    input_buf[1] = pwd[0];                                                                       /* set pwd0 */
    input_buf[2] = pwd[1];                                                                       /* set pwd1 */
    input_buf[3] = pwd[2];                                                                       /* set pwd2 */
    input_buf[4] = pwd[3];                                                                       /* set pwd3 */
    a_ntag21x_iso14443a_crc(input_buf, 5, input_buf + 5);                                        /* get the crc */
    output_len = 4;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("ntag21x: contactless transceiver failed.\n");                       /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 4)                                                                         /* check the output_len */
    {
        handle->debug_print("ntag21x: output_len is invalid.\n");                                /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    a_ntag21x_iso14443a_crc(output_buf, 2, crc_buf);                                             /* get the crc */
    if ((output_buf[2] == crc_buf[0]) && (output_buf[3] == crc_buf[1]))                          /* check the crc */
    {
        if ((output_buf[0] != pack[0]) || (output_buf[1] != pack[1]))                            /* check the pack */
        {
            handle->debug_print("ntag21x: pack check failed.\n");                                /* pack check failed. */
            
            return 6;                                                                            /* return error */
        }
        
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        handle->debug_print("ntag21x: crc error.\n");                                            /* crc error */
        
        return 5;                                                                                /* return error */
    }
}

/**
 * @brief     ntag21x set the password
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] *pwd points to a password buffer
 * @return    status code
 *            - 0 success
 *            - 1 set pwd failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 output_len is invalid
 *            - 5 ack error
 * @note      none
 */
uint8_t ntag21x_set_password(ntag21x_handle_t *handle, uint8_t pwd[4])
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[8];
    uint8_t output_len;
    uint8_t output_buf[1];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 8;                                                                               /* set the input length */
    input_buf[0] = NTAG21X_COMMAND_WRITE;                                                        /* set the command */
    input_buf[1] = handle->end_page - 1;                                                         /* set the last page */
    input_buf[2] = pwd[0];                                                                       /* set pwd0 */
    input_buf[3] = pwd[1];                                                                       /* set pwd1 */
    input_buf[4] = pwd[2];                                                                       /* set pwd2 */
    input_buf[5] = pwd[3];                                                                       /* set pwd3 */
    a_ntag21x_iso14443a_crc(input_buf, 6, input_buf + 6);                                        /* get the crc */
    output_len = 1;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("ntag21x: contactless transceiver failed.\n");                       /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 1)                                                                         /* check the output_len */
    {
        handle->debug_print("ntag21x: output_len is invalid.\n");                                /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    if (output_buf[0] != 0xA)                                                                    /* check the result */
    {
        handle->debug_print("ntag21x: ack error.\n");                                            /* ack error */
        
        return 5;                                                                                /* return error */
    }
    
    return 0;                                                                                    /* success return 0 */
}

/**
 * @brief     ntag21x set the pack
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] *pack points to a pack buffer
 * @return    status code
 *            - 0 success
 *            - 1 set pack failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 output_len is invalid
 *            - 5 ack error
 * @note      none
 */
uint8_t ntag21x_set_pack(ntag21x_handle_t *handle, uint8_t pack[2])
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[8];
    uint8_t output_len;
    uint8_t output_buf[1];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 8;                                                                               /* set the input length */
    input_buf[0] = NTAG21X_COMMAND_WRITE;                                                        /* set the command */
    input_buf[1] = handle->end_page;                                                             /* set the last page */
    input_buf[2] = pack[0];                                                                      /* set pack0 */
    input_buf[3] = pack[1];                                                                      /* set pack1 */
    input_buf[4] = 0x00;                                                                         /* set 0x00 */
    input_buf[5] = 0x00;                                                                         /* set 0x00 */
    a_ntag21x_iso14443a_crc(input_buf, 6, input_buf + 6);                                        /* get the crc */
    output_len = 1;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("ntag21x: contactless transceiver failed.\n");                       /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 1)                                                                         /* check the output_len */
    {
        handle->debug_print("ntag21x: output_len is invalid.\n");                                /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    if (output_buf[0] != 0xA)                                                                    /* check the result */
    {
        handle->debug_print("ntag21x: ack error.\n");                                            /* ack error */
        
        return 5;                                                                                /* return error */
    }
    
    return 0;                                                                                    /* success return 0 */
}

/**
 * @brief     ntag21x set the dynamic lock
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] *lock points to a lock buffer
 * @return    status code
 *            - 0 success
 *            - 1 set dynamic lock failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 output_len is invalid
 *            - 5 ack error
 * @note                                                               ntag213                                                                                    
 *                                                                     byte0                                                                                      
 *            bit7               bit6               bit5               bit4               bit3               bit2              bit1              bit0             
 *            lock_page_30_31    lock_page_28_29    lock_page_26_27    lock_page_24_25    lock_page_22_23    lock_page_20_21   lock_page_18_19   lock_page_16_17  
 *                                                                     byte1                                                                                      
 *            bit7               bit6               bit5               bit4               bit3               bit2              bit1              bit0             
 *            0                  0                  0                  0                  lock_page_38_39    lock_page_36_37   lock_page_34_35   lock_page_32_33  
 *                                                                     byte2                                                                                      
 *            bit7               bit6               bit5               bit4               bit3               bit2              bit1              bit0             
 *            0                  0                  block_page_36_39   block_page_32_35   block_page_28_31   block_page_24_27  block_page_20_23  block_page_16_19 
 *                                                                     ntag215                                                                                    
 *                                                                     byte0                                                                                      
 *            bit7               bit6               bit5               bit4               bit3               bit2              bit1              bit0             
 *            lock_page_128_129  lock_page_112_127  lock_page_96_111   lock_page_80_95    lock_page_64_79    lock_page_48_63   lock_page_32_47   lock_page_16_31  
 *                                                                     byte1                                                                                      
 *            bit7               bit6               bit5               bit4               bit3               bit2              bit1              bit0             
 *            0                  0                  0                  0                  0                  0                 0                 0                
 *                                                                     byte2                                                                                      
 *            bit7               bit6               bit5               bit4               bit3               bit2              bit1              bit0             
 *            0                  0                  0                  0                  block_page_112_129 block_page_80_111 block_page_48_79  block_page_16_47 
 *                                                                     ntag216                                                                                    
 *                                                                     byte0                                                                                      
 *            bit7               bit6               bit5               bit4               bit3               bit2              bit1              bit0             
 *            lock_page_128_143  lock_page_112_127  lock_page_96_111   lock_page_80_95    lock_page_64_79    lock_page_48_63   lock_page_32_47   lock_page_16_31  
 *                                                                     byte1                                                                                      
 *            bit7               bit6               bit5               bit4               bit3               bit2              bit1              bit0             
 *            0                  0                  lock_page_224_225  lock_page_208_223  lock_page_192_207  lock_page_176_191 lock_page_160_175 lock_page_144_159
 *                                                                     byte2                                                                                      
 *            bit7               bit6               bit5               bit4               bit3               bit2              bit1              bit0             
 *            0                  block_page_208_225 block_page_176_207 block_page_144_175 block_page_112_143 block_page_80_111 block_page_48_79  block_page_16_47 
 */
uint8_t ntag21x_set_dynamic_lock(ntag21x_handle_t *handle, uint8_t lock[3])
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[8];
    uint8_t output_len;
    uint8_t output_buf[1];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 8;                                                                               /* set the input length */
    input_buf[0] = NTAG21X_COMMAND_WRITE;                                                        /* set the command */
    input_buf[1] = handle->end_page - 4;                                                         /* set the setting page */
    input_buf[2] = lock[0];                                                                      /* set lock0 */
    input_buf[3] = lock[1];                                                                      /* set lock1 */
    input_buf[4] = lock[2];                                                                      /* set lock2 */
    input_buf[5] = 0x00;                                                                         /* set 0x00 */
    a_ntag21x_iso14443a_crc(input_buf, 6, input_buf + 6);                                        /* get the crc */
    output_len = 1;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("ntag21x: contactless transceiver failed.\n");                       /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 1)                                                                         /* check the output_len */
    {
        handle->debug_print("ntag21x: output_len is invalid.\n");                                /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    if (output_buf[0] != 0xA)                                                                    /* check the result */
    {
        handle->debug_print("ntag21x: ack error.\n");                                            /* ack error */
        
        return 5;                                                                                /* return error */
    }
    
    return 0;                                                                                    /* success return 0 */
}

/**
 * @brief      ntag21x get the dynamic lock
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *lock points to a lock buffer
 * @return     status code
 *             - 0 success
 *             - 1 get dynamic lock failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 crc error
 * @note       none
 */
uint8_t ntag21x_get_dynamic_lock(ntag21x_handle_t *handle, uint8_t lock[3])
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[5];
    uint8_t output_len;
    uint8_t output_buf[6];
    uint8_t crc_buf[2];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 5;                                                                               /* set the input length */
    input_buf[0] = NTAG21X_COMMAND_FAST_READ;                                                    /* set the command */
    input_buf[1] = handle->end_page - 4;                                                         /* set the start page */
    input_buf[2] = handle->end_page - 4;                                                         /* set the stop page */
    a_ntag21x_iso14443a_crc(input_buf , 3, input_buf + 3);                                       /* get the crc */
    output_len = 6;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("ntag21x: contactless transceiver failed.\n");                       /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 6)                                                                         /* check the output_len */
    {
        handle->debug_print("ntag21x: output_len is invalid.\n");                                /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    a_ntag21x_iso14443a_crc(output_buf, 4, crc_buf);                                             /* get the crc */
    if ((output_buf[4] == crc_buf[0]) && (output_buf[5] == crc_buf[1]))                          /* check the crc */
    {
        memcpy(lock, output_buf, 3);                                                             /* copy the data */
        
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        handle->debug_print("ntag21x: crc error.\n");                                            /* crc error */
        
        return 5;                                                                                /* return error */
    }
}

/**
 * @brief     ntag21x set the static lock
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] *lock points to a lock buffer
 * @return    status code
 *            - 0 success
 *            - 1 set static lock failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 output_len is invalid
 *            - 5 ack error
 * @note                                                          byte0                                                    
 *            bit7         bit6         bit5         bit4         bit3         bit2             bit1           bit0        
 *            lock_page_7  lock_page_6  lock_page_5  lock_page_4  lock_page_cc block_page_15_10 block_page_9_2 bock_page_cc
 *                                                                byte1                                                    
 *            bit7         bit6         bit5         bit4         bit3         bit2             bit1           bit0        
 *            lock_page_15 lock_page_14 lock_page_13 lock_page_12 lock_page_11 lock_page_10     lock_page_9    lock_page_8 
 */
uint8_t ntag21x_set_static_lock(ntag21x_handle_t *handle, uint8_t lock[2])
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[8];
    uint8_t output_len;
    uint8_t output_buf[1];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 8;                                                                               /* set the input length */
    input_buf[0] = NTAG21X_COMMAND_WRITE;                                                        /* set the command */
    input_buf[1] = 0x02;                                                                         /* set the setting page */
    input_buf[2] = 0x00;                                                                         /* set 0x00 */
    input_buf[3] = 0x00;                                                                         /* set 0x00 */
    input_buf[4] = lock[0];                                                                      /* set lock0 */
    input_buf[5] = lock[1];                                                                      /* set lock1 */
    a_ntag21x_iso14443a_crc(input_buf, 6, input_buf + 6);                                        /* get the crc */
    output_len = 1;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("ntag21x: contactless transceiver failed.\n");                       /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 1)                                                                         /* check the output_len */
    {
        handle->debug_print("ntag21x: output_len is invalid.\n");                                /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    if (output_buf[0] != 0xA)                                                                    /* check the result */
    {
        handle->debug_print("ntag21x: ack error.\n");                                            /* ack error */
        
        return 5;                                                                                /* return error */
    }
    
    return 0;                                                                                    /* success return 0 */
}

/**
 * @brief      ntag21x get the static lock
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *lock points to a lock buffer
 * @return     status code
 *             - 0 success
 *             - 1 get static lock failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 crc error
 * @note       none
 */
uint8_t ntag21x_get_static_lock(ntag21x_handle_t *handle, uint8_t lock[2])
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[5];
    uint8_t output_len;
    uint8_t output_buf[6];
    uint8_t crc_buf[2];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 5;                                                                               /* set the input length */
    input_buf[0] = NTAG21X_COMMAND_FAST_READ;                                                    /* set the command */
    input_buf[1] = 2;                                                                            /* set the start page */
    input_buf[2] = 2;                                                                            /* set the stop page */
    a_ntag21x_iso14443a_crc(input_buf , 3, input_buf + 3);                                       /* get the crc */
    output_len = 6;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("ntag21x: contactless transceiver failed.\n");                       /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 6)                                                                         /* check the output_len */
    {
        handle->debug_print("ntag21x: output_len is invalid.\n");                                /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    a_ntag21x_iso14443a_crc(output_buf, 4, crc_buf);                                             /* get the crc */
    if ((output_buf[4] == crc_buf[0]) && (output_buf[5] == crc_buf[1]))                          /* check the crc */
    {
        memcpy(lock, output_buf + 2, 2);                                                         /* copy the data */
        
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        handle->debug_print("ntag21x: crc error.\n");                                            /* crc error */
        
        return 5;                                                                                /* return error */
    }
}

/**
 * @brief     ntag21x set the mirror
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] mirror is the chip mirror mode
 * @return    status code
 *            - 0 success
 *            - 1 set mirror failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ntag21x_set_mirror(ntag21x_handle_t *handle, ntag21x_mirror_t mirror)
{
    uint8_t res;
    uint8_t conf[4];
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    memset(conf, 0, sizeof(uint8_t) * 4);                                  /* clear the conf */
    res = a_ntag21x_conf_read(handle, handle->end_page - 3, conf);         /* read conf */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("ntag21x: conf read failed.\n");               /* conf read failed */
        
        return 1;                                                          /* return error */
    }
    conf[0] &= ~(3 << 6);                                                  /* clear the settings */
    conf[0] |= mirror << 6;                                                /* set the mirror */
    res = a_ntag21x_conf_write(handle, handle->end_page - 3, conf);        /* write conf */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("ntag21x: conf write failed.\n");              /* conf write failed */
        
        return 1;                                                          /* return error */
    }
    
    return 0;                                                              /* success return 0 */
}

/**
 * @brief      ntag21x get the mirror
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *mirror points to a chip mirror mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get mirror failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ntag21x_get_mirror(ntag21x_handle_t *handle, ntag21x_mirror_t *mirror)
{
    uint8_t res;
    uint8_t conf[4];
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    memset(conf, 0, sizeof(uint8_t) * 4);                                  /* clear the conf */
    res = a_ntag21x_conf_read(handle, handle->end_page - 3, conf);         /* read conf */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("ntag21x: conf read failed.\n");               /* conf read failed */
        
        return 1;                                                          /* return error */
    }
    *mirror = (ntag21x_mirror_t)((conf[0] >> 6) & 0x3);                    /* get the conf */
    
    return 0;                                                              /* success return 0 */
}

/**
 * @brief     ntag21x set the mirror byte
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] byte is the mirror byte
 * @return    status code
 *            - 0 success
 *            - 1 set mirror byte failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ntag21x_set_mirror_byte(ntag21x_handle_t *handle, ntag21x_mirror_byte_t byte)
{
    uint8_t res;
    uint8_t conf[4];
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    memset(conf, 0, sizeof(uint8_t) * 4);                                  /* clear the conf */
    res = a_ntag21x_conf_read(handle, handle->end_page - 3, conf);         /* read conf */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("ntag21x: conf read failed.\n");               /* conf read failed */
        
        return 1;                                                          /* return error */
    }
    conf[0] &= ~(3 << 4);                                                  /* clear the settings */
    conf[0] |= byte << 4;                                                  /* set the byte */
    res = a_ntag21x_conf_write(handle, handle->end_page - 3, conf);        /* write conf */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("ntag21x: conf write failed.\n");              /* conf write failed */
        
        return 1;                                                          /* return error */
    }
    
    return 0;                                                              /* success return 0 */
}

/**
 * @brief      ntag21x get the mirror byte
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *byte points to a mirror byte buffer
 * @return     status code
 *             - 0 success
 *             - 1 get mirror byte failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ntag21x_get_mirror_byte(ntag21x_handle_t *handle, ntag21x_mirror_byte_t *byte)
{
    uint8_t res;
    uint8_t conf[4];
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    memset(conf, 0, sizeof(uint8_t) * 4);                                  /* clear the conf */
    res = a_ntag21x_conf_read(handle, handle->end_page - 3, conf);         /* read conf */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("ntag21x: conf read failed.\n");               /* conf read failed */
        
        return 1;                                                          /* return error */
    }
    *byte = (ntag21x_mirror_byte_t)((conf[0] >> 4) & 0x3);                 /* get the conf */
    
    return 0;                                                              /* success return 0 */
}

/**
 * @brief     ntag21x set the modulation mode
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] mode is the modulation mode
 * @return    status code
 *            - 0 success
 *            - 1 set modulation mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ntag21x_set_modulation_mode(ntag21x_handle_t *handle, ntag21x_modulation_mode_t mode)
{
    uint8_t res;
    uint8_t conf[4];
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    memset(conf, 0, sizeof(uint8_t) * 4);                                  /* clear the conf */
    res = a_ntag21x_conf_read(handle, handle->end_page - 3, conf);         /* read conf */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("ntag21x: conf read failed.\n");               /* conf read failed */
        
        return 1;                                                          /* return error */
    }
    conf[0] &= ~(1 << 2);                                                  /* clear the settings */
    conf[0] |= mode << 2;                                                  /* set the mode */
    res = a_ntag21x_conf_write(handle, handle->end_page - 3, conf);        /* write conf */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("ntag21x: conf write failed.\n");              /* conf write failed */
        
        return 1;                                                          /* return error */
    }
    
    return 0;                                                              /* success return 0 */
}

/**
 * @brief      ntag21x get the modulation mode
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *mode points to a modulation mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get modulation mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ntag21x_get_modulation_mode(ntag21x_handle_t *handle, ntag21x_modulation_mode_t *mode)
{
    uint8_t res;
    uint8_t conf[4];
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    memset(conf, 0, sizeof(uint8_t) * 4);                                  /* clear the conf */
    res = a_ntag21x_conf_read(handle, handle->end_page - 3, conf);         /* read conf */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("ntag21x: conf read failed.\n");               /* conf read failed */
        
        return 1;                                                          /* return error */
    }
    *mode = (ntag21x_modulation_mode_t)((conf[0] >> 2) & 0x1);             /* get the conf */
    
    return 0;                                                              /* success return 0 */
}

/**
 * @brief     ntag21x set the mirror page
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] page is the mirror page
 * @return    status code
 *            - 0 success
 *            - 1 set mirror page failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ntag21x_set_mirror_page(ntag21x_handle_t *handle, uint8_t page)
{
    uint8_t res;
    uint8_t conf[4];
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    memset(conf, 0, sizeof(uint8_t) * 4);                                  /* clear the conf */
    res = a_ntag21x_conf_read(handle, handle->end_page - 3, conf);         /* read conf */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("ntag21x: conf read failed.\n");               /* conf read failed */
        
        return 1;                                                          /* return error */
    }
    conf[2] = page;                                                        /* set the page */
    res = a_ntag21x_conf_write(handle, handle->end_page - 3, conf);        /* write conf */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("ntag21x: conf write failed.\n");              /* conf write failed */
        
        return 1;                                                          /* return error */
    }
    
    return 0;                                                              /* success return 0 */
}

/**
 * @brief      ntag21x get the mirror page
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *page points to a mirror page buffer
 * @return     status code
 *             - 0 success
 *             - 1 get mirror page failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ntag21x_get_mirror_page(ntag21x_handle_t *handle, uint8_t *page)
{
    uint8_t res;
    uint8_t conf[4];
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    memset(conf, 0, sizeof(uint8_t) * 4);                                  /* clear the conf */
    res = a_ntag21x_conf_read(handle, handle->end_page - 3, conf);         /* read conf */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("ntag21x: conf read failed.\n");               /* conf read failed */
        
        return 1;                                                          /* return error */
    }
    *page = conf[2];                                                       /* get the page */
    
    return 0;                                                              /* success return 0 */
}

/**
 * @brief     ntag21x set the start page of protection
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] page is the start page
 * @return    status code
 *            - 0 success
 *            - 1 set protect start page failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ntag21x_set_protect_start_page(ntag21x_handle_t *handle, uint8_t page)
{
    uint8_t res;
    uint8_t conf[4];
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    memset(conf, 0, sizeof(uint8_t) * 4);                                  /* clear the conf */
    res = a_ntag21x_conf_read(handle, handle->end_page - 3, conf);         /* read conf */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("ntag21x: conf read failed.\n");               /* conf read failed */
        
        return 1;                                                          /* return error */
    }
    conf[3] = page;                                                        /* set the page */
    res = a_ntag21x_conf_write(handle, handle->end_page - 3, conf);        /* write conf */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("ntag21x: conf write failed.\n");              /* conf write failed */
        
        return 1;                                                          /* return error */
    }
    
    return 0;                                                              /* success return 0 */
}

/**
 * @brief      ntag21x get the start page of protection
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *page points to a start page buffer
 * @return     status code
 *             - 0 success
 *             - 1 get protect start page failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ntag21x_get_protect_start_page(ntag21x_handle_t *handle, uint8_t *page)
{
    uint8_t res;
    uint8_t conf[4];
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    memset(conf, 0, sizeof(uint8_t) * 4);                                  /* clear the conf */
    res = a_ntag21x_conf_read(handle, handle->end_page - 3, conf);         /* read conf */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("ntag21x: conf read failed.\n");               /* conf read failed */
        
        return 1;                                                          /* return error */
    }
    *page = conf[3];                                                       /* get the page */
    
    return 0;                                                              /* success return 0 */
}

/**
 * @brief     ntag21x enable or disable access
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] access is the set access
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set access failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ntag21x_set_access(ntag21x_handle_t *handle, ntag21x_access_t access, ntag21x_bool_t enable)
{
    uint8_t res;
    uint8_t conf[4];
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    memset(conf, 0, sizeof(uint8_t) * 4);                                  /* clear the conf */
    res = a_ntag21x_conf_read(handle, handle->end_page - 2, conf);         /* read conf */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("ntag21x: conf read failed.\n");               /* conf read failed */
        
        return 1;                                                          /* return error */
    }
    conf[0] &= ~(1 << access);                                             /* clear the settings */
    conf[0] |= enable << access;                                           /* set the access */
    res = a_ntag21x_conf_write(handle, handle->end_page - 2, conf);        /* write conf */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("ntag21x: conf write failed.\n");              /* conf write failed */
        
        return 1;                                                          /* return error */
    }
    
    return 0;                                                              /* success return 0 */
}

/**
 * @brief      ntag21x get the access status
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[in]  access is the set access
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get access failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ntag21x_get_access(ntag21x_handle_t *handle, ntag21x_access_t access, ntag21x_bool_t *enable)
{
    uint8_t res;
    uint8_t conf[4];
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    memset(conf, 0, sizeof(uint8_t) * 4);                                  /* clear the conf */
    res = a_ntag21x_conf_read(handle, handle->end_page - 2, conf);         /* read conf */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("ntag21x: conf read failed.\n");               /* conf read failed */
        
        return 1;                                                          /* return error */
    }
    *enable = (ntag21x_bool_t)((conf[0] >> access) & 0x1);                 /* get the bool */
    
    return 0;                                                              /* success return 0 */
}

/**
 * @brief     ntag21x set the authenticate limitation
 * @param[in] *handle points to an ntag21x handle structure
 * @param[in] limit is the authenticate limitation
 * @return    status code
 *            - 0 success
 *            - 1 set authenticate limitation failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 limit > 7
 * @note      none
 */
uint8_t ntag21x_set_authenticate_limitation(ntag21x_handle_t *handle, uint8_t limit)
{
    uint8_t res;
    uint8_t conf[4];
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    if (limit > 7)                                                         /* check the limit */
    {
        handle->debug_print("ntag21x: limit > 7.\n");                      /* limit > 7 */
        
        return 4;                                                          /* return error */
    }
    
    memset(conf, 0, sizeof(uint8_t) * 4);                                  /* clear the conf */
    res = a_ntag21x_conf_read(handle, handle->end_page - 2, conf);         /* read conf */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("ntag21x: conf read failed.\n");               /* conf read failed */
        
        return 1;                                                          /* return error */
    }
    conf[0] &= ~(7 << 0);                                                  /* clear the settings */
    conf[0] |= limit << 0;                                                 /* set the limit */
    res = a_ntag21x_conf_write(handle, handle->end_page - 2, conf);        /* write conf */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("ntag21x: conf write failed.\n");              /* conf write failed */
        
        return 1;                                                          /* return error */
    }
    
    return 0;                                                              /* success return 0 */
}

/**
 * @brief      ntag21x get the authenticate limitation
 * @param[in]  *handle points to an ntag21x handle structure
 * @param[out] *limit points to an authenticate limitation buffer
 * @return     status code
 *             - 0 success
 *             - 1 get authenticate limitation failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ntag21x_get_authenticate_limitation(ntag21x_handle_t *handle, uint8_t *limit)
{
    uint8_t res;
    uint8_t conf[4];
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    memset(conf, 0, sizeof(uint8_t) * 4);                                  /* clear the conf */
    res = a_ntag21x_conf_read(handle, handle->end_page - 2, conf);         /* read conf */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("ntag21x: conf read failed.\n");               /* conf read failed */
        
        return 1;                                                          /* return error */
    }
    *limit = conf[0] & 0x7;                                                /* set the limit */
    
    return 0;                                                              /* success return 0 */
}

/**
 * @brief         transceiver data
 * @param[in]     *handle points to an ntag21x handle structure
 * @param[in]     *in_buf points to an input buffer
 * @param[in]     in_len is the input length
 * @param[out]    *out_buf points to an output buffer
 * @param[in,out] *out_len points to an output length buffer
 * @return        status code
 *                - 0 success
 *                - 1 transceiver failed
 * @note          none
 */
uint8_t ntag21x_transceiver(ntag21x_handle_t *handle, uint8_t *in_buf, uint8_t in_len, uint8_t *out_buf, uint8_t *out_len)
{
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->inited != 1)                                           /* check handle initialization */
    {
        return 3;                                                      /* return error */
    }
    
    if (handle->contactless_transceiver(in_buf, in_len, 
                                        out_buf, out_len) != 0)        /* transceiver data */
    {
        return 1;                                                      /* return error */
    }
    else
    {
        return 0;                                                      /* success return 0 */
    }
}

/**
 * @brief      get chip information
 * @param[out] *info points to an ntag21x info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ntag21x_info(ntag21x_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(ntag21x_info_t));                        /* initialize ntag21x info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "RF", 8);                              /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
