/*
 * stim_lib_volt_table.c
 *
 *  Created on: Apr 27, 2023
 *      Author: eidos
 */
#include "stim_lib_volt.h"

#ifdef STIM_LIB_EVKIT_CC
/*
 * -----------------------------
 * Measurement Environment
 * >> Measured with 0 Ω termination resistance.
 * -----------------------------
 * */
/*
 * SYSTEM CLOCK : 80MHz
 *
 * ADC SAMPLING TIME
 * >> hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;
 * >> sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
 *
 *  VOLTAGE DIV CIRCUIT RESISTANCE
 * >> R1 : 3.6 MΩ, R2 : 110 kΩ
 *
 * STEP UP CIRCUIT INPUT VOLTAGE
 * >> 5.0v
 *
 * STEP UP PULSE TIMER
 * >> 25.0 kHz
 *
 * Load Test :: End Point Resistance / Max Voltage / Pulse Step
 * >> VENUS CHIP (Ω) / 42.59v / 29(max)
 *
 * No Load Test :: End Point Resistance / Max Voltage / Pulse Step
 * >> VENUS CHIP (Ω) / 42.59v / 29(max)s
 *
 * Circuit Inductor Capacity
 * >> 6.8uH
 *
 * Maximum Pulse Step : 30 Step (0 ~ 29)
 * */
const stim_lib_stepup_ref_value_t exStimLib_stepup_table[STIM_LIB_STEPUP_TABLE_SIZE] =
{
/* Output Voltage, Feed Back ADC, Feed Back Voltage */
/* Pulse Step Value : 0 ~ 4 */
{ 47910, 243, 1193 },
{ 48190, 245, 1194 },
{ 48530, 247, 1195 },
{ 48910, 248, 1198 },
{ 109400, 648, 2924 },

/* Pulse Step Value : 5 ~ 9 */
{ 126900, 764, 3417 },
{ 138800, 847, 3764 },
{ 151800, 933, 4118 },
{ 165700, 1015, 4465 },
{ 178700, 1101, 4816 },

/* Pulse Step Value : 10 ~ 14 */
{ 191000, 1185, 5181 },
{ 203000, 1268, 5461 },
{ 215300, 1351, 5864 },
{ 227600, 1436, 6228 },
{ 239700, 1518, 6578 },

/* Pulse Step Value : 15 ~ 19 */
{ 251600, 1604, 6915 },
{ 264500, 1685, 7267 },
{ 277600, 1769, 7614 },
{ 289700, 1849, 7954 },
{ 301800, 1931, 8314 },

/* Pulse Step Value : 20 ~ 24 */
{ 314600, 2015, 8576 },
{ 327400, 2125, 9443 },
{ 339400, 2203, 9781 },
{ 351200, 2284, 10030 },
{ 363700, 2366, 10450 },

/* Pulse Step Value : 25 ~ 29 */
{ 375700, 2450, 10790 },
{ 387600, 2533, 11120 },
{ 399600, 2612, 11480 },
{ 411500, 2691, 11820 },
{ 424100, 2746, 12070 }, };
#endif

#ifdef STIM_LIB_EVKIT_CV
#if 0
/*
 * -----------------------------
 * Measurement Environment
 * >> Measurement with 50 kΩ Termination Resistance.
 * -----------------------------
 * */
/*
 * SYSTEM CLOCK : 80MHz
 *
 * ADC SAMPLING TIME
 * >> hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;
 * >> sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
 *
 *  VOLTAGE DIV CIRCUIT RESISTANCE
 * >> R1 : 3.6 MΩ, R2 : 110 kΩ
 *
 * STEP UP CIRCUIT INPUT VOLTAGE
 * >> 5.3v
 *
 * STEP UP PULSE TIMER
 * >> 10.0 kHz
 *
 * Load Test :: End Point Resistance / Max Voltage / Pulse Step
 * >> 50 kΩ / 45.77v / 29(max)
 *
 * No Load Test :: End Point Resistance / Max Voltage / Pulse Step
 * >> In-Ear 0 Ω / 49.03v / 29(max)
 *
 * Circuit Inductor Capacity
 * >> 6.8uH and 10.0uH
 *
 * Maximum Pulse Step : 30 Step (0 ~ 29)
 * */
