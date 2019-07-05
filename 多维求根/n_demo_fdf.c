struct rparams
{
double a;
double b;
};
int
rosenbrock_f (const gsl_vector * x, void *params,
gsl_vector * f)
{
double a = ((struct rparams *) params)->a;
double b = ((struct rparams *) params)->b;
const double x0 = gsl_vector_get (x, 0);
const double x1 = gsl_vector_get (x, 1);
const double y0 = a * (1 - x0);
const double y1 = b * (x1 - x0 * x0);
gsl_vector_set (f, 0, y0);
gsl_vector_set (f, 1, y1);
return GSL_SUCCESS;
}
int
rosenbrock_df (const gsl_vector * x, void *params,
gsl_matrix * J)
{
const double a = ((struct rparams *) params)->a;
const double b = ((struct rparams *) params)->b;
const double x0 = gsl_vector_get (x, 0);
const double df00 = -a;
const double df01 = 0;
const double df10 = -2 * b * x0;
const double df11 = b;
gsl_matrix_set (J, 0, 0, df00);
gsl_matrix_set (J, 0, 1, df01);
gsl_matrix_set (J, 1, 0, df10);
gsl_matrix_set (J, 1, 1, df11);
return GSL_SUCCESS;
}
int
rosenbrock_fdf (const gsl_vector * x, void *params,
gsl_vector * f, gsl_matrix * J)
{
rosenbrock_f (x, params, f);
rosenbrock_df (x, params, J);
return GSL_SUCCESS;
}
