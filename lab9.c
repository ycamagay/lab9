#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{
	struct RecordType record;
	struct HashType* head;
	struct HashType* next;
};

// Compute the hash function
int hash(int x)
{
	return x % 23;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{	
	int i;

	for (i=0;i<hashSz;++i)
	{
		// if index is occupied with any records, print all
		struct HashType* tmp = pHashArray[i].head;

		if(tmp == NULL){
			continue;
		}

		printf("index %d ", i);

		while(tmp != NULL){
			printf("-> %d, %c, %d ", tmp->record.id, tmp->record.name, tmp->record.order);
			tmp = tmp->next;
		}

		// when done move to the next
		printf("\n");
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input_lab_9.txt", &pRecords);
	printRecords(pRecords, recordSz);

	// Your hash implementation

	// init

	struct HashType *hashArray = (struct HashType*)malloc(sizeof(struct HashType) * 23);	
	
	for(int i = 0; i < 23; ++i){
		hashArray[i].head = NULL;
	}

	for(int j = 0; j < recordSz; ++j){
		int index = hash(pRecords[j].id);
		struct HashType* newHashType = (struct HashType*)malloc(sizeof(struct HashType));

		newHashType->record = pRecords[j];
		newHashType->next = hashArray[index].head;
		hashArray[index].head = newHashType;
	}

	displayRecordsInHash(hashArray, 23);

	for(int k = 0; k < 23; ++k){
		struct HashType* tmp = hashArray[k].head;

		while(tmp != NULL){
			struct HashType* next = tmp -> next;
			free(tmp);
			tmp = next;
		}
	}

	free(hashArray);
	free(pRecords);

	return 0;
}