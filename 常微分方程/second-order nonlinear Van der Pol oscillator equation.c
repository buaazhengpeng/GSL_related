#include <stdio.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>

int
func (double t, const double y[], double f[],
void *params)
{
(void)(t); /* avoid unused parameter warning */
double mu = *(double *)params;
f[0] = y[1];
f[1] = -y[0] - mu*y[1]*(y[0]*y[0] - 1);
return GSL_SUCCESS;
}


int
jac (double t, const double y[], double *dfdy,
double dfdt[], void *params)
{
	/*
(void)(t); /* avoid unused parameter warning 
double mu = *(double *)params;
gsl_matrix_view dfdy_mat
= gsl_matrix_view_array (dfdy, 2, 2);
gsl_matrix * m = &dfdy_mat.matrix;
gsl_matrix_set (m, 0, 0, 0.0);
gsl_matrix_set (m, 0, 1, 1.0);
gsl_matrix_set (m, 1, 0, -2.0*mu*y[0]*y[1] - 1.0);
gsl_matrix_set (m, 1, 1, -mu*(y[0]*y[0] - 1.0));
dfdt[0] = 0.0;
dfdt[1] = 0.0;
return GSL_SUCCESS;
*/
}


int
main (void)
{
	
const gsl_odeiv_step_type * T
= gsl_odeiv_step_rk8pd;

gsl_odeiv_step * s
= gsl_odeiv_step_alloc (T, 2);

gsl_odeiv_control * c
= gsl_odeiv_control_y_new (1e-6, 0.0);

gsl_odeiv_evolve * e
= gsl_odeiv_evolve_alloc (2);

double mu = 10;
gsl_odeiv_system sys = {func, jac, 2, &mu};

double t = 0.0, t1 = 100.0;
double h = 1e-6;
double y[2] = { 1.0, 0.0 };
while (t < t1)
{
int status = gsl_odeiv_evolve_apply (e, c, s,
&sys,
&t, t1,
&h, y);
if (status != GSL_SUCCESS)
break;
printf ("%.5e %.5e %.5e\n", t, y[0], y[1]);
}
gsl_odeiv_evolve_free (e);
gsl_odeiv_control_free (c);
gsl_odeiv_step_free (s);
return 0;
}
//没有GSL_ODEIV2.H,  未定义driver 
/*************************************************************************************************************************
It is also possible to work with a non-adaptive integrator, using only the stepping function itself,
gsl_odeiv2_driver_apply_fixed_step() or gsl_odeiv2_evolve_apply_fixed_step(). The
following program uses the driver level function, with fourth-order Runge-Kutta stepping function with a fixed stepsize
of 0.001.
*************************************************************************************************************************
int
main (void)
{
double mu = 10;
gsl_odeiv2_system sys = { func, jac, 2, &mu };
gsl_odeiv2_driver *d =
gsl_odeiv2_driver_alloc_y_new (&sys, gsl_odeiv2_step_rk4,
1e-3, 1e-8, 1e-8);
double t = 0.0;
double y[2] = { 1.0, 0.0 };
int i, s;
for (i = 0; i < 100; i++)
{
s = gsl_odeiv2_driver_apply_fixed_step (d, &t, 1e-3, 1000, y);
if (s != GSL_SUCCESS)
{
printf ("error: driver returned %d\n", s);
break;
}
printf ("%.5e %.5e %.5e\n", t, y[0], y[1]);
}
gsl_odeiv2_driver_free (d);
return s;
}
*/

//********************************************************************************************************************************
/*
int
main (void)
{
	double mu = 10;
gsl_odeiv_system sys = {func, jac, 2, &mu};
gsl_odeiv_driver *d =gsl_odeiv_driver_alloc_y_new (&sys, gsl_odeiv_step_rk8pd,
1e-6, 1e-6, 0.0);
int i;
double t = 0.0, t1 = 100.0;
double y[2] = { 1.0, 0.0 };
for (i = 1; i <= 100; i++)
{
double ti = i * t1 / 100.0;
int status = gsl_odeiv_driver_apply (d, &t, ti, y);
if (status != GSL_SUCCESS)
{
printf ("error, return value=%d\n", status);
break;
}
printf ("%.5e %.5e %.5e\n", t, y[0], y[1]);
}
gsl_odeiv_driver_free (d);
return 0;
}*/
