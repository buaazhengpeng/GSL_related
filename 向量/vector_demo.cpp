#include <stdio.h>
#include <gsl/gsl_vector.h>
int
main (void)
{
int i;
double p;
gsl_vector * v = gsl_vector_alloc (3);
for (i = 0; i < 3; i++)
{
gsl_vector_set (v, i, 1.23 + i);
}
for (i = 0; i < 100; i++) /* OUT OF RANGE ERROR */
{
p=gsl_vector_get (v, i);
printf ("v_%d = %g\n", i, p);
}

gsl_vector_free (v);
return 0;
}
/*
//The next program shows how to write a vector to a file.
#include <stdio.h>
#include <gsl/gsl_vector.h>
int
main (void)
{
int i;
gsl_vector * v = gsl_vector_alloc (100);
for (i = 0; i < 100; i++)
{
gsl_vector_set (v, i, 1.23 + i);
}
{
FILE * f = fopen ("test.dat", "w");
gsl_vector_fprintf (f, v, "%.5g");
fclose (f);
}
gsl_vector_free (v);
return 0;
}

//After running this program the file test.dat should contain the elements of v, written using the format specifier
//%.5g. The vector could then be read back in using the function gsl_vector_fscanf (f, v) as follows:
#include <stdio.h>
#include <gsl/gsl_vector.h>
int
main (void)
{
int i;
gsl_vector * v = gsl_vector_alloc (10);
{
FILE * f = fopen ("test.dat", "r");
gsl_vector_fscanf (f, v);
fclose (f);
}
*/
