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
 * >> 4.0 kHz
 *
 * Load Test :: End Point Resistance / Max Voltage / Pulse Step
 * >> 50 kΩ / 35.31v / 29(max)
 *
 * No Load Test :: End Point Resistance / Max Voltage / Pulse Step
 * >> In-Ear 0 Ω / 42.47v / 29(max)
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
{ 53630, 287, 1358 },
{ 53730, 288, 1362 },
{ 53800, 290, 1378 },
{ 59650, 331, 1534 },
{ 118300, 725, 3244 },

/* Pulse Step Value : 5 ~ 9 */
{ 134300, 825, 3696 },
{ 147000, 909, 4049 },
{ 159300, 993, 4403 },
{ 171500, 1077, 4752 },
{ 185400, 1164, 5144 },

/* Pulse Step Value : 10 ~ 14 */
{ 197600, 1248, 5492 },
{ 209700, 1333, 5860 },
{ 222100, 1416, 6203 },
{ 234200, 1502, 6560 },
{ 247100, 1583, 6915 },

/* Pulse Step Value : 15 ~ 19 */
{ 259700, 1671, 7275 },
{ 271400, 1750, 7612 },
{ 283900, 1834, 7975 },
{ 296200, 1913, 8335 },
{ 308400, 1994, 8674 },

/* Pulse Step Value : 20 ~ 24 */
{ 320500, 2080, 9162 },
{ 333300, 2163, 9604 },
{ 345600, 2242, 9948 },
{ 357200, 2325, 10290 },
{ 369600, 2406, 10630 },

/* Pulse Step Value : 25 ~ 29 */
{ 382000, 2484, 10970 },
{ 394000, 2564, 11310 },
{ 406000, 2643, 11680 },
{ 417700, 2726, 12000 },
{ 424700, 2762, 12190 } };
#endif

#ifdef STIM_LIB_EVKIT_CV
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