const stim_lib_stepup_ref_value_t exStimLib_stepup_table[STIM_LIB_STEPUP_TABLE_SIZE] =
{
/* Output Voltage, Feed Back ADC, Feed Back Voltage */
/* Pulse Step Value : 0 ~ 4 */
{ 53820, 290, 1375 },
{ 53820, 290, 1375 },
{ 53820, 290, 1375 },
{ 62840, 350, 1629 },
{ 131900, 813, 3636 },

/* Pulse Step Value : 5 ~ 9 */
{ 149400, 930, 4134 },
{ 164400, 1028, 4552 },
{ 179000, 1126, 4976 },
{ 193600, 1224, 5391 },
{ 207700, 1322, 5824 },

/* Pulse Step Value : 10 ~ 14 */
{ 222500, 1420, 6226 },
{ 236900, 1519, 6634 },
{ 251100, 1617, 7046 },
{ 265200, 1713, 7456 },
{ 280100, 1807, 7860 },

/* Pulse Step Value : 15 ~ 19 */
{ 294300, 1908, 8289 },
{ 308700, 2002, 8689 },
{ 322900, 2103, 9309 },
{ 337300, 2195, 9735 },
{ 351400, 2293, 10140 },

/* Pulse Step Value : 20 ~ 24 */
{ 365500, 2386, 10530 },
{ 373100, 2484, 10740 },
{ 394800, 2576, 11350 },
{ 408500, 2671, 11750 },
{ 422600, 2764, 12140 },

/* Pulse Step Value : 25 ~ 29 */
{ 435900, 2860, 12530 },
{ 449600, 2947, 12930 },
{ 461800, 3033, 13300 },
{ 461000, 3030, 13270 },
{ 457700, 3004, 13170 }, };
#endif

#if 1

/*
 * -----------------------------
 * Measurement Environment
 * >> Measurement with 50 kΩ Termination Resistance.
 * -----------------------------
 * */
/*
 * SYSTEM CLOCK : 80MHz
 *
 * ADC SAMPLING TIME
 * >> hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;
 * >> sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
 *
 *  VOLTAGE DIV CIRCUIT RESISTANCE
 * >> R1 : 3.6 MΩ, R2 : 110 kΩ
 *
 * STEP UP CIRCUIT INPUT VOLTAGE
 * >> 5.3v
 *
 * STEP UP PULSE TIMER
 * >> 9.0 kHz
 *
 * Load Test :: End Point Resistance / Max Voltage / Pulse Step
 * >> 50 kΩ / 45.24v / 29(max)
 *
 * No Load Test :: End Point Resistance / Max Voltage / Pulse Step
 * >> In-Ear 0 Ω / 46.54v / 29(max)
 *
 * Circuit Inductor Capacity
 * >> 6.8uH and 10.0uH
 *
 * Maximum Pulse Step : 30 Step (0 ~ 29)
 * */
