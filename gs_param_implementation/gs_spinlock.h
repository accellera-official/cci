// LICENSETEXT
//
//   Copyright (C) 2016 : GreenSocs Ltd
// 	 https://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Guillaume Delbergue <guillaume.delbergue@greensocs.com>,
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
//
// ENDLICENSETEXT

#ifndef GS_SPINLOCK_H_INCLUDED_
#define GS_SPINLOCK_H_INCLUDED_

__OPEN_NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__

#ifdef GS_SPINLOCK_GCC_ATOMICS
typedef volatile unsigned int gs_spinlock_t;

static inline int gs_spinlock_init(gs_spinlock_t *spinlock)
{
    *spinlock = 0;
    return 0;
}

static inline int gs_spinlock_lock(gs_spinlock_t *spinlock)
{
    while (__sync_lock_test_and_set(spinlock, 1)) {
        while(*spinlock);
    }
    return 0;
}

static inline int gs_spinlock_trylock(gs_spinlock_t *spinlock)
{
    return __sync_lock_test_and_set(spinlock, 1);
}

static inline int gs_spinlock_unlock(gs_spinlock_t *spinlock)
{
    __sync_lock_release(spinlock);
    return 0;
}

static inline int gs_spinlock_destroy(gs_spinlock_t *spinlock)
{
    return 0;
}
#endif
#ifdef GS_SPINLOCK_X86
typedef volatile unsigned int gs_spinlock_t;

static inline int gs_spinlock_init(gs_spinlock_t *spinlock)
{
    *spinlock = 0;
    return 0;
}

static inline int x86chgl(gs_spinlock_t *p, unsigned int v)
{
    __asm __volatile (
        "  xchg   %0, %1 ;"
        : "+r" (v), "=m" (*p)
        : "m" (*p)
    );

    return (v);
}

static inline int gs_spinlock_lock(gs_spinlock_t *spinlock)
{
    while (x86chgl(spinlock, 1)) {
       while(*spinlock);
    }
    return 0;
}

static inline int gs_spinlock_trylock(gs_spinlock_t *spinlock)
{
    return x86chgl(spinlock, 1);
}

static inline int gs_spinlock_unlock(gs_spinlock_t *spinlock)
{
    x86chgl(spinlock, 0);
    return 0;
}

static inline int gs_spinlock_destroy(gs_spinlock_t *spinlock)
{
    return 0;
}
#endif
#ifdef GS_SPINLOCK_PTHREAD_SPINLOCK
typedef pthread_mutex_t gs_spinlock_t;

static inline int gs_spinlock_init(gs_spinlock_t *spinlock)
{
    pthread_mutex_init(spinlock, NULL);
    return 0;
}

static inline int gs_spinlock_lock(gs_spinlock_t *spinlock)
{
    return pthread_mutex_lock(spinlock);
}

static inline int gs_spinlock_trylock(gs_spinlock_t *spinlock)
{
    return pthread_mutex_trylock(spinlock);
}

static inline int gs_spinlock_unlock(gs_spinlock_t *spinlock)
{
    return pthread_mutex_unlock(spinlock);
}

static inline int gs_spinlock_destroy(gs_spinlock_t *spinlock)
{
    return pthread_mutex_destroy(spinlock);
}
#endif

__CLOSE_NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__

#endif
