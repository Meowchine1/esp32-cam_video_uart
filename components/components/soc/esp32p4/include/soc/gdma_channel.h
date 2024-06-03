/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

// The following macros have a format SOC_[periph][instance_id] to make it work with `GDMA_MAKE_TRIGGER`
#define SOC_GDMA_TRIG_PERIPH_M2M0     -1
#define SOC_GDMA_TRIG_PERIPH_I3C0     0
#define SOC_GDMA_TRIG_PERIPH_UHCI0    2
#define SOC_GDMA_TRIG_PERIPH_I2S0     3
#define SOC_GDMA_TRIG_PERIPH_I2S1     4
#define SOC_GDMA_TRIG_PERIPH_I2S2     5
#define SOC_GDMA_TRIG_PERIPH_ADC0     8
#define SOC_GDMA_TRIG_PERIPH_RMT0     10
#define SOC_GDMA_TRIG_PERIPH_LCD0     0
#define SOC_GDMA_TRIG_PERIPH_CAM0     0
#define SOC_GDMA_TRIG_PERIPH_SPI2     1
#define SOC_GDMA_TRIG_PERIPH_SPI3     2
#define SOC_GDMA_TRIG_PERIPH_PARLIO0  3
#define SOC_GDMA_TRIG_PERIPH_AES0     4
#define SOC_GDMA_TRIG_PERIPH_SHA0     5

// On which system bus is the DMA instance of the peripheral connection mounted
#define SOC_GDMA_BUS_ANY -1
#define SOC_GDMA_BUS_AHB 0
#define SOC_GDMA_BUS_AXI 1

#define SOC_GDMA_TRIG_PERIPH_M2M0_BUS     SOC_GDMA_BUS_ANY
#define SOC_GDMA_TRIG_PERIPH_UHCI0_BUS    SOC_GDMA_BUS_AHB
#define SOC_GDMA_TRIG_PERIPH_I2S0_BUS     SOC_GDMA_BUS_AHB
#define SOC_GDMA_TRIG_PERIPH_I2S1_BUS     SOC_GDMA_BUS_AHB
#define SOC_GDMA_TRIG_PERIPH_I2S2_BUS     SOC_GDMA_BUS_AHB
#define SOC_GDMA_TRIG_PERIPH_ADC0_BUS     SOC_GDMA_BUS_AHB
#define SOC_GDMA_TRIG_PERIPH_RMT0_BUS     SOC_GDMA_BUS_AHB
#define SOC_GDMA_TRIG_PERIPH_I3C0_BUS     SOC_GDMA_BUS_AHB
#define SOC_GDMA_TRIG_PERIPH_SPI2_BUS     SOC_GDMA_BUS_AXI
#define SOC_GDMA_TRIG_PERIPH_SPI3_BUS     SOC_GDMA_BUS_AXI
#define SOC_GDMA_TRIG_PERIPH_LCD0_BUS     SOC_GDMA_BUS_AXI
#define SOC_GDMA_TRIG_PERIPH_CAM0_BUS     SOC_GDMA_BUS_AXI
#define SOC_GDMA_TRIG_PERIPH_AES0_BUS     SOC_GDMA_BUS_AXI
#define SOC_GDMA_TRIG_PERIPH_SHA0_BUS     SOC_GDMA_BUS_AXI
#define SOC_GDMA_TRIG_PERIPH_PARLIO0_BUS  SOC_GDMA_BUS_AXI
