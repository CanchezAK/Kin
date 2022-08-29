#include "main.h"
/* Choose filter order */
#define FILTER_SMA_ORDER 5
#define STOPPER   0 /* Smaller than any datum */
#define MEDIAN_FILTER_SIZE 5
/**
  * @brief Simple Moving Average (SMA) filter.
  * @note Before use define filter order.
  * @param[in] Input raw (unfiltered) value.
  * @retval Return filtered data.
  */
unsigned long Filter_SMA(unsigned int For_Filtered);
unsigned long MedianFilter(unsigned int datum);