#include "erl_nif.h"
#include "stdio.h"

struct vector {
    double *value;
    unsigned int size;
};

struct matrix {
    double *value
    unsigned int rows;
    unsigned int cols;
}

void print_vector(struct vector v) {
    for (int i = 0; i < v.size; ++i) {
        printf("%.2lf |", v.value[i]);
    }
    fflush(stdout);
}


void print_matrix(struct matrix m) {
    for (int i = 0; i < m.rows; ++i) {
        for (int j = 0; j < m.cols; ++j) {
            printf("%.2lf |", m.value[i][j]);
        }
        printf("\n");
    }
    fflush(stdout);
}


struct vector make_vec(ErlNifEnv* env, const ERL_NIF_TERM input_list, int size) {
    struct vector v;
    v.size = size;
    v.value = enif_alloc(size * sizeof(double));
    ERL_NIF_TERM item;
    ERL_NIF_TERM list = input_list;
    int i = 0;
    while(enif_get_list_cell(env, list, &item, &list)) {
        double inner_item;
        if (!enif_get_double(env, item, &inner_item)) {
            printf("FUCKING FAIL\n");
        }
        v.value[i] = inner_item;
        ++i;
    }
    return v;
}

ERL_NIF_TERM make_list(ErlNifEnv* env, const struct vector v) {
    ERL_NIF_TERM *array;
    array = enif_alloc(v.size * sizeof(ERL_NIF_TERM));
    for (unsigned i = 0; i < v.size; ++i) {
        array[i] = enif_make_double(env, v.value[i]);
    }
    return enif_make_list_from_array(env, array, v.size);
}

ERL_NIF_TERM sum(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    unsigned size;
    if (!enif_get_list_length(env, argv[0], &size)) {
        return enif_make_badarg(env);
    }
    unsigned size_1;
    if (!enif_get_list_length(env, argv[1], &size_1)) {
        return enif_make_badarg(env);
    }
    if (size != size_1) {
        return enif_make_badarg(env);
    }
    struct vector v_0 = make_vec(env, argv[0], size);
    struct vector v_1 = make_vec(env, argv[1], size);
    struct vector vector_result;
    vector_result.size = size;
    vector_result.value = enif_alloc(size * sizeof(double));
    for (int i = 0; i < size; ++i) {
        vector_result.value[i] = v_0.value[i] + v_1.value[i];
    }
    return make_list(env, vector_result);
}

ERL_NIF_TERM scale(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    unsigned size;
    if (!enif_get_list_length(env, argv[1], &size)) {
        return enif_make_badarg(env);
    }
    struct vector v = make_vec(env, argv[1], size);
    double scaler;
    if (!enif_get_double(env, argv[0], &scaler)) {
        return enif_make_badarg(env);
    }
    struct vector vector_result;
    vector_result.size = size;
    vector_result.value = enif_alloc(size * sizeof(double));
    for (int i = 0; i < size; ++i) {
        vector_result.value[i] = scaler * v.value[i];
    }
    return make_list(env, vector_result);
}

static ErlNifFunc nif_funcs[] = {
    {"sum", 2, sum},
    {"scale", 2, scale}
};

ERL_NIF_INIT(Elixir.Numpy, nif_funcs, NULL, NULL, NULL, NULL);
