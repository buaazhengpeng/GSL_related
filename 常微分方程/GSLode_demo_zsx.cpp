#include <stdio.h>
#include <math.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv.h>
#define ERR 0.0001  //�жϽӴ�������ٶ��Ƿ�Ϊ���С��
#define ZERO 0.00001  //�ж�Բ���Ƿ������Ӵ���С��
#define PI 3.1415926  //Բ����
#define mm 2  //Բ������
#define gg 10  //�������ٶ�
#define RR 0.2  //Բ�̰뾶
#define ee 0.1  // ƫ�ľ�
#define Jc 0.045  //�����ĵ�ת������
#define mu 0.15  //��Ħ��ϵ�� 0.075*n (ȡn<=10)
#define mus 0.2  //��Ħ��ϵ��
void vFsFN(double v,double theta,double dtheta,double vFF[]);//��Ħ������֧����������vFF[0]Ϊ�Ӵ�������ٶ�v��vFF[1]ΪFs��vFF[2]ΪFN
double TT(double theta,double dtheta,double dx0,double dy0);//����
double VV(double theta,double y0);//������
int func (double t, const double y[], double f[],void *params)
{
(void)(t); /* avoid unused parameter warning*/ 
	//f
	double vFF[3];
	vFF[0]=y[3]+RR*y[1];  //�Ӵ����ٶȣ������ʱ����Ϊ����͵�ˮƽ�ٶ�
	if(y[4]<=ZERO){  //�����Ӵ�ʱ
		vFsFN(vFF[0],y[0],y[1],vFF);}
	else {  //�뿪����ʱ
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
T=TT(y[0],y[1],y[3],y[5]);  //����
V=VV(y[0],y[4]);  //������
int status = gsl_odeiv_evolve_apply (e, c, s,&sys,&t, t1,&h, y);
if (status != GSL_SUCCESS)
break;
printf ("%.2e\n",t );
fprintf (fp,"%.2e   %.5e %.5e %.5e %.5e %.5e %.5e   %.5e %.5e \n", t, y[0], y[1],y[2],y[3], y[4],y[5],T,V);



	double vFF[3];
	vFF[0]=y[3]+RR*y[1];  //�Ӵ����ٶȣ������ʱ����Ϊ����͵�ˮƽ�ٶ�
	if(y[4]<=1e-4){  //�����Ӵ�ʱ
		vFsFN(vFF[0],y[0],y[1],vFF);
	}
	else{  //�뿪����ʱ
		vFF[1]=0; vFF[2]=0;
	}
fprintf(fp1,"%5.5f,%5.5f,%5.5f\n",vFF[0],vFF[1],vFF[2]);  //�������ٶ�v,Fs,FN
}

gsl_odeiv_evolve_free (e);
gsl_odeiv_control_free (c);
gsl_odeiv_step_free (s);
return 0;
}
 
//**************************************************************************
 void vFsFN(double v,double theta,double dtheta,double vFF[])//��Ħ����
  {
	  double Fs,FN,A,B,ddtheta;

	  if(fabs(v)>ERR){ //����Ի���ʱ
		  A=(RR-ee*cos(theta))/(Jc+mm*ee*ee*sin(theta)*sin(theta));
		  B=(mm*gg*ee*sin(theta)+mm*ee*ee*dtheta*dtheta*sin(theta)*cos(theta))/(Jc+mm*ee*ee*sin(theta)*sin(theta));
		  if(v>ERR)
			  Fs=-(mu*mm*gg+mu*mm*ee*dtheta*dtheta*cos(theta)-mu*mm*ee*sin(theta)*B)/(1+mu*mm*ee*sin(theta)*A);
		  else
			  Fs=(mu*mm*gg+mu*mm*ee*dtheta*dtheta*cos(theta)-mu*mm*ee*sin(theta)*B)/(1-mu*mm*ee*sin(theta)*A);
		  FN=mm*gg+mm*ee*dtheta*dtheta*cos(theta)+mm*ee*sin(theta)*(A*Fs-B);
	  } 
	  else{  //����Ի���ʱ
		  ddtheta=(-mm*ee*RR*dtheta*dtheta*sin(theta)-mm*gg*ee*sin(theta))/(mm*ee*ee+Jc+mm*RR*RR-2*mm*ee*RR*cos(theta));
		  Fs=-mm*RR*ddtheta+mm*ee*ddtheta*cos(theta)-mm*ee*dtheta*dtheta*sin(theta);
		  FN=mm*gg+mm*ee*ddtheta*sin(theta)+mm*ee*dtheta*dtheta*cos(theta);
		  if(Fs>mus*FN)
			  Fs=mus*FN;
		  else if(Fs<-mus*FN)
			  Fs=-mus*FN;	  
	  }
	  if(FN<=0){  //�������
		  Fs=0; FN=0;
	  }
	  vFF[1]=Fs;  //Ħ����
	  vFF[2]=FN;  //֧����
	  return;
  }

  double TT(double theta,double dtheta,double dx0,double dy0)//����
  {
	  double M,N,T;
	  M=(dx0+ee*dtheta*cos(theta))*(dx0+ee*dtheta*cos(theta));
	  N=(dy0+ee*dtheta*sin(theta))*(dy0+ee*dtheta*sin(theta));
	  T=0.5*mm*(M+N)+0.5*Jc*dtheta*dtheta;
      return T;   
  }
  double VV(double theta,double y0)//������
  {
	  double V;
	  V=mm*gg*(y0+RR-ee*cos(theta));
	  return V;  
  }
