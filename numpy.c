#include "erl_nif.h"
#include "stdio.h"
#define INX(row, col, LDA) ((row)*(LDA)+(col))

struct vector {
    double *value;
    unsigned int size;
};

struct matrix {
    double *value;
    unsigned int rows;
    unsigned int cols;
    unsigned int step;
    unsigned int offset;
};

static void delete_vector(ErlNifEnv* env, void* arg) {
    struct vector *v = (struct vector *)arg;
    enif_free(v->value);
    enif_free(v);
}

static void delete_matrix(ErlNifEnv* env, void* arg) {
    struct matrix *m = (struct matrix *)arg;
    enif_free(m->value);
    enif_free(m);
}

static ErlNifResourceType* vector_resource = NULL;
static ErlNifResourceType* matrix_resource = NULL;

static int on_load(ErlNifEnv* env, void** priv_data, ERL_NIF_TERM load_info)
{
    ErlNifResourceFlags flag = ERL_NIF_RT_CREATE;
    vector_resource = enif_open_resource_type(env, NULL, "vector_resource",
                                                 &delete_vector, flag, NULL);
    matrix_resource = enif_open_resource_type(env, NULL, "matrix_resource",
                                                 &delete_matrix, flag, NULL);

    return 0;
}

void print_vector(struct vector *v) {
    for (int i = 0; i < v->size; ++i) {
        printf("%.2lf |", v->value[i]);
    }
    fflush(stdout);
}

/*
void print_matrix(struct matrix m) {
    for (int i = 0; i < m.rows; ++i) {
        for (int j = 0; j < m.cols; ++j) {
            printf("%.2lf |", m.value[i ]);
        }
        printf("\n");
    }
    fflush(stdout);
}
*/

ERL_NIF_TERM make_vector(ErlNifEnv* env, const ERL_NIF_TERM input_list) {
    struct vector *v = enif_alloc_resource(vector_resource, sizeof(struct vector));
    enif_get_list_length(env, input_list, &v->size);
    v->value = enif_alloc(v->size * sizeof(double));
    ERL_NIF_TERM item;
    ERL_NIF_TERM list = input_list;
    int i = 0;
    while(enif_get_list_cell(env, list, &item, &list)) {
        double inner_item;
        if (!enif_get_double(env, item, &inner_item)) {
            printf("FUCKING FAIL\n");
        }
        v->value[i] = inner_item;
        ++i;
    }
    ERL_NIF_TERM t = enif_make_resource(env, v);
    return t;
}
/*
struct matrix make_matrix(ErlNifEnv* env, const ERL_NIF_TERM input_matrix) {
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
*/

ERL_NIF_TERM make_list(ErlNifEnv* env, const ERL_NIF_TERM term) {
    struct vector *v;
    enif_get_resource(env, term, vector_resource, (void **)&v);
    ERL_NIF_TERM *array;
    array = enif_alloc(v->size * sizeof(ERL_NIF_TERM));
    for (unsigned i = 0; i < v->size; ++i) {
        array[i] = enif_make_double(env, v->value[i]);
    }
    return enif_make_list_from_array(env, array, v->size);
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
    struct vector *v_0;
    struct vector *v_1;
    ERL_NIF_TERM v_term_0 = make_vector(env, argv[0]);
    ERL_NIF_TERM v_term_1 = make_vector(env, argv[1]);
    enif_get_resource(env, v_term_0, vector_resource, (void **)&v_0);
    enif_get_resource(env, v_term_1, vector_resource, (void **)&v_1);

    struct vector *vector_result;
    vector_result = enif_alloc_resource(vector_resource, sizeof(struct vector));
    vector_result->size = size;
    vector_result->value = enif_alloc(size * sizeof(double));

    for (int i = 0; i < size; ++i) {
        vector_result->value[i] = v_0->value[i] + v_1->value[i];
    }
    return make_list(env, enif_make_resource(env, vector_result));
}

ERL_NIF_TERM scale(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    unsigned size;
    if (!enif_get_list_length(env, argv[1], &size)) {
        return enif_make_badarg(env);
    }
    struct vector *v;
    ERL_NIF_TERM v_term = make_vector(env, argv[1]);
    enif_get_resource(env, v_term, vector_resource, (void **)&v);

    double scaler;
    if (!enif_get_double(env, argv[0], &scaler)) {
        return enif_make_badarg(env);
    }

    struct vector *vector_result;
    vector_result = enif_alloc_resource(vector_resource, sizeof(struct vector));
    vector_result->size = size;
    vector_result->value = enif_alloc(size * sizeof(double));

    for (int i = 0; i < size; ++i) {
        vector_result->value[i] = scaler * v->value[i];
    }
    return make_list(env, enif_make_resource(env, vector_result));
}

static ErlNifFunc nif_funcs[] = {
    {"sum", 2, sum},
    {"scale", 2, scale},
};

ERL_NIF_INIT(Elixir.Numpy, nif_funcs, &on_load, NULL, NULL, NULL);
