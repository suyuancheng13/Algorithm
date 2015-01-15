#include"BP_NetWork.h"
int main(int argc, char* argv[])
{

	double Data[90][3] ;
	double Test[60][3];
	FILE *fp1 = fopen("F:\\MATLAB701\\work\\A\\lbp_glcm_all1.txt","r");
	FILE *fp2 = fopen("F:\\MATLAB701\\work\\B\\lbp_glcm_all1.txt","r");
	FILE *fp3 = fopen("F:\\MATLAB701\\work\\C\\lbp_glcm_all1.txt","r");
	for(int i =0;i<150;i++)
	{
		for(int j=0;j<3;j++)
			if(i<30)
				{
					fscanf(fp1,"%lf,",&Data[i][j]);
					Data[i][j]*=0.7;
		}
			else if(i<50)
				{
					fscanf(fp1,"%lf,",&Test[i-30][j]);
					Test[i-30][j]*=0.7;
			}
			else if(i<80)
			{
			   fscanf(fp2,"%lf,",&Data[i-20][j]);
			   Data[i-20][j]*=0.9;
			}
			else if(i<100)
			{
				fscanf(fp2,"%lf,",&Test[i-60][j]);
				Test[i-60][j]*=0.9;
			}
			else if(i<130)
				{
					fscanf(fp3,"%lf,",&Data[i-40][j]);
					Data[i-40][j]*=1.05;

			}
			else
				{
					fscanf(fp3,"%lf,",&Test[i-90][j]);
					Test[i-90][j]*=1.05;
			}
	}
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	/*for(int i=0;i<90;i++)
	{	for(int j=0;j<4;j++)
			printf("\t%lf",Data[i][j]);
	printf("\n");
	}*/
	double T[90][2];

	for(int i=0;i<90;i++)
		for(int j =0;j<2;j++)
		{
			if(i<30)
			    T[i][j] = 0;
			else if(i<60)
			{
				if( j ==0)
					T[i][j] =0;
				else
					T[i][j] = 1;
			}
			else
			{
				if( j ==0)
					T[i][j] =1;
				else
					T[i][j] = 0;
			}
		}
		// prepare XOR traing data
	int i=0;
	double data[][4]={
				0,	0,	0,	0,
				0,	0,	1,	1,
				0,	1,	0,	1,
				0,	1,	1,	0,
				1,	0,	0,	1,
				1,	0,	1,	0,
				1,	1,	0,	0,
				1,	1,	1,	1 };

				double p[4][2] = {-1,0,-1,5,2,0,2,5};//{-1 ,-1, 2 ,2,0 ,5 ,0 ,5};
				double t[4]={-1,-1,1,1};
	// prepare test data
	double target[8][2]  = {  0,0,
											0,1,
											0,1,
											0,0,
											0,1,
											0,0,
											0,0,
											0,1};
	double testData[][3]={
								0,      0,      0,
                                0,      0,      1,
                                0,      1,      0,
                                0,      1,      1,
                                1,      0,      0,
                                1,      0,      1,
                                1,      1,      0,
                                1,      1,      1};

	
	// defining a net with 4 layers having 3,3,3, and 1 neuron respectively,
	// the first layer is input layer i.e. simply holder for the input parameters
	// and has to be the same size as the no of input parameters, in out example 3
	int numLayers = 3, lSz[3] = {3,6,2};


	double Thresh =  0.0001;

	int inputn = 90;
	int testInputN = 60;
//	long i;
	
	BP_NetWork *bpnet = new BP_NetWork(numLayers,lSz,5000,Thresh,inputn,Data[0],T[0]);
	bpnet->Train();
	bpnet->Simulate(Test[0],testInputN);
	
	return 0;
}