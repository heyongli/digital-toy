#ifndef _METAL_IO_H_
#define _METAL_IO_H_

/*
 *   PIN DEFINE, CLI/sti, inp, outp
 */
#include <config.h>

#ifdef ARCH_AVR
#include <avr/io.h> 
#include <atmel/avr-io.h>
#endif 

#ifdef ARCH_C51
#include <at89x52.h>
#include <intrins.h>
#include <c51/c51-io.h> //AVR gcc delay libs
#endif 

#endif
