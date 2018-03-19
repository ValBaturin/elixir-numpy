#include "erl_nif.h"
#include "stdio.h"

#define MATRIX 0
#define ROW 1
#define COL 2
#define DIAG 3

unsigned int inx(unsigned int row, unsigned int col, unsigned int col_size) {
    return col_size * row + col;
}

unsigned int min(unsigned int first, unsigned int second) {
    if (first < second) {
        return first;
    } else {
        return second;
    }
}

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
    int type;
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

static int on_load(ErlNifEnv* env, void** priv_data, ERL_NIF_TERM load_info) {
    ErlNifResourceFlags flag = ERL_NIF_RT_CREATE;
    vector_resource = enif_open_resource_type(env, NULL, "vector_resource",
                                                 &delete_vector, flag, NULL);
    matrix_resource = enif_open_resource_type(env, NULL, "matrix_resource",
                                                 &delete_matrix, flag, NULL);

    return 0;
}

ERL_NIF_TERM make_vector(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    struct vector *v = enif_alloc_resource(vector_resource, sizeof(struct vector));
    enif_get_list_length(env, argv[0], &v->size);
    v->value = enif_alloc(v->size * sizeof(double));
    ERL_NIF_TERM item;
    ERL_NIF_TERM list = argv[0];
    int i = 0;
    while(enif_get_list_cell(env, list, &item, &list)) {
        double inner_item;
        if (!enif_get_double(env, item, &inner_item)) {
            return enif_make_badarg(env);
        }
        v->value[i] = inner_item;
        ++i;
    }
    ERL_NIF_TERM t = enif_make_resource(env, v);
    return t;
}

ERL_NIF_TERM make_matrix(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    struct matrix *m = enif_alloc_resource(matrix_resource, sizeof(struct matrix));
    m->step = 1;
    m->offset = 0;
    m->type = MATRIX;
    enif_get_list_length(env, argv[0], &m->rows);
    ERL_NIF_TERM list = argv[0];
    ERL_NIF_TERM col, item;
    enif_get_list_cell(env, argv[0], &col, &item);
    enif_get_list_length(env, col, &m->cols);
    m->value = enif_alloc(m->rows * m->cols * sizeof(double));
    for (int i = 0; i < m->rows; ++i) {
        enif_get_list_cell(env, list, &col, &list);
        for (int j = 0; j < m->cols; ++j) {
            double inner_item;
            enif_get_list_cell(env, col, &item, &col);
            enif_get_double(env, item, &inner_item);
            m->value[inx(i, j, m->cols)] = inner_item;
        }
    }
    fflush(stdout);
    ERL_NIF_TERM t = enif_make_resource(env, m);
    return t;
}


ERL_NIF_TERM make_list_from_vector(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    struct vector *v;
    enif_get_resource(env, argv[0], vector_resource, (void **)&v);
    ERL_NIF_TERM *array;
    array = enif_alloc(v->size * sizeof(ERL_NIF_TERM));
    for (unsigned i = 0; i < v->size; ++i) {
        array[i] = enif_make_double(env, v->value[i]);
    }
    ERL_NIF_TERM result = enif_make_list_from_array(env, array, v->size);
	enif_free(array);
    return result;
}

