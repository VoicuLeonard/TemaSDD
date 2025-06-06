#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#define LINESIZE 128

typedef struct Car Car;
typedef struct Node Node;
typedef struct HashTable HT;

struct Car {
	int year;
	char* manufacturer;
	float cc;
};

struct Node {
	Car car;
	Node* next;
};

struct HashTable {
	int size;
	Node** array;
};

int hash(int hashTableSize, char* key) {
	int sum = 0;
	for (int i = 0; i < strlen(key); i++) {
		sum += key[i];
	}
	return sum % hashTableSize;
}

HT initHashTable(int size) {
	HT hashTable;
	hashTable.size = size;
	hashTable.array = malloc(hashTable.size * sizeof(Node*));

	for (int i = 0; i < hashTable.size; i++) {
		hashTable.array[i] = NULL;
	}

	return hashTable;
}

void listInsert(Node** start, Car car) {

	Node* newNode = malloc(sizeof(Node));
	newNode->car = car;
	newNode->next = NULL;

	if (*start) {

		Node* aux = *start;

		while (aux->next) {
			aux = aux->next;
		}

		aux->next = newNode;

	}
	else {
		*start = newNode;
	}
}

void hashTableInsert(HT hashTable, Car car) {

	int position = hash(hashTable.size, car.manufacturer);

	// if (hashTable.array[position]) {

		// if block is redundat, we only exemplify a collision
		// we have collision

	// 	listInsert(&(hashTable.array[position]), car);

	// } else {

		// we don't have collision -> list is empty -> but we still use the very same function to insert into the list

	// 	listInsert(&(hashTable.array[position]), car);

	// }

	listInsert(&(hashTable.array[position]), car);
}



void deleteCarByManufacturer(Node** start, const char* manufacturer) {

	if (*start) {

		Node* previous = NULL;
		Node* current = *start;

		while (current) {

			if (strcmp(current->car.manufacturer, manufacturer) == 0) {

				if (previous) {

					Node* toBeDeleted = current;
					previous->next = current->next;
					current = current->next;

					free(toBeDeleted->car.manufacturer);
					free(toBeDeleted);

				}
				else {

					Node* toBeDeleted = current;

					current = current->next;
					*start = current;

					free(toBeDeleted->car.manufacturer);
					free(toBeDeleted);
				}

			}
			else {
				previous = current;
				current = current->next;
			}
		}

	}
	else {
		printf("List is empty");
	}
}


void printCar(Car car) {
	printf("%s from %d has %5.2f\n", car.manufacturer, car.year, car.cc);
}

void parseListAndPrint(Node* start) {
	while (start) {
		printf("\t");
		printCar(start->car);
		start = start->next;
	}
}

void printHashTable(HT hashTable) {
	for (int i = 0; i < hashTable.size; i++) {
		printf("Bucket %d: \n", i);
		parseListAndPrint(hashTable.array[i]);
	}
}

Car readCarFromFile(FILE* f) {

	char buffer[LINESIZE];

	fgets(buffer, LINESIZE, f);

	Car car;

	char* carData = NULL;

	carData = strtok(buffer, ",");
	car.year = atoi(carData);

	carData = strtok(NULL, ",");
	car.manufacturer = malloc((strlen(carData) + 1) * sizeof(char));
	strcpy(car.manufacturer, carData);

	carData = strtok(NULL, ",");
	car.cc = atof(carData);

	return car;

}

HT readCarsFromFile(int* noCars, const char* fileName) {

	FILE* f;
	f = fopen(fileName, "r");

	char buffer[LINESIZE];
	fgets(buffer, LINESIZE, f);

	*noCars = (int)strtol(buffer, NULL, 10);

	HT hashTable = initHashTable(*noCars);

	for (int i = 0; i < *noCars; i++) {
		hashTableInsert(hashTable, readCarFromFile(f));
	}

	fclose(f);

	return hashTable;
}

Car searchHashTable(HT hashTable, char* key) {
	int position = hash(hashTable.size, key);

	Node* aux = hashTable.array[position];

	while (aux && strcmp(key, aux->car.manufacturer)) {
		aux = aux->next;
	}

	if (aux) {
		printf("\nFound at position %d", position);
		return aux->car;
	}
	else {
		Car err = { .year = 0, .manufacturer = "", .cc = 0 };
		return err;
	}
}

void freeList(Node* start) {
	Node* current = start;
	while (current) {
		Node* toDelete = current;
		current = current->next;
		free(toDelete->car.manufacturer); 
		free(toDelete); 
	}
}

void freeHashTable(HT* hashTable) {
	if (hashTable->array) {
		for (int i = 0; i < hashTable->size; i++) {
			freeList(hashTable->array[i]);
		}
		free(hashTable->array);
		hashTable->array = NULL;
		hashTable->size = 0;
	}
}

int main() {

	int noCars = 0;

	HT hashTable = readCarsFromFile(&noCars, "cars.txt");

	printHashTable(hashTable);

	freeHashTable(&hashTable);

	return 0;
}
