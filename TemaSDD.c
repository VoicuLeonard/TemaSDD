#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#define LINESIZE 128

typedef struct Car Car;
typedef struct Node Node;

struct Car {
	int year;
	char* manufacturer;
	float cc;
};

struct Node {
	Car car;
	Node* next;
};



void insertAtBeginning(Node** start, Car car) {
	
	Node* newNode = malloc(sizeof(Node));

	newNode->car = car;
	newNode->next = *start;

	*start = newNode;
}

void insertAtEnd(Node** start, Car car) {

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

void insertAfterNode(Node* start, Node* currentNode, Car newCar) {
	if (currentNode == NULL) {
		printf("Inserarea nu poate fi efectuata.\n");
		return;
	}

	Node* newNode = malloc(sizeof(Node));
	newNode->car = newCar;

	newNode->next = currentNode->next;
	currentNode->next = newNode;
}

Node* findNodeByYear(Node* start, int year) {
	while (start != NULL) {
		if (start->car.year == year) {
			return start;
		}
		start = start->next;
	}
	return NULL; 
}


Car deleteFromBeginning(Node** start) {

	Node* deleted = *start;
	*start = (*start)->next;

	Car deletedCar = deleted->car;
	free(deleted);

	return deletedCar;
}

Car deleteFromEnd(Node** start) {
	if (*start == NULL) {
		printf("Lista este goala!\n");
		return;
	}

	Node* temp = *start;
	Node* prev = NULL;

	if (temp->next == NULL) { 
		Car deletedCar = temp->car;
		free(temp);
		*start = NULL;
		return deletedCar;
	}

	while (temp->next) {
		prev = temp;
		temp = temp->next;
	}

	Car deletedCar = temp->car;
	free(temp);
	prev->next = NULL;

	return deletedCar;
}

Car deleteFromMiddle(Node** start, int year) {
	if (*start == NULL) {
		printf("Lista este goala!\n");
		return;
	}

	Node* temp = *start;
	Node* prev = NULL;

	while (temp && temp->car.year != year) {
		prev = temp;
		temp = temp->next;
	}

	if (temp == NULL) {
		printf("Masina cu anul %d nu a fost gasita.\n", year);
		return;
	}

	if (prev == NULL) { 
		*start = temp->next;
	}
	else {
		prev->next = temp->next;
	}

	Car deletedCar = temp->car;
	free(temp);

	return deletedCar;
}

void printCar(Car car) {
	printf("%s from %d has %5.2f\n", car.manufacturer, car.year, car.cc);
}

void parseListAndPrint(Node* start) {
	while (start) {
		printCar(start->car);
		start = start->next;
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

void readCarsFromFile(Node** start, int* noCars, const char* fileName) {

	FILE* f;
	f = fopen(fileName, "r");

	char buffer[LINESIZE];
	fgets(buffer, LINESIZE, f);

	*noCars = (int)strtol(buffer, NULL, 10);

	for (int i = 0; i < (*noCars); i++) {
		insertAtEnd(start, readCarFromFile(f));
	}

	fclose(f);

}

int main() {

	int noCars = 0;
	Node* list = NULL;

	readCarsFromFile(&list, &noCars, "cars.txt");
	parseListAndPrint(list);

	printf("-------------------\n");

	Car deletedCar = deleteFromBeginning(&list);

	parseListAndPrint(list);

	printf("-------------------\n");

	printCar(deletedCar);
	free(deletedCar.manufacturer);

	printf("-------------------\n");

	int searchYear;
	printf("Introduceti anul masinii dupa care doriti sa inserati: ");
	scanf("%d", &searchYear);

	Node* foundNode = findNodeByYear(list, searchYear);
	if (foundNode) {
		Car newCar = { 2024, "Ferrari", 3.9 };  
		insertAfterNode(list, foundNode, newCar);
		printf("Masina a fost inserata dupa anul %d.\n", searchYear);
	}
	else {
		printf("Nu s-a gasit nicio masina cu anul %d.\n", searchYear);
	}

	printf("-------------------\n");
	printf("Lista dupa inserarea unei masini noi:\n");
	parseListAndPrint(list);


	int yearToDelete;
	printf("-------------------\n");
	printf("Introduceti anul masinii pe care doriti sa o stergeti din interior: ");
	scanf("%d", &yearToDelete);

	Car deletedCarMid = deleteFromMiddle(&list, yearToDelete);
	printf("Masina stearsa din interior:\n");
	printCar(deletedCarMid);
	free(deletedCarMid.manufacturer);

	printf("-------------------\n");
	printf("Lista dupa stergerea din interior:\n");
	parseListAndPrint(list);


	return 0;
}
