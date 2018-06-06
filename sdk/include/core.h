#ifndef OMS_CORE_H
#define OMS_CORE_H

#include <set>
#include <string>

#include <pthread.h>

#include "smart_pointer.h"

#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>

#include "JavaOption.h"
#include "Namespace.h"
#include "CurrentEnv.h"
#include "Uncopyable.h"

BEGIN_NAMESPACE_2(io, openmessaging)

    class Types {
    public:
        static const char *void_;
        static const char *boolean_;
        static const char *byte_;
        static const char *char_;
        static const char *short_;
        static const char *int_;
        static const char *long_;
        static const char *float_;
        static const char *double_;

        static const char *byteArray_;

        static const char *Object_;
        static const char *String_;
        static const char *KeyValue_;
        static const char *Set_;
        static const char *MessagingAccessPoint_;
        static const char *Producer_;
        static const char *PushConsumer_;
        static const char *PullConsumer_;
        static const char *Message_;
        static const char *ByteMessage_;
        static const char *SendResult_;
        static const char *LocalTransactionExecutor_;
        static const char *Context_;
        static const char *MessageListener_;
        static const char *ConsumerInterceptor_;
        static const char *ProducerInterceptor_;
        static const char *BatchMessageSender_;
        static const char *OMSException_;
        static const char *Future_;
        static const char *Class_;
    };

    class VM : Uncopyable {
    public:
        VM() {
            init_logging();
            init();
        }

        ~VM() {
            if (jvm) {
                jvm->DestroyJavaVM();
            }
        }

        JavaVM* get() {
            return jvm;
        }

    private:
        JavaVM *jvm;
        JNIEnv *env;

        void init_logging();

        void init();
    };

#ifdef CHECK_PTHREAD_RETURN_VALUE

#ifdef NDEBUG
__BEGIN_DECLS
extern void __assert_perror_fail (int errnum,
                                  const char *file,
                                  unsigned int line,
                                  const char *function)
    __THROW __attribute__ ((__noreturn__));
__END_DECLS
#endif

#define MCHECK(ret) ({ __typeof__ (ret) errnum = (ret);         \
                       if (__builtin_expect(errnum != 0, 0))    \
                         __assert_perror_fail (errnum, __FILE__, __LINE__, __func__);})

#else  // CHECK_PTHREAD_RETURN_VALUE

#define MCHECK(ret) ({ __typeof__ (ret) errnum = (ret);         \
                       assert(errnum == 0); (void) errnum;})

#endif // CHECK_PTHREAD_RETURN_VALUE

    class Mutex : Uncopyable {
    public:
        Mutex() {
            MCHECK(pthread_mutex_init(&mtx_, NULL));
        }

        ~Mutex() {
            LOG_DEBUG << "Try to destroy mutex: " << &mtx_;
            MCHECK(pthread_mutex_destroy(&mtx_));
            LOG_DEBUG << "Mutex " << &mtx_ << " destroyed [OK]";
        }

        pthread_mutex_t* get() {
            return &mtx_;
        }

        void lock() {
            LOG_DEBUG << "Try to lock: " << &mtx_;
            MCHECK(pthread_mutex_lock(&mtx_));
            LOG_DEBUG << "Lock: " << &mtx_ << "[OK]";
        }

        void unlock() {
            LOG_DEBUG << "Try to unlock: " << &mtx_;
            MCHECK(pthread_mutex_unlock(&mtx_));
            LOG_DEBUG << "Unlock: " << &mtx_ << " [OK]";
        }

    private:
        pthread_mutex_t mtx_;
    };

    class LockGuard : Uncopyable {
    public:
        LockGuard(Mutex& mutex) : mtx(mutex) {
            mtx.lock();
        }

        ~LockGuard() {
            mtx.unlock();
        }

    private:
        Mutex& mtx;
    };

#define LockGuard(x) error "Missing lock guard object name"

    class Condition : Uncopyable {
    public:
        explicit Condition(Mutex& mutex) : mtx(mutex) {
            pthread_cond_init(&cv, NULL);
        }

        ~Condition() {
            pthread_cond_destroy(&cv);
        }

        void wait() {
            LockGuard lk(mtx);
            pthread_cond_wait(&cv, mtx.get());
        }

        template<typename Predicate>
        void wait(Predicate predicate) {
            LockGuard lk(mtx);
            while (!predicate()) {
                pthread_cond_wait(&cv, mtx.get());
            }
        }

        void wait(unsigned long milliseconds) {
            struct timespec ts;
            add(ts, milliseconds);
            LockGuard lk(mtx);
            pthread_cond_timedwait(&cv, mtx.get(), &ts);
        }

        template<typename Predicate>
        void wait(unsigned long milliseconds, Predicate predicate) {
            struct timespec ts;
            add(ts, milliseconds);
            long abs_time = ts.tv_sec * one_second_in_nano + ts.tv_nsec;
            struct timeval tv;
            LockGuard lk(mtx);
            while (!predicate()) {
                gettimeofday(&tv, NULL);
                if (tv.tv_sec * one_second_in_nano + tv.tv_usec * one_micro_second_in_nano >= abs_time) {
                    break;
                }
                pthread_cond_timedwait(&cv, mtx.get(), &ts);
            }
        }

        void notify() {
            LockGuard lk(mtx);
            pthread_cond_signal(&cv);
        }

        void notifyAll() {
            LockGuard lk(mtx);
            pthread_cond_broadcast(&cv);
        }

    private:
        Mutex& mtx;
        pthread_cond_t cv;

        void add(struct timespec& ts, long timeout) {
            struct timeval tv;
            gettimeofday(&tv, NULL);
            ts.tv_sec = tv.tv_sec + timeout / one_second_in_milli;
            ts.tv_nsec = tv.tv_usec * one_micro_second_in_nano + (timeout % one_second_in_milli) * one_milli_second_in_nano;
            if (ts.tv_nsec >= one_second_in_nano) {
                ts.tv_sec = ts.tv_sec + (ts.tv_nsec / one_second_in_nano);
                ts.tv_nsec = ts.tv_nsec % one_second_in_nano;
            }
        }

        const static long one_second_in_nano;
        const static long one_milli_second_in_nano;
        const static long one_micro_second_in_nano;
        const static long one_second_in_milli;
    };

    std::string buildSignature(const std::string &return_type, int n, ...);

    std::set<std::string> toNativeSet(CurrentEnv &env, jobject s);

    std::string build_class_path_option();

END_NAMESPACE_2(io, openmessaging)

#endif //OMS_CORE_H