ERL_NIF_TERM make_list_from_matrix_row_col_diag(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    struct matrix *m;
    enif_get_resource(env, argv[0], matrix_resource, (void **)&m);
    ERL_NIF_TERM *array;
    ERL_NIF_TERM result;
    if (m->type == ROW) {
        array = enif_alloc(m->cols * sizeof(ERL_NIF_TERM));
        for (int i = 0; i < m->cols; ++i) {
            int index = m->offset + i;
            array[i] = enif_make_double(env, m->value[index]);
        }
    	result = enif_make_list_from_array(env, array, m->cols);
    } else if (m->type == COL) {
        array = enif_alloc(m->rows * sizeof(ERL_NIF_TERM));
        for (int i = 0; i < m->rows; ++i) {
            int index = m->offset + i * m->step;
            array[i] = enif_make_double(env, m->value[index]);
        }
    	result = enif_make_list_from_array(env, array, m->rows);
    } else if (m->type == DIAG) {
        unsigned int steps = min(m->rows, m->cols);
        array = enif_alloc(steps * sizeof(ERL_NIF_TERM));
        for (int i = 0; i < steps; ++i) {
            int index = m->offset + i * m->step;
            array[i] = enif_make_double(env, m->value[index]);
        }
    	result = enif_make_list_from_array(env, array, steps);
    } else {
        return enif_make_badarg(env);
	}
	enif_free(array);
    return result;
}


ERL_NIF_TERM make_list(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    struct vector *v;
    struct matrix *m;
    if (enif_get_resource(env, argv[0], vector_resource, (void **)&v)) {
        return make_list_from_vector(env, argc, argv);
    } else if (enif_get_resource(env, argv[0], matrix_resource, (void **)&m)) {
        return make_list_from_matrix_row_col_diag(env, argc, argv);
    } else {
        return enif_make_badarg(env);
    }
}

ERL_NIF_TERM make_deep_list(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    struct matrix *m;
    enif_get_resource(env, argv[0], matrix_resource, (void **)&m);
    ERL_NIF_TERM *col;
    ERL_NIF_TERM *row;
    col = enif_alloc(m->cols * sizeof(ERL_NIF_TERM));
    row = enif_alloc(m->rows * sizeof(ERL_NIF_TERM));
    for (unsigned i = 0; i < m->rows; ++i) {
        for (unsigned j = 0; j < m->cols; ++j) {
            unsigned index = inx(i, j, m->cols);
            ERL_NIF_TERM value = enif_make_double(env, m->value[index]);
            col[j] = value;
        }
        ERL_NIF_TERM inner = enif_make_list_from_array(env, col, m->cols);
        row[i] = inner;
    }
    return enif_make_list_from_array(env, row, m->rows);
}

ERL_NIF_TERM vsum(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
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
    ERL_NIF_TERM v_term_0 = make_vector(env, 1, &argv[0]);
    ERL_NIF_TERM v_term_1 = make_vector(env, 1, &argv[1]);
    enif_get_resource(env, v_term_0, vector_resource, (void **)&v_0);
    enif_get_resource(env, v_term_1, vector_resource, (void **)&v_1);

    struct vector *vector_result;
    vector_result = enif_alloc_resource(vector_resource, sizeof(struct vector));
    vector_result->size = size;
    vector_result->value = enif_alloc(size * sizeof(double));

    for (int i = 0; i < size; ++i) {
        vector_result->value[i] = v_0->value[i] + v_1->value[i];
    }
    ERL_NIF_TERM result_term = enif_make_resource(env, vector_result);
    return make_list(env, 1, &result_term);
}

ERL_NIF_TERM msum(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    struct matrix *m_0;
    struct matrix *m_1;
    ERL_NIF_TERM v_term_0 = make_matrix(env, 1, &argv[0]);
    ERL_NIF_TERM v_term_1 = make_matrix(env, 1, &argv[1]);
    enif_get_resource(env, v_term_0, matrix_resource, (void **)&m_0);
    enif_get_resource(env, v_term_1, matrix_resource, (void **)&m_1);

    struct matrix *matrix_result;
    matrix_result = enif_alloc_resource(matrix_resource, sizeof(struct matrix));
    matrix_result->rows = m_0->rows;
    matrix_result->cols = m_0->cols;
    matrix_result->offset = 0;
    matrix_result->step = 1;
    matrix_result->type = MATRIX;
    matrix_result->value = enif_alloc(matrix_result->cols * matrix_result->rows * sizeof(double));

    for (int i = 0; i < matrix_result->rows; ++i) {
        for (int j = 0; j < matrix_result->cols; ++j) {
            unsigned int index = inx(i, j, matrix_result->cols);
            matrix_result->value[index] = m_0->value[index] + m_1->value[index];
        }
    }

    ERL_NIF_TERM result_term = enif_make_resource(env, matrix_result);
    return make_deep_list(env, 1, &result_term);
}


