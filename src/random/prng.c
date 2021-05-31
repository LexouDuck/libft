
#include "libccc/memory.h"
#include "libccc/sys/time.h"
#include "libccc/math/fixed.h"
#include "libccc/math/float.h"
#include "libccc/random/prng.h"

#include LIBCONFIG_HANDLE_INCLUDE



t_u32   PRNG_Shuffle(t_u32 n);
t_u32   PRNG_U32(t_prng* state);



inline
t_u32   PRNG_Shuffle(t_u32 n)
{
	n = (n << 12) ^ (n >> 20);
	return (36969 * (n & 0xFFFF) + (n >> 16));
}

inline
t_u32   PRNG_U32(t_prng* state)
{
	*state = ((CEIL_SQRT_MOD * PRNG_Shuffle(*state) + OFFSET) & MODULUS);
	return (*state);
}

inline
void    PRNG_SetSeed(t_prng* state, t_u32 seed)
{
	*state = seed;
}

inline
void    PRNG_NewSeed(t_prng* state)
{
	*state = PRNG_Shuffle(Time_Now());
}



t_prng* PRNG_New(void)
{
	t_prng* result;

	result = (t_prng*)Memory_Allocate(sizeof(t_prng));
	HANDLE_ERROR(ALLOCFAILURE, (result == NULL), return (NULL);)
	PRNG_SetSeed(result, DEFAULT_SEED);
	return (result);
}

void    PRNG_Delete(t_prng* *a_state)
{
	HANDLE_ERROR(NULLPOINTER, (a_state == NULL), return;)
	Memory_Free(*a_state);
	*a_state = NULL;
}



e_cccerror  PRNG_Next(t_prng* state, void* dest, t_size n)
{
	t_u8*   buffer = (t_u8*)dest;
	t_u32   random = 0;
	t_size  i = 0;

	HANDLE_ERROR(NULLPOINTER, (dest == NULL), return (ERROR_NULLPOINTER);)
	HANDLE_ERROR(NULLPOINTER, (state == NULL), return (ERROR_NULLPOINTER);)
	while (i < n)
	{
		random = PRNG_U32(state);
		buffer[i] = (random >>  0) & 0xFF;	if (i++ == n)   break;
		buffer[i] = (random >>  8) & 0xFF;	if (i++ == n)   break;
		buffer[i] = (random >> 16) & 0xFF;	if (i++ == n)   break;
		buffer[i] = (random >> 24) & 0xFF;	if (i++ == n)   break;
	}
	return (OK);
}


#define PRNG_INIT_STATE() \
	t_prng* state;                                           \
	state = PRNG_New();                                      \
	HANDLE_ERROR(ALLOCFAILURE, (state == NULL), return (0);) \
	PRNG_NewSeed(state);                                     \


void*	PRNG_Get(void* dest, t_size size)
{
	HANDLE_ERROR(NULLPOINTER, (dest == NULL), return (0);)
	PRNG_INIT_STATE()
	if (PRNG_Next(state, dest, size)) return (0);
	PRNG_Delete(&state);
	return (dest);
}

#define DEFINE_PRNG(TYPE, ACTION_ERROR) \
	TYPE	result = 0;			\
	if (PRNG_Next(state,		\
		&result, sizeof(TYPE)))	\
		ACTION_ERROR			\

inline t_uint   PRNG_UInt (t_prng* state)	{ DEFINE_PRNG(t_uint,	return (0);)	return (result); }
inline t_sint   PRNG_SInt (t_prng* state)	{ DEFINE_PRNG(t_sint,	return (0);)	return (result); }
inline t_fixed  PRNG_Fixed(t_prng* state)	{ DEFINE_PRNG(t_fixed,	return (0);)	return (result); }
inline t_float  PRNG_Float(t_prng* state)
{
	t_float	result = NAN;
	while (isnan(result))
	{
		if (PRNG_Next(state, &result, sizeof(t_float)))
			return (NAN);
	}
	return (result);
}



#define DEFINE_PRNG_RANGE(ACTION_ERROR) \
	if (min == max)				\
		return (min);			\
	HANDLE_ERROR(INVALIDRANGE,	\
		(min > max),			\
		ACTION_ERROR)			\

t_uint  PRNG_UInt_Range     (t_prng* state, t_uint  min, t_uint  max)  { DEFINE_PRNG_RANGE(return (0);)	return (         (PRNG_UInt(state) % (max - min)) + min); }
t_sint  PRNG_SInt_Range     (t_prng* state, t_sint  min, t_sint  max)  { DEFINE_PRNG_RANGE(return (0);)	return (         (PRNG_SInt(state) % (max - min)) + min); }
t_fixed PRNG_Fixed_Range    (t_prng* state, t_fixed min, t_fixed max)  { DEFINE_PRNG_RANGE(return (0);)	return (Fixed_Mod(PRNG_Fixed(state), (max - min)) + min); }
t_float PRNG_Float_Range    (t_prng* state, t_float min, t_float max)  { DEFINE_PRNG_RANGE(return (0);)	return (Float_Mod(PRNG_Float(state), (max - min)) + min); }

t_uint  PRNG_UInt_Get       (void)                      { PRNG_INIT_STATE()  t_uint  result = PRNG_UInt       (state);            PRNG_Delete(&state);  return (result); }
t_sint  PRNG_SInt_Get       (void)                      { PRNG_INIT_STATE()  t_sint  result = PRNG_SInt       (state);            PRNG_Delete(&state);  return (result); }
t_fixed PRNG_Fixed_Get      (void)                      { PRNG_INIT_STATE()  t_fixed result = PRNG_Fixed      (state);            PRNG_Delete(&state);  return (result); }
t_float PRNG_Float_Get      (void)                      { PRNG_INIT_STATE()  t_float result = PRNG_Float      (state);            PRNG_Delete(&state);  return (result); }

t_uint  PRNG_UInt_Get_Range (t_uint  min, t_uint  max)  { PRNG_INIT_STATE()  t_uint  result = PRNG_UInt_Range (state, min, max);  PRNG_Delete(&state);  return (result); }
t_sint  PRNG_SInt_Get_Range (t_sint  min, t_sint  max)  { PRNG_INIT_STATE()  t_sint  result = PRNG_SInt_Range (state, min, max);  PRNG_Delete(&state);  return (result); }
t_fixed PRNG_Fixed_Get_Range(t_fixed min, t_fixed max)  { PRNG_INIT_STATE()  t_fixed result = PRNG_Fixed_Range(state, min, max);  PRNG_Delete(&state);  return (result); }
t_float PRNG_Float_Get_Range(t_float min, t_float max)  { PRNG_INIT_STATE()  t_float result = PRNG_Float_Range(state, min, max);  PRNG_Delete(&state);  return (result); }