const stim_lib_stepup_ref_value_t exStimLib_stepup_table[STIM_LIB_STEPUP_TABLE_SIZE] =
{
/* Output Voltage, Feed Back ADC, Feed Back Voltage */
/* Pulse Step Value : 0 ~ 4 */
{ 53780, 287, 1450 },
{ 53830, 288, 1454 },
{ 53850, 289, 1456 },
{ 53850, 289, 1462 },
{ 125700, 760, 3497 },

/* Pulse Step Value : 5 ~ 9 */
{ 147600, 907, 4109 },
{ 163100, 1010, 4541 },
{ 177800, 1111, 4981 },
{ 194100, 1214, 5436 },
{ 209300, 1318, 5865 },

/* Pulse Step Value : 10 ~ 14 */
{ 224900, 1419, 6289 },
{ 239700, 1522, 6720 },
{ 254100, 1623, 7150 },
{ 269500, 1726, 7596 },
{ 285400, 1826, 8033 },

/* Pulse Step Value : 15 ~ 19 */
{ 299600, 1927, 8452 },
{ 315800, 2028, 8889 },
{ 331500, 2133, 9525 },
{ 344600, 2226, 9918 },
{ 359700, 2323, 10330 },

/* Pulse Step Value : 20 ~ 24 */
{ 374800, 2425, 10760 },
{ 390200, 2522, 11180 },
{ 404900, 2622, 11620 },
{ 419600, 2718, 12040 },
{ 426000, 2768, 12220 },

/* Pulse Step Value : 25 ~ 29 */
{ 423400, 2746, 12150 },
{ 422400, 2743, 12120 },
{ 423700, 2754, 12150 },
{ 437500, 2838, 12550 },
{ 452400, 2939, 12980 }, };

#endif

#if 0
/*
 * -----------------------------
 * Measurement Environment
 * >> Measured with 0 Ω termination resistance.
 * -----------------------------
 * */
/*
 * SYSTEM CLOCK : 80MHz
 *
 * ADC SAMPLING TIME
 * >> hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;
 * >> sConfig.SamplingTime = ADC_SAMPLETIME_47CYCLES_5;
 *
 *  VOLTAGE DIV CIRCUIT RESISTANCE
 * >> R1 : 3.6 MΩ, R2 : 110 kΩ
 *
 * STEP UP CIRCUIT INPUT VOLTAGE
 * >> 5.3v
 *
 * STEP UP PULSE TIMER
 * >> 4.0 kHz
 *
 * Load Test :: End Point Resistance / Max Voltage / Pulse Step
 * >> 50 kΩ / 35.31v / 29(max)
 *
 * No Load Test :: End Point Resistance / Max Voltage / Pulse Step
 * >> In-Ear 0 Ω / 42.49v / 29(max)
 *
 * Circuit Inductor Capacity
 * >> 6.8uH
 *
 * Maximum Pulse Step : 30 Step (0 ~ 29)
 * */
const stim_lib_stepup_ref_value_t exStimLib_stepup_table[STIM_LIB_STEPUP_TABLE_SIZE] =
{
/* Output Voltage, Feed Back ADC, Feed Back Voltage */
/* Pulse Step Value : 0 ~ 4 */
{ 53650, 194, 940 },
{ 53650, 194, 940 },
{ 53650, 194, 940 },
{ 59680, 224, 1059 },
{ 117700, 558, 2505 },

/* Pulse Step Value : 5 ~ 9 */
{ 133100, 631, 2808 },
{ 145600, 693, 3052 },
{ 158600, 755, 3326 },
{ 170400, 808, 3552 },
{ 183700, 878, 3829 },

/* Pulse Step Value : 10 ~ 14 */
{ 197500, 933, 4094 },
{ 209200, 1001, 4355 },
{ 220200, 1058, 4586 },
{ 234000, 1121, 4875 },
{ 246900, 1193, 5147 },

/* Pulse Step Value : 15 ~ 19 */
{ 258400, 1259, 5397 },
{ 271300, 1312, 5661 },
{ 283600, 1385, 5914 },
{ 295600, 1444, 6163 },
{ 309000, 1503, 6416 },

/* Pulse Step Value : 20 ~ 24 */
{ 320400, 1551, 6676 },
{ 333300, 1615, 6905 },
{ 345200, 1674, 7167 },
{ 357200, 1730, 7425 },
{ 369000, 1785, 7670 },

/* Pulse Step Value : 25 ~ 29 */
{ 382100, 1845, 7908 },
{ 394100, 1915, 8148 },
{ 406000, 1955, 8399 },
{ 418200, 2394, 10510 },
{ 424900, 2450, 10660 }, };
#endif

#endif
