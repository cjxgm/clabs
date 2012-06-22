
#ifndef ___KE_VECTOR_H___
#define ___KE_VECTOR_H___

void vec_add(float vd[3], float v1[3], float v2[3]);
void vec_addv(float vd[3], float v[3]);

void vec_sub(float vd[3], float v1[3], float v2[3]);
void vec_subv(float vd[3], float v[3]);

void vec_mul(float vd[3], float v[3], float s);
void vec_mulv(float vd[3], float s);

float vec_dot(float v1[3], float v2[3]);
void vec_cross(float vd[3], float v1[3], float v2[3]);

float vec_sqr(float v[3]);
float vec_mod(float v[3]);

void vec_norm(float vd[3], float v[3]);
void vec_normv(float vd[3]);

void vec_unit_normal(float vd[3], float v1[3], float v2[3]);
void vec_cpy(float vd[3], float v[3]);

#endif

