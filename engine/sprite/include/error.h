#ifndef SIMPLE_ERROR_H
#define SIMPLE_ERROR_H

#ifndef ERR_STACK_MAX
#define ERR_STACK_MAX 32
#endif

#include <stdlib.h>

/* 单个错误帧 */
typedef struct {
    const char *func;
    const char *file;
    int line;
} ErrFrame;

typedef struct {
    int code;
    char *msg;
    int depth;
    ErrFrame stack[ERR_STACK_MAX];
} Error;

static inline void
err_push(Error *e, const char *func, const char *file, int line) {   
    if (e->depth < ERR_STACK_MAX) {
        e->stack[e->depth++] = (ErrFrame){
            .func = func,
            .file = file,
            .line = line,
        };
    }
}

#define return_err_wrap(e) \
    do { \
        err_push((e),  __func__, __FILE__, __LINE__); \
        return (e); \
    } while (0);

#define panic(msg) do { \
    fprintf(stderr, "panic: %s\n", msg); \
    abort(); \
} while(0)

static inline Error*
err_msg(char *msg) {
    Error *err = (Error*) malloc(sizeof(Error));
    if (!err) {
        panic("out of memory!");
    }
    err->depth = 0;
    err->code = -1;
    err->msg = msg;
    return err;
}

#define return_err_msg(msg) \
    do { \
        Error *err = err_msg((msg)); \
        err_push(err, __func__, __FILE__, __LINE__); \
        return err; \
    } while (0)

static inline Error*
err_code(int code) {
    Error *err = malloc(sizeof(Error));
    if (!err) {
        panic("out of memory!");
    }
    err->depth = 0;
    err->code = code;
    return err;
}

#define return_err_code(code) \
    do { \
        Error *err = err_code((code)); \
        err_push(err, __func__, __FILE__, __LINE__); \
        return err; \
    } while (0)


static inline void
err_print_h(const Error *e, FILE *out) {
    if (e->code == 0) {
        fprintf(out, "OK\n");
        return;
    }

    fprintf(out, "error(code=%d):\n", e->code);
    fprintf(out, "error(msg=%s):\n", e->msg);
    for (int i = e->depth - 1; i >= 0; i--) {
        const ErrFrame *f = &e->stack[i];
        fprintf(out,
            "  at %s (%s:%d)\n",
            f->func,
            f->file,
            f->line);
    }
}

#define err_print(e, out) \
    do { \
        err_push(e, __func__, __FILE__, __LINE__); \
        err_print_h(e, out); \
        free(e); \
    } while (0)

#endif