ERL_NIF_TERM vscale(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    unsigned size;
    if (!enif_get_list_length(env, argv[1], &size)) {
        return enif_make_badarg(env);
    }
    struct vector *v;
    ERL_NIF_TERM v_term = make_vector(env, 1, &argv[1]);
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
    ERL_NIF_TERM result_term = enif_make_resource(env, vector_result);
    return make_list(env, 1, &result_term);
}

ERL_NIF_TERM mscale(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    struct matrix *m;
    ERL_NIF_TERM v_term = make_matrix(env, 1, &argv[1]);
    enif_get_resource(env, v_term, matrix_resource, (void **)&m);

    double scaler;
    if (!enif_get_double(env, argv[0], &scaler)) {
        return enif_make_badarg(env);
    }

    struct matrix *matrix_result;
    matrix_result = enif_alloc_resource(matrix_resource, sizeof(struct matrix));
    matrix_result->rows = m->rows;
    matrix_result->cols = m->cols;
    matrix_result->offset = 0;
    matrix_result->step = 1;
    matrix_result->type = MATRIX;
    matrix_result->value = enif_alloc(matrix_result->cols * matrix_result->rows * sizeof(double));

    for (int i = 0; i < matrix_result->rows; ++i) {
        for (int j = 0; j < matrix_result->cols; ++j) {
            unsigned int index = inx(i, j, matrix_result->cols);
            matrix_result->value[index] = scaler * m->value[index];
        }
    }

    ERL_NIF_TERM result_term = enif_make_resource(env, matrix_result);
    return make_deep_list(env, 1, &result_term);
}


ERL_NIF_TERM get_col(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    struct matrix *m;
    enif_get_resource(env, argv[1], matrix_resource, (void **)&m);

    int col;
    if (!enif_get_int(env, argv[0], &col)) {
        return enif_make_badarg(env);
    }
    m->type = COL;
    m->offset = col;
    m->step = m->cols;
    return argv[1];
}


ERL_NIF_TERM get_row(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    struct matrix *m;
    ERL_NIF_TERM term = make_matrix(env, 1, &argv[1]);
    enif_get_resource(env, term, matrix_resource, (void **)&m);

    int row;
    if (!enif_get_int(env, argv[0], &row)) {
        return enif_make_badarg(env);
    }
    m->type = ROW;
    m->offset = row;
    m->step = m->cols;
    return term;
}


ERL_NIF_TERM get_diag(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    struct matrix *m;
    ERL_NIF_TERM term = make_matrix(env, 1, &argv[1]);
    enif_get_resource(env, term, matrix_resource, (void **)&m);

    int diag;
    if (!enif_get_int(env, argv[0], &diag)) {
        return enif_make_badarg(env);
    }
    m->type = DIAG;
    if (diag == 1) {
        m->step = m->cols + 1;
    } else if (diag == -1) {
        m->offset = m->cols - 1;
        m->step = m->cols - 1;
    } else {
        return enif_make_badarg(env);
    }
    return term;
}


static ErlNifFunc nif_funcs[] = {
    {"vsum", 2, vsum},
    {"vscale", 2, vscale},
    {"msum", 2, msum},
    {"mscale", 2, mscale},
    {"make_list", 1, make_list},
    {"make_deep_list", 1, make_deep_list},
    {"make_vector", 1, make_vector},
    {"make_matrix", 1, make_matrix},
    {"get_col", 2, get_col},
    {"get_row", 2, get_row},
    {"get_diag", 2, get_diag}
};

ERL_NIF_INIT(Elixir.Numpy, nif_funcs, &on_load, NULL, NULL, NULL);
