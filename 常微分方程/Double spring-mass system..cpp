#include <stdio.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>
struct rparams
{
double a;
double b;
};
int
func (double t, const double y[], double f[],
void *params)
{
(void)(t); /* avoid unused parameter warning */
double a= ((struct rparams *) params)->a;
double b= ((struct rparams *) params)->b;
f[0] = y[1];
f[1] = - a*y[0]+b*(y[2]-y[0]);
f[2] = y[3];
f[3] = b*(y[0]-y[2]);
return GSL_SUCCESS;
}
int
jac (double t, const double y[], double *dfdy,
double dfdt[], void *params)
{}

int
main (void)
{
FILE *rtf;
rtf=fopen("rtf.txt","w+");

const gsl_odeiv_step_type * T
= gsl_odeiv_step_gear2;
gsl_odeiv_step * s
= gsl_odeiv_step_alloc (T, 4);
gsl_odeiv_control * c
= gsl_odeiv_control_y_new (1e-1, 0.0);
gsl_odeiv_evolve * e
= gsl_odeiv_evolve_alloc (4);
struct rparams mu= {79.0, 1e7*2368.7};
gsl_odeiv_system sys = {func, jac, 4, &mu};
double t = 0.0, t1 = 30;
double h = 1e-3;
double y[4] = { 1.0, 0.0,  1.0,0.0 };
while (t < t1)
{
int status = gsl_odeiv_evolve_apply (e, c, s,
&sys,
&t, t1,
&h, y);
if (status != GSL_SUCCESS)
break;
printf("%g\n",t);

fprintf (rtf,"%.5e %.5e %.5e %.5e  %.5e \n", t, y[0], y[1],y[2],y[3]);
}
gsl_odeiv_evolve_free (e);
gsl_odeiv_control_free (c);
gsl_odeiv_step_free (s);
return 0;
}
