#include <stdio.h>
#include <math.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv.h>
#define ERR 0.0001  //判断接触点相对速度是否为零的小量
#define ZERO 0.00001  //判断圆盘是否与地面接触的小量
#define PI 3.1415926  //圆周率
#define mm 2  //圆盘质量
#define gg 10  //重力加速度
#define RR 0.2  //圆盘半径
#define ee 0.1  // 偏心距
#define Jc 0.045  //绕质心的转动惯量
#define mu 0.15  //动摩擦系数 0.075*n (取n<=10)
#define mus 0.2  //静摩擦系数
void vFsFN(double v,double theta,double dtheta,double vFF[]);//求摩擦力和支持力，其中vFF[0]为接触点相对速度v，vFF[1]为Fs，vFF[2]为FN
double TT(double theta,double dtheta,double dx0,double dy0);//求动能
double VV(double theta,double y0);//求势能
int func (double t, const double y[], double f[],void *params)
{
(void)(t); /* avoid unused parameter warning*/ 
	//f
	double vFF[3];
	vFF[0]=y[3]+RR*y[1];  //接触点速度，轮离地时，即为轮最低点水平速度
	if(y[4]<=ZERO){  //与地面接触时
		vFsFN(vFF[0],y[0],y[1],vFF);}
	else {  //离开地面时
		vFF[1]=0; vFF[2]=0;
	}
	
	f[0]=y[1];
	f[1]=(vFF[1]*(RR-ee*cos(y[0]))-vFF[2]*ee*sin(y[0]))/Jc;
	f[2]=y[3];
	f[3]=vFF[1]/mm-(vFF[1]*(ee*RR*cos(y[0])-ee*ee*cos(y[0])*cos(y[0]))-vFF[2]*ee*ee*sin(y[0])*cos(y[0]))/Jc+ee*y[1]*y[1]*sin(y[0]);
	f[4]=y[5];
	f[5]=vFF[2]/mm-(vFF[1]*(ee*RR*sin(y[0])-ee*ee*sin(y[0])*cos(y[0]))-vFF[2]*ee*ee*sin(y[0])*sin(y[0]))/Jc-gg-ee*y[1]*y[1]*cos(y[0]);
return GSL_SUCCESS;
}

int jac (double t, const double y[], double *dfdy,double dfdt[], void *params){}

int
main (void)
{
FILE *fp,*fp1,*fp2;
fp=fopen("fp.txt","w+");
fp1=fopen("fp1.txt","w+");
fp2=fopen("fp2.txt","w+");

/* Available stepper types.
 *
 * rk2    : embedded 2nd(3rd) Runge-Kutta
 * rk4    : 4th order (classical) Runge-Kutta
 * rkck   : embedded 4th(5th) Runge-Kutta, Cash-Karp
 * rk8pd  : embedded 8th(9th) Runge-Kutta, Prince-Dormand
 * rk2imp : implicit 2nd order Runge-Kutta at Gaussian points
 * rk4imp : implicit 4th order Runge-Kutta at Gaussian points
 * gear1  : M=1 implicit Gear method
 * gear2  : M=2 implicit Gear method
 */
const gsl_odeiv_step_type *T= gsl_odeiv_step_rk4;
gsl_odeiv_step * s= gsl_odeiv_step_alloc (T, 6);
gsl_odeiv_control * c= gsl_odeiv_control_y_new (1e-6, 1e-16);
gsl_odeiv_evolve * e= gsl_odeiv_evolve_alloc (6);
double vF[3]={};//params
gsl_odeiv_system sys = {func, jac, 6, &vF};
double t=0.0,t1=10.0;
double h=0.00001; 
double y[6] = { PI/2, 0.0, 0.0, 0.0,RR,0.0};

while (t < t1)
{
double T,V;	
T=TT(y[0],y[1],y[3],y[5]);  //求动能
V=VV(y[0],y[4]);  //求势能
int status = gsl_odeiv_evolve_apply (e, c, s,&sys,&t, t1,&h, y);
if (status != GSL_SUCCESS)
break;
printf ("%.2e\n",t );
fprintf (fp,"%.2e   %.5e %.5e %.5e %.5e %.5e %.5e   %.5e %.5e \n", t, y[0], y[1],y[2],y[3], y[4],y[5],T,V);



	double vFF[3];
	vFF[0]=y[3]+RR*y[1];  //接触点速度，轮离地时，即为轮最低点水平速度
	if(y[4]<=1e-4){  //与地面接触时
		vFsFN(vFF[0],y[0],y[1],vFF);
	}
	else{  //离开地面时
		vFF[1]=0; vFF[2]=0;
	}
fprintf(fp1,"%5.5f,%5.5f,%5.5f\n",vFF[0],vFF[1],vFF[2]);  //输出相对速度v,Fs,FN
}

gsl_odeiv_evolve_free (e);
gsl_odeiv_control_free (c);
gsl_odeiv_step_free (s);
return 0;
}
 
//**************************************************************************
 void vFsFN(double v,double theta,double dtheta,double vFF[])//求摩擦力
  {
	  double Fs,FN,A,B,ddtheta;

	  if(fabs(v)>ERR){ //有相对滑动时
		  A=(RR-ee*cos(theta))/(Jc+mm*ee*ee*sin(theta)*sin(theta));
		  B=(mm*gg*ee*sin(theta)+mm*ee*ee*dtheta*dtheta*sin(theta)*cos(theta))/(Jc+mm*ee*ee*sin(theta)*sin(theta));
		  if(v>ERR)
			  Fs=-(mu*mm*gg+mu*mm*ee*dtheta*dtheta*cos(theta)-mu*mm*ee*sin(theta)*B)/(1+mu*mm*ee*sin(theta)*A);
		  else
			  Fs=(mu*mm*gg+mu*mm*ee*dtheta*dtheta*cos(theta)-mu*mm*ee*sin(theta)*B)/(1-mu*mm*ee*sin(theta)*A);
		  FN=mm*gg+mm*ee*dtheta*dtheta*cos(theta)+mm*ee*sin(theta)*(A*Fs-B);
	  } 
	  else{  //无相对滑动时
		  ddtheta=(-mm*ee*RR*dtheta*dtheta*sin(theta)-mm*gg*ee*sin(theta))/(mm*ee*ee+Jc+mm*RR*RR-2*mm*ee*RR*cos(theta));
		  Fs=-mm*RR*ddtheta+mm*ee*ddtheta*cos(theta)-mm*ee*dtheta*dtheta*sin(theta);
		  FN=mm*gg+mm*ee*ddtheta*sin(theta)+mm*ee*dtheta*dtheta*cos(theta);
		  if(Fs>mus*FN)
			  Fs=mus*FN;
		  else if(Fs<-mus*FN)
			  Fs=-mus*FN;	  
	  }
	  if(FN<=0){  //离地情形
		  Fs=0; FN=0;
	  }
	  vFF[1]=Fs;  //摩擦力
	  vFF[2]=FN;  //支持力
	  return;
  }

  double TT(double theta,double dtheta,double dx0,double dy0)//求动能
  {
	  double M,N,T;
	  M=(dx0+ee*dtheta*cos(theta))*(dx0+ee*dtheta*cos(theta));
	  N=(dy0+ee*dtheta*sin(theta))*(dy0+ee*dtheta*sin(theta));
	  T=0.5*mm*(M+N)+0.5*Jc*dtheta*dtheta;
      return T;   
  }
  double VV(double theta,double y0)//求势能
  {
	  double V;
	  V=mm*gg*(y0+RR-ee*cos(theta));
	  return V;  
  }
