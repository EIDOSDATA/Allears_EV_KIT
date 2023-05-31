/*
 * stim_lib_st_inc_st.c
 *
 *  Created on: May 9, 2023
 *      Author: eidos
 */
#include "stim_lib_st_inc.h"
#include "stim_lib_state.h"
#include "stim_lib_stim_cfg.h"
#include "stim_lib_st_init.h"
#include "stim_lib_common.h"

#define STIM_LIB_PLUS_OUTPUT_OFF_TIME()						for (int i = 0; i < 0xffff; i++) /* Delay */

void stimLib_stimStopDelayRaw(void)
{
	if (stimLib_stateGet() == stim_lib_state_stimulating)
	{
		/*
		 * I had a problem with not going straight to LOW when ending the pulse.
		 * At the end of the pulse, I change the OC mode immediately to drop the GPIO to the LOW level.
		 * */
		TIM2->CCMR1 &= ~(TIM_CCMR1_OC1M | TIM_CCMR1_OC2M);
		TIM2->CCMR1 |= (TIM_OCMODE_FORCED_INACTIVE) | (TIM_OCMODE_FORCED_INACTIVE << 8U);

		TIM2->CCMR2 &= ~(TIM_CCMR2_OC3M | TIM_CCMR2_OC4M);
		TIM2->CCMR2 |= (TIM_OCMODE_FORCED_INACTIVE) | (TIM_OCMODE_FORCED_INACTIVE << 8U);

		/*
		 * Since it must be used not only in the polling method but also inside the interrupt, the Delay function is avoided and replaced with a For statement.
		 * stimLib_delay(STIM_LIB_PLUS_OUTPUT_OFF_TIME);
		 * */
		STIM_LIB_PLUS_OUTPUT_OFF_TIME();

		/*
		 * TIM2 CH3 DMA TC INTERRUPT ENABLE
		 * */
		__HAL_DMA_ENABLE_IT(&hdma_tim2_ch3, (DMA_IT_TC));

		stimLib_stateSet(stim_lib_state_stim_stopping);
	}
}

