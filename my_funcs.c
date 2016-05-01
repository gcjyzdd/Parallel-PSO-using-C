#include "my_funcs.h"
void   state_eq(double x[4],double beta[26],double u,double f[4]){

		double z=x[0];
		if (z>1-1e-5)
				z=1-1e-5;
		if (z<1e-5)
				z=1e-5;

//		double C0=beta[0];
		double C1=-beta[1]*exp(-beta[2]*z)+beta[3]+beta[1];
		double C2=-beta[4]*exp(-beta[5]*z)+beta[6]+beta[4];
        double tau1=(beta[7]*exp(-beta[8]*z)+beta[9])*C1;
		double tau2=(beta[10]*exp(-beta[11]*z)+beta[12])*C2;

		if (tau1<1)
			tau1=1.0;

		if (tau2<1)
			tau2=1.0;
		double k=beta[24];
		double M=beta[25];
		f[0]=u/beta[0];	
		f[1]=-x[1]/tau1+u/C1;
		f[2]=-x[2]/tau2+u/C2;
		f[3]=-abs(u)*k*x[3]+k*M*u;
}

double OCV(double x[7],double y[8],double x0){
		double ocv=0;
		if ((x[0]<=x0)&&(x0<=x[1])){
			ocv=-y[0]*exp(-y[1]*x0)+y[2];
			return ocv;					
		}
		if (x0<x[2]){
			double a=-y[0]*exp(-y[1]*x[1])+y[2];
			ocv=(y[3]-a)/(x[2]-x[1])*(x0-x[1])+a;
			return ocv;
		}
		for (int i=2;i<6;i++){
			if(x0<=x[i+1])
					return (y[i+2]-y[i+1])/(x[i+1]-x[i])*(x0-x[i])+y[i+1];
		}
		printf("x0 is out of [0,1]!\n");
		return ocv;
}

double output_eq(double x[4],double beta[26],double u){
		
		double soc[]={0, 0.1,0.2,0.4,0.6,0.8,1};
		double K[8];
		for (int i=0;i<8;i++)
				K[i]=beta[i+16];
		double R0=beta[13]*exp(-beta[14]*x[0])+beta[15];
		if(x[0]>1-1e-5)
				x[0]=1-1e-5;
		if(x[0]<1e-5)
				x[0]=1e-5;

		return OCV(soc,K,x[0])+x[1]+x[2]+x[3]+u*R0;
}

void sys_output(double x[4],double beta[26],double Y[DATA_SIZE]){
		double dt=0.25;
		Y[0]=output_eq(x,beta,U_in[0]);
		double temp[4];
		for(int i=0;i<DATA_SIZE-2;i++){
			state_eq(x,beta,U_in[i],temp);
			for(int j=0;j<4;j++)
					x[j]=x[j]+temp[j]*dt;
			Y[i+1]=output_eq(x,beta,U_in[i+1]);
		}	
}

double my_cost_func(double pp[30]){
		double sum=0;
		
		double x0[4];
		double beta[26];
		for (int i=0;i<4;i++)
			x0[i]=pp[i];
		for (int i=4;i<30;i++)
			beta[i-4]=pp[i];
		double *Y_sim=malloc(3735658*sizeof(double));
		sys_output(x0,beta,Y_sim);
		for (int i=0;i<DATA_SIZE;i++)
				sum=sum+(Y_sim[i]-Z_out[i])*(Y_sim[i]-Z_out[i]);
		return sqrt(sum);
}

