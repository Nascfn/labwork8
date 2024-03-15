#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
Paulo Fernandes Nascimento
Lab Work #8
Write the mergeSort function so it can merge sort and keep track of allocated memory
*/
int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	if(l < r)
	{
		//finds array midpoint
		int m = l + (r - l) / 2;

		//splits the first half in half
		mergeSort(pData, l, m);
		//splits second half in half
		mergeSort(pData, m+1, r);

		//merges sorted arrays
		int leftSide = m - l + 1;
		int rightSide = r - m;

		//creates and allocates pointer for left and right sides
		int* leftAr = (int*)Alloc(sizeof(int) * leftSide);
		int* rightAr = (int*)Alloc(sizeof(int) * rightSide);

		//fills those two arrays with numbers from og array
		for(int i = 0; i < leftSide; i++)
		{
			leftAr[i] = pData[l + i];
		}
		for(int i = 0; i < rightSide; i++)
		{
			rightAr[i] = pData[m + 1 + i];
		}

		//index of leftAr
		int i = 0;
		//index of rightAr
		int j = 0;
		//index of merged array
		int k = l;

		while(i < leftSide && j < rightSide)
		{
			//compare both arrays if left is greater then add it to pData and increase index
			//else add right side and increase its index
			if(leftAr[i] <= rightAr[j])
			{
				pData[k] = leftAr[i];
				i++;
			}
			else
			{
				pData[k] = rightAr[j];
				j++;
			}
			//+1 to actual array index
			k++;
		}

		//since we prioritize left array this portion will copy any last numbers from right array
		while(j < rightSide)
		{
			pData[k] = rightAr[j];
			j++;
			k++;
		}

		//frees memory from ptr's
		DeAlloc(leftAr);
		DeAlloc(rightAr);
	}
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = (dataSz > 100 ? dataSz - 100 : 0);
	int firstHundred = (dataSz < 100 ? dataSz : 100);
	printf("\tData:\n\t");
	for (i=0;i<firstHundred;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}
/*
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}
*/
int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "/Users/paulofn/Desktop/CS1/Lab8/lab_8/input1.txt", "/Users/paulofn/Desktop/CS1/Lab8/lab_8/input2.txt", "/Users/paulofn/Desktop/CS1/Lab8/lab_8/input3.txt", "/Users/paulofn/Desktop/CS1/Lab8/lab_8/input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}