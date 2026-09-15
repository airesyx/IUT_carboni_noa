#ifndef ADC_H
#define	ADC_H

void InitADC1(void);
unsigned int * ADCGetResult(void);
void ADC1StartConversionSequence();
void ADCClearConversionFinishedFlag(void);

#endif	/* ADC_H */

