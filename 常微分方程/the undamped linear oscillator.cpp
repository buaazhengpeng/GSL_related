#include <stdio.h>
#include <math.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>
int
func (double t, const double y[], double f[],
void *params)
{
(void)(t); /* avoid unused parameter warning */
double ww = *(double *)params;
f[0] = y[1];
//f[1] = - ww*y[0];
f[1] = - ww*sin(y[0]);
return GSL_SUCCESS;
}
int
jac (double t, const double y[], double *dfdy,
double dfdt[], void *params)
{
(void)(t); // avoid unused parameter warning 
double ww= *(double *)params;
gsl_matrix_view dfdy_mat
= gsl_matrix_view_array (dfdy, 2, 2);
gsl_matrix * m = &dfdy_mat.matrix;
gsl_matrix_set (m, 0, 0, 0.0);
gsl_matrix_set (m, 0, 1, 1.0);
gsl_matrix_set (m, 1, 0, -ww);
gsl_matrix_set (m, 1, 1, 0);
dfdt[0] = 0.0;
dfdt[1] = 0.0;
return GSL_SUCCESS;
}

int
main (void)
{
FILE *rtf;
rtf=fopen("rk4.txt","w+");
const gsl_odeiv_step_type * T
= gsl_odeiv_step_rkf45;
gsl_odeiv_step * s
= gsl_odeiv_step_alloc (T, 2);
gsl_odeiv_control * c
= gsl_odeiv_control_y_new (1e-6, 0.0);
gsl_odeiv_evolve * e
= gsl_odeiv_evolve_alloc (2);
double mu = 980;
gsl_odeiv_system sys = {func, jac, 2, &mu};
double t = 0.0, t1 = 20.0;
double h = 1e-3;
double y[2] = { acos(-1), 0.0 };
while (t < t1)
{
int status = gsl_odeiv_evolve_apply (e, c, s,
&sys,
&t, t1,
&h, y);
if (status != GSL_SUCCESS)
break;
printf("%g\n",t);
fprintf (rtf,"%.5e %.5e %.5e\n", t, y[0], y[1]);
}
gsl_odeiv_evolve_free (e);
gsl_odeiv_control_free (c);
gsl_odeiv_step_free (s);
return 0;
}
