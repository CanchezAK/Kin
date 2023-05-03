#include "meter.h"

volatile uint16_t m_cnt_prev = 0;

volatile uint16_t m_capt = 0;

volatile uint16_t n_cnt_prev = 0;

volatile uint16_t n_capt = 0;

volatile uint32_t m_corr = 0;

volatile uint32_t m_capt_corr = 0;

volatile uint32_t n_corr = 0;

volatile uint32_t n_capt_corr = 0;


volatile DBuf dbuf;

void _update_fm_vars()
{
    uint32_t m_ccr1 = TIM1->CCR1;
    uint32_t n_ccr1 = TIM2->CCR1;

    uint32_t m_cnt = (uint16_t)(TIM1->CNT - 1);
    uint32_t n_cnt = TIM2->CNT;

    bool m_ovf = m_cnt < m_cnt_prev;
    bool n_ovf = n_cnt < n_cnt_prev;

    m_cnt_prev = m_cnt;
    n_cnt_prev = n_cnt;

    bool captf = (m_ccr1 != m_capt);

    if(captf)
    {
        m_capt = m_ccr1;
        n_capt = n_ccr1;
        m_capt_corr = m_corr;
        n_capt_corr = n_corr;
        if(m_ovf && m_ccr1 <= m_cnt)
            m_capt_corr += CNT_OVR;
        if(n_ovf && n_ccr1 <= n_cnt)
            n_capt_corr += CNT_OVR;
    }

    if(m_ovf)
        m_corr += CNT_OVR;
    if(n_ovf)
        n_corr += CNT_OVR;
}


void fm_init()
{
    TIM1->CR2 |= 3 << 4;    // MMS=011 ==> TRGO = Compare pulse
    TIM1->SMCR |= (5 << 4) | 7;     // TS=101 (TI1FP1) | SMS=111 (External mode clock 1)
    TIM1->CCMR1 |= 1;       // CC1S=01, IC1=TI1
    // CC1S=11: IC1=TRC=TRGI0 (SMCR_TS=00 ==> TRC=TRGIO)
    TIM2->CCMR1 |= 3;
    TIM3->ARR = 0x7FFF;

}

void fm_stop()
{
    TIM4->CR1 &= ~TIM_CR1_CEN;
    TIM3->CR1 &= ~TIM_CR1_CEN;
    TIM1->CCER &= ~TIM_CCER_CC1E;
    TIM1->CR1 &= ~TIM_CR1_CEN;
    TIM2->CCER &= ~TIM_CCER_CC1E;
    TIM2->CR1 &= ~TIM_CR1_CEN;

    TIM3->DIER &= ~TIM_DIER_UIE;
    TIM4->DIER &= ~TIM_DIER_UIE;
}

bool fm_start(uint32_t timer_div1, uint32_t timer_div2, uint32_t k)
{
    if(timer_div1 - 1 > 0xFFFF || timer_div2 - 2 > 0xFFFE || k == 0)
    {
        return false;
    }
    
    if(TIM1->CCER & TIM_CCER_CC1E)
    {
        fm_stop();
    }
    m_cnt_prev = 0;
    m_capt = 0;
    m_corr = 0;
    m_capt_corr = 0;

    n_cnt_prev = 0;
    n_capt = 0;
    n_corr = 0;
    n_capt_corr = 0;

    dbuf.reset(k + 1);

    TIM2->CNT = 0;
    TIM2->CR1 |= TIM_CR1_CEN;
    TIM2->CCER |= TIM_CCER_CC1E;

    TIM1->CNT = 1;
    TIM1->CR1 |= TIM_CR1_CEN;
    TIM1->CCER |= TIM_CCER_CC1E;

    TIM3->CNT = 0;
    TIM3->CR1 |= TIM_CR1_CEN;

    TIM4->PSC = timer_div1 - 1;
    TIM4->EGR = TIM_EGR_UG;
    TIM4->ARR = timer_div2 - 1;
    TIM4->CR1 |= TIM_CR1_CEN;

    TIM3->DIER |= TIM_DIER_UIE;
    TIM4->SR = ~TIM_SR_UIF;
    TIM4->DIER |= TIM_DIER_UIE;

    return true;
}

bool get_result(uint32_t &m, uint32_t &n, uint32_t &cnt, uint32_t &c0)
{
    m = 0;
    n = 0;
    cnt = dbuf.get_cnt();
    c0 = dbuf.c0;
    bool r = dbuf.is_ready;
    if(r)
    {
        __disable_irq();
        cnt = dbuf.get_cnt();
        c0 = dbuf.c0;
        volatile const Data *p1 = dbuf.first();
        volatile const Data *p2 = dbuf.last();
        m = p2->m - p1->m;
        n = p2->n - p1->n;
        __enable_irq();
    }
    return r;
}