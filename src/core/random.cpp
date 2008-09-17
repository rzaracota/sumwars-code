#include "random.h"

int Random::randDiscrete(float* distribution,int nr, float sum)
{
	float x = randf(sum);
	for (int i=0;i<nr;i++)
	{
		x-= distribution[i];
		if (x<=0)
		{
			return i;
		}
	}
}


int Random::randDiscrete(vector<float> distribution)
{
	vector<float>::iterator i;
	float sum =0;
	for (i= distribution.begin(); i!=distribution.end(); ++i)
	{
		sum += *i;
	}
	float x = randf(sum);
	int res =0;
	for (i=distribution.begin();i!=distribution.end();++i)
	{
		x-= *i;
		if (x<=0)
		{
			return res;
		}
		res++;
	}
}


