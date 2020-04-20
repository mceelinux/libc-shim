#pragma once

#include <cstdint>
#include <pthread.h>
#include <libc_shim.h>
#include "meta.h"
#include "argrewrite.h"
#include "common.h"

namespace shim {

    namespace bionic {

        struct pthread_mutex_t {
            ::pthread_mutex_t *wrapped;
#if defined(__LP64__)
            int64_t priv[4];
#endif
        };

        struct pthread_cond_t {
            ::pthread_cond_t *wrapped;
#if defined(__LP64__)
            int64_t priv[5];
#endif
        };

        template <>
        auto to_host<pthread_mutex_t>(pthread_mutex_t const *m) { return m->wrapped; }

        enum class mutex_type : uint32_t {
            NORMAL = 0,
            RECURSIVE = 1,
            ERRORCHECK = 2,
            END = 2
        };

        static int to_host_mutex_type(mutex_type type);

        struct pthread_mutexattr_t {
            mutex_type type : 4;
        };

        struct pthread_condattr_t {
            bool shared : 1;
            clock_type clock : 2;
        };

    }

    using pthread_mutex_t_resolver = detail::wrapper_type_resolver<::pthread_mutex_t, bionic::pthread_mutex_t>;
    using pthread_mutex_t = pthread_mutex_t_resolver::type;
    using pthread_mutexattr_t = bionic::pthread_mutexattr_t;

    using pthread_cond_t_resolver = detail::wrapper_type_resolver<::pthread_cond_t, bionic::pthread_cond_t>;
    using pthread_cond_t = pthread_cond_t_resolver::type;
    using pthread_condattr_t = bionic::pthread_condattr_t;

    struct host_mutexattr {

        ::pthread_mutexattr_t attr;

        host_mutexattr(bionic::pthread_mutexattr_t const *bionic_attr);

        ~host_mutexattr();

    };

    struct host_condattr {

        ::pthread_condattr_t attr;

        host_condattr(bionic::pthread_condattr_t const *bionic_attr);

        ~host_condattr();

    };

    int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
    int pthread_mutex_destroy(pthread_mutex_t *mutex);

    int pthread_mutexattr_init(pthread_mutexattr_t *attr);
    int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);
    int pthread_mutexattr_gettype(const pthread_mutexattr_t *attr, int *type);
    int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type);

    int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);
    int pthread_cond_destroy(pthread_cond_t* cond);

    int pthread_condattr_init(pthread_condattr_t *attr);
    int pthread_condattr_destroy(pthread_condattr_t *attr);
    int pthread_condattr_setclock(pthread_condattr_t *attr, int clock);
    int pthread_condattr_getclock(const pthread_condattr_t *attr, int *clock);


    void add_pthread_shimmed_symbols(std::vector<shimmed_symbol> &list);

    namespace detail {

        template <>
        struct arg_rewrite<pthread_mutex_t const *> : wrapper_resolved_const_ptr_rewriter<pthread_mutex_t_resolver> {};

        template <>
        struct arg_rewrite<pthread_cond_t const *> : wrapper_resolved_const_ptr_rewriter<pthread_cond_t_resolver> {};

    }

}